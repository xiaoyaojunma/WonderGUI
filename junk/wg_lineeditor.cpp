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

#include <wg_lineeditor.h>

#include	<wg_key.h>
#include	<wg_font.h>
#include 	<wg_gfxdevice.h>
#include 	<wg_pen.h>
#include 	<wg_msgrouter.h>
#include	<wg_base.h>

namespace wg 
{
	
	
	
	const char LineEditor::CLASSNAME[] = {"LineEditor"};
	
	//____ Constructor ____________________________________________________________
	
	LineEditor::LineEditor() : m_text(this), text(&m_text)
	{
		m_text.setWrap(false);
		m_text.setAutoEllipsis(isAutoEllipsisDefault());
		m_text.setEditMode( TextEditMode::Editable );
		m_bPasswordMode = false;
		m_pwGlyph		= '*';
		m_viewOfs		= 0;
		m_pointerStyle	= PointerStyle::Ibeam;
		m_bResetCaretOnFocus = true;
		m_tickRouteId = 0;
	}
	
	//____ Destructor _____________________________________________________________
	
	LineEditor::~LineEditor()
	{
		if( m_tickRouteId )
			Base::msgRouter()->deleteRoute( m_tickRouteId );
	}
	
	//____ isInstanceOf() _________________________________________________________
	
	bool LineEditor::isInstanceOf( const char * pClassName ) const
	{ 
		if( pClassName==CLASSNAME )
			return true;
	
		return Widget::isInstanceOf(pClassName);
	}
	
	//____ className() ____________________________________________________________
	
	const char * LineEditor::className( void ) const
	{ 
		return CLASSNAME; 
	}
	
	//____ cast() _________________________________________________________________
	
	LineEditor_p LineEditor::cast( const Object_p& pObject )
	{
		if( pObject && pObject->isInstanceOf(CLASSNAME) )
			return LineEditor_p( static_cast<LineEditor*>(pObject.rawPtr()) );
	
		return 0;
	}
	
	//____ setEditMode() __________________________________________________________
	
	void LineEditor::setEditMode(TextEditMode mode)
	{
		m_text.setEditMode( mode );
	
		if( _isSelectable() )
		{
			m_pointerStyle = PointerStyle::Ibeam;
		}
		else
		{
			m_pointerStyle = PointerStyle::Default;
		}
	}
	
	//____ setPasswordGlyph() _____________________________________________________
	
	void LineEditor::setPasswordGlyph( uint16_t glyph )
	{
		assert(glyph);
		if(glyph)
		{
			m_pwGlyph = glyph;
		}
	}
	
	//____ insertTextAtCaret() ___________________________________________________
	
	int LineEditor::insertTextAtCaret( const CharSeq& str )
	{
		if( !_isEditable() )
			return 0;
	
		if( !m_state.isFocused() )
			if( !grabFocus() )
				return 0;				// Couldn't get input focus...
	
		int retVal = m_text.putText( str );
	
		Base::msgRouter()->post( new TextEditMsg(text.ptr(),false) );
	
		_adjustViewOfs();
	
		return retVal;
	}
	
	//____ insertCharAtCaret() ___________________________________________________
	
	bool LineEditor::insertCharAtCaret( uint16_t c )
	{
		if( !_isEditable() )
			return false;
	
		if( !m_state.isFocused() )
			if( !grabFocus() )
				return false;				// Couldn't get input focus...
	
		if( !m_text.putChar( c ) )
			return false;
	
		Base::msgRouter()->post( new TextEditMsg(text.ptr(),false) );
	
		_adjustViewOfs();
		return true;
	}
	
	//____ preferredSize() __________________________________________________________
	
	Size LineEditor::preferredSize() const
	{
		TextAttr attr;
		m_text.getBaseAttr( attr );
		int width = TextTool::lineWidth( attr, "MMMMMMMMMM" );		// Default line editor should fit 10 letter M in textitem
		Size contentSize( m_text.height(), width );
		
		if( m_pSkin )
			return m_pSkin->sizeForContent( contentSize );
		else
			return contentSize;
	}
	
	//____ _cloneContent() _______________________________________________________
	
	void LineEditor::_cloneContent( const Widget * _pOrg )
	{
		Widget::_cloneContent( _pOrg );
		
		LineEditor * pOrg = (LineEditor*) _pOrg;
	
		m_text			= pOrg->m_text;
		m_bPasswordMode = pOrg->m_bPasswordMode;
		m_pwGlyph		= pOrg->m_pwGlyph;
	}
	
	//____ _render() _____________________________________________________________
	
