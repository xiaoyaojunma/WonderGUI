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
#ifndef WG_ROOTPANEL_DOT_H
#define WG_ROOTPANEL_DOT_H

#ifndef WG_PANEL_DOT_H
#	include <wg_panel.h>
#endif

#ifndef WG_GEO_DOT_H
#	include <wg_geo.h>
#endif

#ifndef WG_PATCHES_DOT_H
#	include <wg_patches.h>
#endif

class WgGfxDevice;
class WgWidget;

class WgRootPanel : public WgWidgetHolder
{
public:
	WgRootPanel();
	WgRootPanel( WgGfxDevice * pGfxDevice );
	~WgRootPanel();

	bool					SetGfxDevice( WgGfxDevice * pDevice );
	inline WgGfxDevice * 	GfxDevice() const { return m_pGfxDevice; };

	inline WgEventHandler *	EventHandler() const { return m_pEventHandler; }

	bool					SetGeo( const WgRect& geo );
	WgRect					Geo() const;

	inline WgWidget *		Child() const { return m_hook.Widget(); }
	bool					SetChild( WgWidget * pWidget );
	inline void				DeleteChild() { SetChild(0); }
	WgWidget * 				ReleaseChild();

	// Inherited from WgWidgetHolder

	bool					DeleteChild( WgWidget * pWidget );
	WgWidget *				ReleaseChild( WgWidget * pWidget );

	bool					DeleteAllChildren();
	bool					ReleaseAllChildren();

	bool					IsWidget() const { return false; }
	bool					IsPanel() const { return false; }
	bool					IsRoot() const { return true; }

	WgWidget *				CastToWidget() { return 0; }
	const WgWidget *			CastToWidget() const { return 0; }
	WgPanel *		CastToPanel() { return 0; }
	const WgPanel *CastToPanel() const { return 0; }
	WgRootPanel *				CastToRoot() { return this; }
	const WgRootPanel *			CastToRoot() const { return this; }


	inline int				NbDirtyRects() const { return m_dirtyPatches.Size(); }
	inline const WgRect*	FirstDirtyRect() const { return m_dirtyPatches.IsEmpty() ? 0 : m_dirtyPatches.Begin(); }

	inline int				NbUpdatedRects() const { return m_updatedPatches.Size(); }
	inline const WgRect*	FirstUpdatedRect() const { return m_updatedPatches.IsEmpty() ? 0 : m_updatedPatches.Begin(); }


	bool	Render();
	bool	Render( const WgRect& clip );

	bool	BeginRender();
	bool	RenderSection( const WgRect& clip, int layer = 0xFF );
	bool	EndRender();

	inline void	AddDirtyPatch( const WgRect& rect ) { m_dirtyPatches.Add( rect ); }

	WgWidget *	FindWidget( const WgCoord& ofs, WgSearchMode mode );

protected:
	class Hook : public WgHook
	{
		friend class WgRootPanel;
	public:
		Hook() : m_pRoot(0) {};				// So we can make them members and then make placement new...
		~Hook();

		const char *Type( void ) const;
		static const char * ClassType();

		WgCoord			Pos() const;
		WgSize			Size() const;
		WgRect			Geo() const;
		WgCoord			ScreenPos() const;
		WgRect			ScreenGeo() const;

		bool			SetVisible( bool bVisible );
		WgRootPanel*			Root() const;

	protected:

		void			_requestRender();
		void			_requestRender( const WgRect& rect );
		void			_requestResize();

		WgHook *		_prevHook() const;
		WgHook *		_nextHook() const;
		WgWidgetHolder * _parent() const;

		WgRootPanel *		m_pRoot;
	};


	WgHook*				_firstHook() const { return m_hook.Widget()? const_cast<Hook*>(&m_hook):0; }
	WgHook*				_lastHook() const { return m_hook.Widget()? const_cast<Hook*>(&m_hook):0; }

	bool 				_focusRequested( WgHook * pBranch, WgWidget * pWidgetRequesting );
	bool 				_focusReleased( WgHook * pBranch, WgWidget * pWidgetReleasing );

	WgModalPanel *		_getModalPanel() const { return 0; }
	WgMenuPanel*	_getMenuPanel() const { return 0; }

	WgPatches			m_dirtyPatches;		// Dirty patches that needs to be rendered.
	WgPatches			m_updatedPatches;	// Patches that were updated in last rendering session.

	WgGfxDevice *		m_pGfxDevice;
	WgEventHandler *	m_pEventHandler;
	Hook				m_hook;
	WgRect				m_geo;
	bool				m_bHasGeo;
};



#endif //WG_ROOTPANEL_DOT_H