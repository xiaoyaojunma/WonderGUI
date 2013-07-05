/*=========================================================================

                         >>> WonderGUI <<<

  This file is part of Tord Jansson's WonderGUI Graphics Toolkit
  and copyright (c) Tord Jansson, Sweden [tord.jansson@gmail.com].

                            -----------

  The WonderGUI Graphics Toolkit is free software; you can redistribute
  this file and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

                            -----------

  The WonderGUI Graphics Toolkit is also available for use in commercial
  closed-source projects under a separate license. Interested parties
  should contact Tord Jansson [tord.jansson@gmail.com] for details.

=========================================================================*/


#include <wg_combobox.h>
#include <wg_menu.h>
#include <wg_geo.h>
#include <wg_gfxdevice.h>
#include <wg_util.h>
#include <wg_eventhandler.h>
#include <wg_menulayer.h>

static const char	c_widgetType[] = {"Combobox"};


//____ WgCombobox() _________________________________________________________________

WgCombobox::WgCombobox( void )
{
	m_pText = &m_text;
	m_text.setAlignment( WG_WEST );
	m_text.SetWrap(false);
	m_text.SetAutoEllipsis(IsAutoEllipsisDefault());	
//	m_textColor = m_text.color();
//	m_textDisabledColor	= 0xFFFF;

	m_state = WG_STATE_NORMAL;

	m_bResetCursorOnFocus = true;
	m_bPressInInputRect = false;
	m_pMenu		= 0;
	m_pSelectedItem = 0;
	m_textFormat = "%1";
	m_viewOfs		= 0;
	m_maxCharacters = 0;
}

//____ Destructor _____________________________________________________________

WgCombobox::~WgCombobox()
{
	delete m_pMenu;
}


//____ Type() _________________________________________________________________

const char * WgCombobox::Type( void ) const
{
	return GetClass();
}

//____ GetClass() _____________________________________________________________

const char * WgCombobox::GetClass( void )
{
	return c_widgetType;
}

//____ PreferredSize() __________________________________________________________

WgSize WgCombobox::PreferredSize() const
{
	WgTextAttr attr;
	m_text.GetBaseAttr( attr );
	int width = WgTextTool::lineWidth( m_text.getNode(), attr, "MMMMMMMMMM" );		// Default combobox should fit 10 letter M in textfield
	WgSize contentSize( m_text.height(), width );
	
	if( m_pSkin )
		return m_pSkin->SizeForContent( contentSize );
	else
		return contentSize;
}


//____ SetMenu() ______________________________________________________________

void WgCombobox::SetMenu( WgMenu * pMenu )
{
	if( pMenu == m_pMenu )
		return;

	m_pMenu = pMenu;
	if( m_pMenu && m_pMenu->GetSelectedItem() )
		_entrySelected(m_pMenu->GetSelectedItem()->GetId());
}

//____ Menu() __________________________________________________________

WgMenu* WgCombobox::Menu() const
{
	return m_pMenu;
}

//____ SetEditMode() __________________________________________________________

void WgCombobox::SetEditMode(WgTextEditMode mode)
{
	m_text.SetEditMode(mode);
}

//____ SetTextFormat() ________________________________________________________

void WgCombobox::SetTextFormat( const WgCharSeq& str )
{
	m_textFormat = str;
	if( m_pMenu && m_pMenu->GetSelectedItem() )
		_entrySelected(m_pMenu->GetSelectedItem()->GetId());
}

//____ SetPlaceholderText() ___________________________________________________

void WgCombobox::SetPlaceholderText( const WgCharSeq& str )
{
	m_placeholderText = str;
	if( m_text.IsEmpty() && !m_text.isCursorShowing() )
		_requestRender();
}

//____ InsertTextAtCursor() ___________________________________________________