	void LineEditor::_render( GfxDevice * pDevice, const Rect& _canvas, const Rect& _window, const Rect& _clip )
	{
		Widget::_render(pDevice,_canvas,_window,_clip);
	
		LegacyTextItem * pText = &m_text;
	
	//TODO: Get password mode working again!
	/*
		if( m_bPasswordMode )
		{
			int nChars = m_text.length();
			uint16_t * pContent = new uint16_t[nChars+1];
			for( int i = 0 ; i < nChars ; i++ )
				pContent[i] = m_pwGlyph;
			pContent[nChars] = 0;
			pText = new LegacyTextItem( pContent );
			delete [] pContent;
	
			pText->setWrap(false);
			pText->setAutoEllipsis(false);
			pText->setAlignment(m_text.alignment());
			pText->setProperties(m_text.properties());
			pText->setSelectionProperties(m_text.selectionProperties());
			pText->setState(m_text.state());
	
			pText->setEditMode(m_text.editMode());
			pText->showCaret();
			pText->gotoSoftPos( m_text.line(), m_text.column() );
			pText->incTime( m_text.time() );
	
			int sl, sc, el, ec;
			if( m_text.getSelection(sl, sc, el, ec) )
				pText->selectText(sl, sc, el, ec);
		}
	*/
	
		Rect canvas;
		if( m_pSkin )
			canvas = m_pSkin->sizeForContent(_canvas);
		else
			canvas = _canvas;
	
		Rect	textClip( _clip, canvas );
	
		canvas.x -= m_viewOfs;
		canvas.w += m_viewOfs;
	
		if( m_state.isFocused() && _isEditable() )
			pText->showCaret();
		else
			pText->hideCaret();
	
		pDevice->printText( textClip, pText, canvas );
	
		if( pText != &m_text )
			delete pText;
	}
	
	//____ _receive() ______________________________________________________________
	
