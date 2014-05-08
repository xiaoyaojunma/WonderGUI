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

#ifndef	WG_CHECKBOX_DOT_H
#define	WG_CHECKBOX_DOT_H

#ifndef	WG_WIDGET_DOT_H
#	include <wg_widget.h>
#endif

#ifndef WG_TEXTFIELD_DOT_H
#	include <wg_textfield.h>
#endif

#ifndef WG_ICONFIELD_DOT_H
#	include <wg_iconfield.h>
#endif

class	WgSurface;

class WgCheckBox;
typedef	WgSmartPtr<WgCheckBox,WgWidgetPtr>		WgCheckBoxPtr;
typedef	WgWeakPtr<WgCheckBox,WgWidgetWeakPtr>	WgCheckBoxWeakPtr;


class	WgCheckBox : public WgWidget, public WgIconHolder, public WgTextHolder
{
public:
	static WgCheckBoxPtr	Create() { return WgCheckBoxPtr(new WgCheckBox()); }

	bool					IsInstanceOf( const char * pClassName ) const;
	const char *			ClassName( void ) const;
	static const char		CLASSNAME[];
	static WgCheckBoxPtr	Cast( const WgObjectPtr& pObject );

	enum ClickArea
	{
		DEFAULT,		// Full geometry of icon (no alpha test) + text + area between + alpha test on background.
		ALPHA,			// Alpha test on background and icon.
		GEO,			// Full geometry of Widget is clickable.
		ICON,			// Only the icon (alpha test) is clickable.
		TEXT			// Only the text is clickable.
	};

	inline WgIModifTextPtr	Label() { return WgIModifTextPtr(this,&m_text); } 
	inline WgIIconPtr		Icon() { return WgIIconPtr(this,&m_icon); }

	void			SetClickArea( ClickArea clickArea );
	ClickArea		GetClickArea() const { return m_clickArea; }

	inline bool		IsSelected() { return m_state.IsSelected(); };
	virtual bool	SetSelected( bool bSelected );

	void			SetFlipOnRelease( bool bFlipOnRelease );
	inline bool		FlipOnRelease() { return m_bFlipOnRelease; }

	WgSize			PreferredSize() const;
	bool			IsAutoEllipsisDefault() const { return false; };
	

protected:
	WgCheckBox();
	virtual ~WgCheckBox();
	virtual WgWidget* _newOfMyType() const { return new WgCheckBox(); };

	void	_onCloneContent( const WgWidget * _pOrg );
	void	_onRender( WgGfxDevice * pDevice, const WgRect& _canvas, const WgRect& _window, const WgRect& _clip );
	void	_onEvent( const WgEventPtr& pEvent, WgEventHandler * pHandler );
	void	_onRefresh();
	void	_onNewSize( const WgSize& size );
	bool	_onAlphaTest( const WgCoord& ofs );
	void	_onStateChanged( WgState oldState, WgState newState );
	void	_onSkinChanged( const WgSkinPtr& pOldSkin, const WgSkinPtr& pNewSkin );
	
	virtual void	_iconModified( WgIconField * pIcon );
	virtual void	_textModified( WgTextField * pText );

private:

	void	_refreshTextArea();
	bool	_markTestTextArea( int _x, int _y );
	
	bool			m_bPressed;						// Set when mouse is pressed and over.
	bool			m_bReturnPressed;
	bool			m_bFlipOnRelease;				// Set if we want to flip checkbox on press (default), not click.

	WgTextField		m_text;
	WgIconField		m_icon;

	ClickArea		m_clickArea;
};


#endif //	WG_CHECKBOX_DOT_H
