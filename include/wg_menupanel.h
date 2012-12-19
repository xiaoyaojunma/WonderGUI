/*=========================================================================

                         >>> WonderGUI <<<

  This file is part of Tord Jansson's WonderGUI Graphics Toolkit
  and copyright (c) Tord Jansson, Sweden [tord.jansson@gmail.com].

                            -----------

  The WonderGUI Graphics Toolkit is free software you can redistribute
  this file and/or modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation either
  version 2 of the License, or (at your option) any later version.

                            -----------

  The WonderGUI Graphics Toolkit is also available for use in commercial
  closed-source projects under a separate license. Interested parties
  should contact Tord Jansson [tord.jansson@gmail.com] for details.

=========================================================================*/

#ifndef WG_MENUPANEL_DOT_H
#define WG_MENUPANEL_DOT_H

#ifndef WG_PANEL_DOT_H
#	include <wg_panel.h>
#endif


class WgMenuPanel;

class WgMenuHook : public WgHook, protected WgLink
{
	friend class WgMenuPanel;
	friend class WgChain<WgMenuHook>;

public:

	const char *Type( void ) const;
	static const char * ClassType();

	// Standard Hook methods

	WgCoord			Pos() const { return m_geo.Pos(); }
	WgSize			Size() const { 	return m_geo.Size(); }
	WgRect			Geo() const { return m_geo; }

	WgCoord			ScreenPos() const;
	WgRect			ScreenGeo() const;

	WgMenuHook *	Prev() const { return _prev(); }
	WgMenuHook *	Next() const { return _next(); }

	WgPanel* Parent() const;

protected:
	// TODO: Constructor should in the future call SetHook() on Widget, once we are totally rid of widgets...

	PROTECTED_LINK_METHODS( WgMenuHook );

	WgMenuHook( WgMenuPanel * pParent, WgWidget * pOpener, const WgRect& launcherGeo, WgOrientation attachPoint, WgSize maxSize );

	void		_requestRender();
	void		_requestRender( const WgRect& rect );
	void		_requestResize();

	WgHook *	_prevHook() const;
	WgHook *	_nextHook() const;
	WgPanel * _parent() const;

	bool		_updateGeo();


	WgMenuPanel * m_pParent;

	WgRect			m_geo;				// Widgets geo relative parent
	WgRect			m_launcherGeo;		// Launcher geo relative sibling or parent.
	WgOrientation	m_attachPoint;
	WgSize			m_maxSize;
	WgWidgetWeakPtr	m_pOpener;			// Widget that opened this menu.
	WgWidgetWeakPtr	m_pKeyFocus;		// Pointer at widget that held focus when this menu was ontop.
};



class WgMenuPanel : public WgPanel
{
	friend class BaseHook;
	friend class WgMenuHook;

public:
	WgMenuPanel();
	~WgMenuPanel();

	virtual const char *Type( void ) const;
	static const char * GetClass();
	virtual WgWidget * NewOfMyType() const { return new WgMenuPanel(); };


	WgHook *		SetBase( WgWidget * pWidget );
	WgWidget *		Base();
	bool			DeleteBase();
	WgWidget *		ReleaseBase();

	WgMenuHook *	OpenMenu( WgWidget * pMenu, WgWidget * pOpener, const WgRect& launcherGeo, WgOrientation attachPoint = WG_NORTHEAST, WgSize maxSize = WgSize(INT_MAX,INT_MAX) );

	bool			CloseMenu( WgWidget * pMenu );
	bool			CloseAllMenus();

	WgMenuHook *	FirstMenu();
	WgMenuHook *	LastMenu();


	// Overloaded from WgWidget

	int				HeightForWidth( int width ) const;
	int				WidthForHeight( int height ) const;

	WgSize			DefaultSize() const;

	// Overloaded from WgPanel

	WgWidget *		FindWidget( const WgCoord& ofs, WgSearchMode mode );

	bool			DeleteChild( WgWidget * pWidget ) { return 0; }
	WgWidget *		ReleaseChild( WgWidget * pWidget ) { return 0; }

	bool			DeleteAllChildren() { return 0; }
	bool			ReleaseAllChildren() { return 0; }

private:

	class BaseHook : public WgHook
	{
		friend class WgMenuPanel;

	public:

		const char *Type( void ) const;
		static const char * ClassType();

		// Standard Hook methods

		WgCoord		Pos() const { return m_pParent->Pos(); }
		WgSize		Size() const { 	return m_pParent->Size(); }
		WgRect		Geo() const { return m_pParent->Geo(); }

		WgCoord		ScreenPos() const { return m_pParent->ScreenPos(); }
		WgRect		ScreenGeo() const { return m_pParent->ScreenGeo(); }

		WgMenuPanel* Parent() const { return m_pParent; }

	protected:
		BaseHook( WgMenuPanel * pParent ) : m_pParent(pParent) {}

		void		_requestRender();
		void		_requestRender( const WgRect& rect );
		void		_requestResize();

		WgHook *	_prevHook() const { return 0; }
		WgHook *	_nextHook() const { return m_pParent->FirstMenu(); }
		WgWidgetHolder * _parent() const { return m_pParent; }

		WgMenuPanel * 	m_pParent;
	};


	WgMenuPanel *	_getMenuPanel() const { return const_cast<WgMenuPanel*>(this); }

	void			_stealKeyboardFocus();
	void			_restoreKeyboardFocus();

	//

	void			_onCloneContent( const WgWidget * _pOrg );
	void			_onNewSize( const WgSize& size );
	void			_onEvent( const WgEvent::Event * pEvent, WgEventHandler * pHandler );

	void			_onRequestRender( const WgRect& rect, const WgMenuHook * pHook );	// rect is in our coordinate system.

	WgHook*			_firstHook() const;		// Fist Hook returned is the normal child, then follows the modal ones.
	WgHook*			_lastHook() const;		//

	WgHook *		_firstHookWithGeo( WgRect& geo ) const;
	WgHook *		_nextHookWithGeo( WgRect& geo, WgHook * pHook ) const;

	WgHook *		_lastHookWithGeo( WgRect& geo ) const;
	WgHook *		_prevHookWithGeo( WgRect& geo, WgHook * pHook ) const;

	BaseHook		m_baseHook;
	WgChain<WgMenuHook>		m_menuHooks;		// First menu lies at the bottom.

	WgSize			m_size;
	WgWidgetWeakPtr	m_pKeyFocus;		// Pointer at child that held focus before any menu was opened.


};

#endif //WG_MENUPANEL_DOT_H