int WgCombobox::InsertTextAtCursor( const WgCharSeq& str )
{
	if( !_isEditable() )
		return 0;

	if( !m_state.IsFocused() )
		if( !GrabFocus() )
			return 0;				// Couldn't get input focus...

	int retVal = 0;

	if( m_maxCharacters == 0 || str.Length() < m_maxCharacters - m_pText->nbChars() )
	{
		m_pText->putText( str );
		retVal = str.Length();
	}
	else
	{
		retVal = m_maxCharacters - m_pText->nbChars();
		m_pText->putText( WgCharSeq( str, 0, retVal ) );
	}

	WgEventHandler * pHandler = _eventHandler();		
	if( pHandler )
		pHandler->QueueEvent( new WgEvent::TextModify(this,m_pText) );

	_adjustViewOfs();

	return retVal;
}

//____ InsertCharAtCursor() ___________________________________________________

bool WgCombobox::InsertCharAtCursor( Uint16 c )
{
	if( !_isEditable() )
		return 0;

	if( !m_state.IsFocused() )
		if( !GrabFocus() )
			return false;				// Couldn't get input focus...

	if( m_maxCharacters != 0 && m_maxCharacters < m_pText->nbChars() )
		return false;

	m_pText->putChar( c );

	WgEventHandler * pHandler = _eventHandler();		
	if( pHandler )
		pHandler->QueueEvent( new WgEvent::TextModify(this,m_pText) );

	_adjustViewOfs();
	return true;
}

//____ GoBOL() ________________________________________________________________

void WgCombobox::GoBOL()
{
	if( _isEditable() && m_state.IsFocused() )
		m_pText->goBOL();
}

//____ GoEOL() ________________________________________________________________

void WgCombobox::GoEOL()
{
	if( _isEditable() && m_state.IsFocused() )
		m_pText->goEOL();
}

//____ _closeMenu() ___________________________________________________________

void WgCombobox::_closeMenu()
{
	if( Parent() )
	{
		WgMenuLayer * pLayer = Parent()->_getMenuLayer();
		if( pLayer )
			pLayer->CloseMenu( m_pMenu );

		WgEventHandler * pEH = _eventHandler();
		if( pEH )
			pEH->DeleteCallback( WgEventFilter::MenuitemSelect( m_pMenu ), this );
	}
}

//____ _onEvent() _____________________________________________________________