	void LineEditor::_receive( const Msg_p& pMsg )
	{
		Widget::_receive(pMsg);
	
		MsgRouter_p	pHandler = Base::msgRouter();
		MsgType event = pMsg->type();
	
		if( event == MsgType::Tick )
		{
			if( _isSelectable() && m_state.isFocused() )
			{
				m_text.incTime( TickMsg::cast(pMsg)->timediff() );
				_requestRender();					//TODO: Should only render the cursor and selection!
			}
			return;
		}
	
		if( (event == MsgType::MousePress || event == MsgType::MouseDrag) && MouseButtonMsg::cast(pMsg)->button() == MouseButton::Left )
		{
			MouseButtonMsg_p pButtonMsg = MouseButtonMsg::cast(pMsg);
			
			if( !m_state.isFocused() )
				grabFocus();
	
			if( m_state.isFocused() )
			{
				if( _isSelectable() && (pButtonMsg->modKeys() & MODKEY_SHIFT) )
				{
					m_text.setSelectionMode(true);
				}
	
				Coord ofs = pButtonMsg->pointerPos() - globalPos();
				int x = ofs.x + m_viewOfs;
				int y = 0;
	
				if( m_bPasswordMode )
				{
					TextAttr	attr;
					m_text.getBaseAttr( attr );
	
					Pen	pen;
					pen.setAttributes( attr );
					pen.setChar(m_pwGlyph);
					pen.advancePos();
	
					int spacing = pen.getPosX();
					int height = pen.getLineSpacing();
	
					int line = y/height;
					int col = (x+spacing/2)/spacing;
					if(col < 0)
					{
						col = 0;
						line = 0;
					}
					m_text.gotoSoftPos(line,col);
				}
				else
				{
					m_text.cursorGotoCoord( Coord(x, 0), Rect(0,0,1000000,1000000) );
				}
	
				if(_isSelectable() && event == MsgType::MousePress && !(pButtonMsg->modKeys() & MODKEY_SHIFT))
				{
					m_text.clearSelection();
					m_text.setSelectionMode(true);
				}
			}
			_adjustViewOfs();
		}
	
		if( event == MsgType::MouseRelease )
		{
			if( m_state.isFocused() && MouseButtonMsg::cast(pMsg)->button() == MouseButton::Left )
				m_text.setSelectionMode(false);
		}		
	
		if( event == MsgType::TextInput )
		{
			String str = TextInputMsg::cast(pMsg)->text();
	
			if( _isEditable() && m_state.isFocused() )
			{
				if(m_text.hasSelection())
					m_text.delSelection();
				m_text.setSelectionMode(false);

				bool bModified = false;
				for( int i = 0 ; i < str.length() ; i++ )
				{
					unsigned short ch = str.chars()[i].getGlyph();
					
					if( ch >= 32 && ch != 127 )
					{
						if( m_text.putChar( ch ) )
							bModified = true;
					}
				}

				if( bModified )
				{
					if( pHandler )
						pHandler->post( new TextEditMsg(text.ptr(),false) );

					_adjustViewOfs();
				}
	
			}
		}
	
		if( event == MsgType::KeyRelease && m_state.isFocused() )
		{
			Key key = KeyMsg::cast(pMsg)->translatedKeyCode();
			switch( key )
			{
				case Key::Shift:
					if(!Base::inputHandler()->isButtonPressed(MouseButton::Left))
						m_text.setSelectionMode(false);
				break;
			}
		}
	
		if( (event == MsgType::KeyPress || event == MsgType::KeyRepeat) && _isEditable() && m_state.isFocused() )
		{
			KeyMsg_p pKeyMsg = KeyMsg::cast(pMsg);
			Key key = pKeyMsg->translatedKeyCode();
			switch( key )
			{
				case Key::Left:
					if( pKeyMsg->modKeys() & MODKEY_SHIFT )
						m_text.setSelectionMode(true);
	
					if( pKeyMsg->modKeys() & MODKEY_CTRL )
					{
						if( m_bPasswordMode )
							m_text.goBol();
						else
							m_text.gotoPrevWord();
					}
					else
					{
						m_text.goLeft();
					}
					break;
				case Key::Right:
					if( pKeyMsg->modKeys() & MODKEY_SHIFT )
						m_text.setSelectionMode(true);
	
					if( pKeyMsg->modKeys() & MODKEY_CTRL )
					{
						if( m_bPasswordMode )
							m_text.goEol();
						else
							m_text.gotoNextWord();
					}
					else
					{
						m_text.goRight();
					}
					break;
	
				case Key::Backspace:
				{
					if(m_text.hasSelection())
						m_text.delSelection();
					else if( (pKeyMsg->modKeys() & MODKEY_CTRL) && !m_bPasswordMode)
						m_text.delPrevWord();
					else
						m_text.delPrevChar();
	
					if( pHandler )
						pHandler->post( new TextEditMsg(text.ptr(),false) );
					break;
				}
	
				case Key::Delete:
				{
					if(m_text.hasSelection())
						m_text.delSelection();
					else if( (pKeyMsg->modKeys() & MODKEY_CTRL) && !m_bPasswordMode)
						m_text.delNextWord();
					else
						m_text.delNextChar();
	
					if( pHandler )
						pHandler->post( new TextEditMsg(text.ptr(),false) );
					break;
				}
	
				case Key::Home:
	
					/*
					 *	I am not sure if this is the proper way to this, but in my opinion, the default
					 *	"actions" has to be separated from any modifier key action combination
					 */
					switch( pKeyMsg->modKeys() )
					{
	
					case MODKEY_CTRL:
						break;
	
					default: // no modifier key was pressed
						if(pKeyMsg->modKeys() & MODKEY_SHIFT )
							m_text.setSelectionMode(true);
	
						m_text.goBol();
						break;
					}
	
					break;
	
				case Key::End:
	
					/*
				 	 *	I am not sure if this is the proper way to this, but in my opinion, the default
			 		 *	"actions" has to be separated from any modifier key action combination
					 */
					switch( pKeyMsg->modKeys() )
					{
	
					case MODKEY_CTRL:
						break;
	
					default: // no modifier key was pressed
						if( pKeyMsg->modKeys() & MODKEY_SHIFT )
							m_text.setSelectionMode(true);
	
						m_text.goEol();
						break;
					}
	
					break;
	
				default:
					break;
			}
			_adjustViewOfs();
		}
	
		// Swallow message depending on rules.
	
		if( pMsg->isMouseButtreceive() )
		{
			if( MouseButtonMsg::cast(pMsg)->button() == MouseButton::Left )
				pMsg->swallow();
		}
		else if( pMsg->isKeyMsg() )
		{
			Key key = KeyMsg::cast(pMsg)->translatedKeyCode();
			if( KeyMsg::cast(pMsg)->isMovementKey() == true ||
				key == Key::Delete || key == Key::Backspace )
					pMsg->swallow();
			
			//TODO: Would be good if we didn't forward any character-creating keys either...
		}
	}
	
	
	//____ _adjustViewOfs() ________________________________________________________
	