void WgCombobox::_onEvent( const WgEvent::Event * _pEvent, WgEventHandler * pHandler )
{
	WgState oldState = m_state;

	switch( _pEvent->Type() )
	{
		case WG_EVENT_TICK:
			if( _isEditable() && m_state.IsFocused() )
			{
				m_text.incTime( static_cast<const WgEvent::Tick*>(_pEvent)->Millisec() );
				_requestRender();					//TODO: Should only render the cursor and selection!
			}
		break;

		case WG_EVENT_MENU_CLOSED:
			m_state.SetPressed(false);
		break;

		case WG_EVENT_MENUITEM_SELECT:
		break;

		case WG_EVENT_MOUSE_ENTER:
			m_state.SetHovered(true);
			break;

		case WG_EVENT_MOUSE_LEAVE:
			if( !m_state.IsPressed() )
				m_state.SetHovered(false);
			break;

		case WG_EVENT_MOUSE_POSITION:
		{
			WgCoord pos = static_cast<const WgEvent::MousePosition*>(_pEvent)->PointerPos();
			WgRect inputRect = m_pSkin ? m_pSkin->ContentRect(Size(),m_state): WgRect( 0,0, Size() );

			if( _isSelectable() && inputRect.Contains( pos ) )
			{
				m_pointerStyle = WG_POINTER_IBEAM;
			}
			else
			{
				m_pointerStyle = WG_POINTER_DEFAULT;
			}
			break;
		}



		case WG_EVENT_MOUSEBUTTON_PRESS:
		{
			const WgEvent::MouseButtonPress * pEvent = static_cast<const WgEvent::MouseButtonPress*>(_pEvent);
			WgCoord pos = pEvent->PointerPos();

			if( pEvent->Button() == 1 )
			{
				WgRect inputRect = m_pSkin ? m_pSkin->ContentRect( Size(), m_state ): WgRect( 0,0, Size() );

				if( m_state.IsPressed() && m_pMenu )
				{
					m_bPressInInputRect = false;
					_closeMenu();
				}
				else if( _isEditable() && inputRect.Contains(pos) )
				{
					if( !m_state.IsFocused() )
					{
						GrabFocus();
						if( m_state.IsFocused() )
							m_bFocusPress = true;		// Current button press brought focus.
					}

					m_bPressInInputRect = true;

					if( m_state.IsFocused() )
					{
						if( _isSelectable() && (pEvent->ModKeys() & WG_MODKEY_SHIFT))
						{
							m_pText->setSelectionMode(true);
						}


						int x = pos.x;
						int y = pos.y;
						x += m_viewOfs;

						m_pText->CursorGotoCoord( WgCoord(x, 0), WgRect(inputRect.x,0,1000000,1000000) );
				
						if(_isSelectable() && !(pEvent->ModKeys() & WG_MODKEY_SHIFT))
						{
							m_pText->clearSelection();
							m_pText->setSelectionMode(true);
						}
						_adjustViewOfs();
					}

				}
				else if( m_pMenu )
				{
					m_bPressInInputRect = false;

					if( Parent() )
					{
						WgMenuLayer * pLayer = Parent()->_getMenuLayer();
						if( pLayer )
						{
							pLayer->OpenMenu( m_pMenu, this, m_pHook->ScreenGeo() - pLayer->ScreenPos(), WG_SOUTHWEST );
							pHandler->AddCallback( WgEventFilter::MenuitemSelect( m_pMenu ), cbEntrySelected, this );
						}
					}

					m_state.SetPressed(true);
				}
				_pEvent->Swallow();
			}
			break;
		}

		case WG_EVENT_MOUSEBUTTON_DRAG:
		{
			const WgEvent::MouseButtonDrag * pEvent = static_cast<const WgEvent::MouseButtonDrag*>(_pEvent);
			if( pEvent->Button() == 1 )
			{
				if( m_state.IsFocused() && m_bPressInInputRect )
				{
					if( _isSelectable() && (pEvent->ModKeys() & WG_MODKEY_SHIFT) )
					{
						m_pText->setSelectionMode(true);
					}

					int x = pEvent->PointerPos().x + m_viewOfs;
					int leftBorder = m_pSkin ? m_pSkin->ContentRect( Size(), m_state ).x : 0;

					m_pText->CursorGotoCoord( WgCoord(x, 0), WgRect(leftBorder,0,1000000,1000000) );
					_adjustViewOfs();
				}
				_pEvent->Swallow();
			}
			break;
		}

		case WG_EVENT_MOUSEBUTTON_RELEASE:
		{
			const WgEvent::MouseButtonRelease * pEvent = static_cast<const WgEvent::MouseButtonRelease*>(_pEvent);
			if( pEvent->Button() == 1 )
			{
				if( m_state.IsFocused() )
				{
					m_pText->setSelectionMode(false);
					if( m_bFocusPress )
					{
						m_bFocusPress = false;
						if( !m_pText->hasSelection() )
							m_pText->selectAll();
					}
				}
				_pEvent->Swallow();
			}
			break;
		}

		case WG_EVENT_MOUSEWHEEL_ROLL:
		{		
			const WgEvent::MouseWheelRoll * pEvent = static_cast<const WgEvent::MouseWheelRoll*>(_pEvent);
			if( !m_state.IsFocused() && m_pMenu && m_pMenu->GetItemCount() != 0 )
			{
				WgMenuItem * pItem = m_pSelectedItem;
				int distance = pEvent->Distance();

				if( !pItem )
				{
					if( distance > 0 )
					{
						pItem = m_pMenu->GetLastItem();
						distance--;
					}
					if( distance < 0 )
					{
						pItem = m_pMenu->GetFirstItem();
						distance++;
					}
				}

				while( distance > 0 && pItem->Prev() )
				{
					pItem = pItem->Prev();
					distance--;
				}

				while( distance < 0 && pItem->Next() )
				{
					pItem = pItem->Next();
					distance++;
				}

				m_pMenu->SelectItem( pItem );
				_entrySelected( pItem->GetId() );
				_pEvent->Swallow();
			}
			break;
		}

		case WG_EVENT_CHARACTER:
		{
			const WgEvent::Character * pEvent = static_cast<const WgEvent::Character*>(_pEvent);
			if( _isEditable() && m_state.IsFocused() )
			{

				if(m_pText->hasSelection())
					m_pText->delSelection();
				m_pText->setSelectionMode(false);

				// by default - no max limit
				if( m_maxCharacters == 0 || m_maxCharacters > m_pText->nbChars() )
					m_pText->putChar( pEvent->Char() );

				pHandler->QueueEvent( new WgEvent::TextModify(this,m_pText) );
				_adjustViewOfs();
			}
			break;
		}

		case WG_EVENT_KEY_RELEASE:
			if( m_state.IsFocused() )
			{
				const WgEvent::KeyRelease * pEvent = static_cast<const WgEvent::KeyRelease*>(_pEvent);
				switch( pEvent->TranslatedKeyCode() )
				{
					case WG_KEY_SHIFT:
						if(!pHandler->IsMouseButtonPressed(1))
							m_pText->setSelectionMode(false);
						_pEvent->Swallow();
					break;
				}
			}
		break;

		case WG_EVENT_KEY_PRESS:
		case WG_EVENT_KEY_REPEAT:
		{
			const WgEvent::KeyEvent * pEvent = static_cast<const WgEvent::KeyEvent*>(_pEvent);

			if( pEvent->TranslatedKeyCode() == WG_KEY_ESCAPE && m_state.IsPressed() )
			{
				_closeMenu();
			}
			else if( _isEditable() && m_state.IsFocused() )
			{
				switch( pEvent->TranslatedKeyCode() )
				{
					case WG_KEY_LEFT:
						if( pEvent->ModKeys() & WG_MODKEY_SHIFT )
							m_pText->setSelectionMode(true);

						if( pEvent->ModKeys() & WG_MODKEY_CTRL )
							m_pText->gotoPrevWord();
						else
							m_pText->goLeft();
						_pEvent->Swallow();
						break;
					case WG_KEY_RIGHT:
						if( pEvent->ModKeys() & WG_MODKEY_SHIFT )
							m_pText->setSelectionMode(true);

						if( pEvent->ModKeys() & WG_MODKEY_CTRL )
								m_pText->gotoNextWord();
						else
							m_pText->goRight();
						_pEvent->Swallow();
						break;

					case WG_KEY_BACKSPACE:
						if(m_pText->hasSelection())
							m_pText->delSelection();
						else if( pEvent->ModKeys() & WG_MODKEY_CTRL )
							m_pText->delPrevWord();
						else
							m_pText->delPrevChar();
						
						pHandler->QueueEvent( new WgEvent::TextModify(this, m_pText) ); //TODO: Should only emit if text really has changed
						_pEvent->Swallow();
						break;

					case WG_KEY_DELETE:
						if(m_pText->hasSelection())
							m_pText->delSelection();
						else if( pEvent->ModKeys() & WG_MODKEY_CTRL )
							m_pText->delNextWord();
						else
							m_pText->delNextChar();
						pHandler->QueueEvent( new WgEvent::TextModify(this, m_pText) );		//TODO: Should only emit if text really has changed
						_pEvent->Swallow();
						break;

					case WG_KEY_HOME:

						//
						//	I am not sure if this is the proper way to this, but in my opinion, the default
						//	"actions" has to be separated from any modifier key action combination
						//
						switch( pEvent->ModKeys() )
						{

						case WG_MODKEY_CTRL:
							break;

						default: // no modifier key was pressed
							if( pEvent->ModKeys() & WG_MODKEY_SHIFT )
								m_pText->setSelectionMode(true);

							m_pText->goBOL();
							break;
						}
						_pEvent->Swallow();
						break;

					case WG_KEY_END:

						//
			 			//	I am not sure if this is the proper way to this, but in my opinion, the default
		 				//	"actions" has to be separated from any modifier key action combination
						//
						switch( pEvent->ModKeys() )
						{

						case WG_MODKEY_CTRL:
							break;

						default: // no modifier key was pressed
							if( pEvent->ModKeys() & WG_MODKEY_SHIFT )
								m_pText->setSelectionMode(true);

							m_pText->goEOL();
							break;
						}

						_pEvent->Swallow();
						break;
				}
				_adjustViewOfs();
			}
		}
		break;
	}

	if( m_state != oldState )
		_onStateChanged( oldState, m_state );
}