	void LineEditor::_adjustViewOfs()
	{
		// Possibly move viewOfs so that:
		//	1 Caret remains inside view.
		//  2 At least one character is displayed before the cursor
		//  3 At least one character is displayed after the cursor (if there is one).
	
		if( m_state.isFocused() && m_text.properties() && m_text.properties()->font() )
		{
			Caret_p pCaret = TextTool::getCaret( &m_text );
			if( !pCaret )
				return;
	
			int cursCol	= m_text.column();
	
			TextAttr	attr;
			m_text.getBaseAttr( attr );
	
			Pen	pen;
			pen.setAttributes( attr );
			pen.setChar(m_pwGlyph);
			pen.advancePos();
	
			int pwAdvance	= pen.getPosX();
			int cursAdvance	= pCaret->advance(m_text.cursorMode() );
			int cursBearing	= pCaret->bearingX(m_text.cursorMode() );
			int cursWidth	= pCaret->width(m_text.cursorMode() );
	
			int cursOfs;		// Caret offset from beginning of line in pixels.
			int maxOfs;			// Max allowed view offset in pixels.
			int minOfs;			// Min allowed view offset in pixels.
	
			int geoWidth = size().w;
			if( m_pSkin )
				geoWidth -= m_pSkin->contentPadding().w;
			int	lineWidth = m_text.getSoftLineWidth( 0 ) + cursBearing+cursWidth;
	
			// Calculate cursOfs
	
			if( m_bPasswordMode )
				cursOfs = cursCol * pwAdvance;
			else
				cursOfs	= m_text.getSoftLineWidthPart( 0, 0, cursCol );
	
			// Calculate maxOfs
	
			if( cursCol > 0 )
			{
				if( m_bPasswordMode )
					maxOfs = (cursCol-1) * pwAdvance;
				else
					maxOfs = m_text.getSoftLineWidthPart( 0, 0, cursCol-1 );
	
				if( lineWidth < maxOfs + geoWidth )
					maxOfs = wg::max( lineWidth - geoWidth, 0 );
			}
			else
				maxOfs = cursOfs;
	
	
			// Calculate minOfs
	
			if( cursCol < m_text.getLine(0)->nChars )
			{
				if( m_bPasswordMode )
					minOfs = (cursCol+1) * pwAdvance + cursAdvance - geoWidth;
				else
					minOfs = m_text.getSoftLineWidthPart( 0, 0, cursCol+1 ) + cursAdvance - geoWidth;	// Not 100% right, cursor might affect linewidth different from its own width.
			}
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
	
	//____ _setState() ______________________________________________________
	
	void LineEditor::_setState( State state )
	{
		State oldState = m_state;
		
		Widget::_setState(state);
		
		m_text.setState(state);
		_requestRender();				//TODO: Only request render if text appearance has changed.
	
		if( state.isFocused() && !oldState.isFocused() )
		{
			if( _isEditable() )
			{
				m_tickRouteId = Base::msgRouter()->addRoute( MsgType::Tick, this );
				if( m_bResetCaretOnFocus )
					m_text.goEol();
				_requestRender(); // render with cursor on
			}
		}
	
		if( !state.isFocused() && oldState.isFocused() )
		{
			if( _isSelectable() )
			{
				m_text.clearSelection();
				m_text.setSelectionMode(false);
				m_bResetCaretOnFocus = false;
			}
	
			if( _isEditable() || m_viewOfs != 0 )
			{
				Base::msgRouter()->deleteRoute( m_tickRouteId );
				m_tickRouteId = 0;
				Base::msgRouter()->post( new TextEditMsg(text.ptr(),true) );
	
				m_viewOfs = 0;
				_requestRender();
			}
		}
	}
	
	//____ _setSkin() _______________________________________________________
	
	void LineEditor::_setSkin( const Skin_p& pSkin )
	{
		Widget::_setSkin(pSkin);
	}
	
	
	//____ _setSize() ____________________________________________________________
	
	void LineEditor::_setSize( const Size& size )
	{
		Widget::_setSize(_size);

		_adjustViewOfs();
		_requestRender();
	}
	
	//____ _requestRender() _________________________________________________________
	
	void LineEditor::_requestRender( Item * pItem )
	{
		_requestRender();
	}

	void LineEditor::_requestRender( Item * pItem, const Rect& rect )
	{
		_requestRender();
	}
	
	//____ _requestResize() ________________________________________________________
	
	void LineEditor::_requestResize( Item * pItem )
	{
		m_bResetCaretOnFocus = true;
		_requestResize();
		_requestRender();
		_adjustViewOfs();
	}

} // namespace wg