//____ _onStateChanged() ______________________________________________________

void WgCombobox::_onStateChanged( WgState oldState, WgState newState )
{
	WgWidget::_onStateChanged( oldState, newState );

	m_text.setState( newState );

	// Check if we got focus

	if( newState.IsFocused() && !oldState.IsFocused() )
	{
		if( _isEditable() )
		{
			_startReceiveTicks();
			m_text.showCursor();
			if( m_bResetCursorOnFocus )
			{
				m_text.goEOL();
				m_text.selectAll();
			}
		}
	}

	// Check if we lost focus

	if( !newState.IsFocused() && oldState.IsFocused() )
	{
		if( _isEditable() )
		{
			_stopReceiveTicks();
			m_text.hideCursor();
			m_text.clearSelection();
			m_bResetCursorOnFocus = true;
			_eventHandler()->QueueEvent( new WgEvent::TextSet( this, &m_text ) );	//TODO: Should only do if text was really changed!
		}
	}
}

//____ _onSkinChanged() _______________________________________________________

void WgCombobox::_onSkinChanged( const WgSkinPtr& pOldSkin, const WgSkinPtr& pNewSkin )
{
	WgWidget::_onSkinChanged(pOldSkin,pNewSkin);
	m_text.SetColorSkin(pNewSkin);
}



//____ _onRender() ________________________________________________________

void WgCombobox::_onRender( WgGfxDevice * pDevice, const WgRect& _canvas, const WgRect& _window, const WgRect& _clip )
{
	WgWidget::_onRender( pDevice, _canvas, _window, _clip );

	// Print the text

	WgRect r( _canvas );
	if( m_pSkin )
		r = m_pSkin->ContentRect(_canvas, m_state);

	WgRect	textClip( r, _clip );

	bool bPlaceholder = false;
	if( !m_placeholderText.IsEmpty() && m_text.IsEmpty() && !m_text.isCursorShowing() )
	{
		bPlaceholder = true;
		m_text.setText( m_placeholderText );
	}

	r.x -= m_viewOfs;
	r.w += m_viewOfs;
	pDevice->PrintText( textClip, &m_text, r );

	if( bPlaceholder )
		m_text.clear();
}

//____ _onRefresh() _______________________________________________________

void WgCombobox::_onRefresh( void )
{
}

//____ _onCloneContent() _______________________________________________________

void WgCombobox::_onCloneContent( const WgWidget * _pOrg )
{
	const WgCombobox * pOrg = WgCast<WgCombobox>(_pOrg);
	if( pOrg )
	{
		m_textFormat = pOrg->m_textFormat;
		m_placeholderText = pOrg->m_placeholderText;
		m_pMenu = pOrg->m_pMenu;
		m_pSelectedItem = pOrg->m_pSelectedItem;
		m_text = pOrg->m_text;
		m_maxCharacters = pOrg->m_maxCharacters;
	}
}


//____ _adjustViewOfs() ________________________________________________________

void WgCombobox::_adjustViewOfs()
{
	// Possibly move viewOfs so that:
	//	1 Cursor remains inside view.
	//  2 At least one character is displayed before the cursor
	//  3 At least one character is displayed after the cursor (if there is one).

	if( m_state.IsFocused() && m_pText->getFont() )
	{
		WgCursor * pCursor = WgTextTool::GetCursor( m_pText );
		if( !pCursor )
			return;

		int cursCol	= m_pText->column();

		WgTextAttr	attr;
		m_pText->GetBaseAttr( attr );

		int cursAdvance	= pCursor->Advance(m_pText->cursorMode() );
		int cursBearing	= pCursor->BearingX(m_pText->cursorMode() );
		int cursWidth	= pCursor->Width(m_pText->cursorMode() );

		int cursOfs;		// Cursor offset from beginning of line in pixels.
		int maxOfs;			// Max allowed view offset in pixels.
		int minOfs;			// Min allowed view offset in pixels.

		int geoWidth = Size().w;
		if( m_pSkin )
			geoWidth -= m_pSkin->ContentPadding().w;

		int	lineWidth = m_pText->getSoftLineWidth( 0 ) + cursBearing+cursWidth;

		// Calculate cursOfs

		cursOfs	= m_pText->getSoftLineWidthPart( 0, 0, cursCol );

		// Calculate maxOfs

		if( cursCol > 0 )
		{
			maxOfs = m_pText->getSoftLineWidthPart( 0, 0, cursCol-1 );

			if( lineWidth < maxOfs + geoWidth )
				maxOfs = WgMax( lineWidth - geoWidth, 0 );
		}
		else
			maxOfs = cursOfs;


		// Calculate minOfs

		if( cursCol < m_pText->getLine(0)->nChars )
			minOfs = m_pText->getSoftLineWidthPart( 0, 0, cursCol+1 ) + cursAdvance - geoWidth;	// Not 100% right, cursor might affect linewidth different from its own width.
		else
			minOfs = cursOfs + cursBearing + cursWidth - geoWidth;

		// Check boundaries and update

		if( m_viewOfs > maxOfs )
			m_viewOfs = maxOfs;

		if( m_viewOfs < minOfs )
			m_viewOfs = minOfs;


	}
	else
		m_viewOfs = 0;				// Show beginning of line when cursor disappears.
}

//____ _onAlphaTest() ______________________________________________________

bool WgCombobox::_onAlphaTest( const WgCoord& ofs )
{
	//TODO: Should we treat text-box as opaque for mouse?

	return WgWidget::_onAlphaTest(ofs);
}


//____ _textModified() _________________________________________________________

void WgCombobox::_textModified()
{
	m_bResetCursorOnFocus = true;
	WgEventHandler * pHandler = _eventHandler();		
	if( pHandler )
		pHandler->QueueEvent( new WgEvent::TextModify(this,m_pText) );
	_requestRender();
//	_adjustViewOfs();
}

//____ _entrySelected() ________________________________________________________

void WgCombobox::_entrySelected( int itemId )
{
	WgMenuItem * pItem = m_pMenu->FindItem(itemId);
	if( !pItem )
		return ;

	m_pSelectedItem = pItem;
	if(pItem && pItem->GetType() != SEPARATOR)
	{
		WgCharBuffer	buff;

		buff.PushBack(m_textFormat);

		int ofs = buff.FindFirst( "%1" );
		if( ofs >= 0 )
		{
			WgTextpropPtr pProp = buff[ofs].Properties();

			const WgChar * pEntry = ((WgMenuEntry*)pItem)->GetText().Chars();
			Uint32 len = WgTextTool::strlen( pEntry );

			buff.Replace( ofs, 2, pEntry );

			WgChar * p = buff.BeginWrite() + ofs;

			for( unsigned int i = 0 ; i < len ; i++ )
			{
				if( p[i].PropHandle() == 0 )
					p[i].SetProperties( pProp );
			}

			buff.EndWrite();

		}
		SetText( &buff );
		_adjustViewOfs();
	}
}
