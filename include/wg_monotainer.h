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

#ifndef WG_MONOTAINER_DOT_H
#define WG_MONOTAINER_DOT_H

#ifndef WG_GIZMO_CONTAINER_DOT_H
#	include <wg_gizmo_container.h>
#endif


class WgMonotainer : public WgGizmo, public WgGizmoContainer
{
public:
	WgMonotainer();
	~WgMonotainer();

	WgGizmoHook *	SetGizmo( WgGizmoContainer * pGizmo );
	WgGizmo *		Gizmo();
	bool			DeleteGizmo();
	WgGizmo *		ReleaseGizmo();

	bool			DeleteGizmo( WgGizmo * pGizmo );
	WgGizmo *		ReleaseGizmo( WgGizmo * pGizmo );

	bool			DeleteAllGizmos();
	bool			ReleaseAllGizmos();


	// Overloaded from WgGizmo

	int				HeightForWidth( int width ) const;
	int				WidthForHeight( int height ) const;

	WgSize			MinSize() const;
	WgSize			BestSize() const;
	WgSize			MaxSize() const;

	bool			IsView() const { return false; }
	bool			IsContainer() const { return true; }

	WgGizmoContainer * CastToContainer() { return this; }
	const WgGizmoContainer * CastToContainer() const { return this; }

	WgGizmo *		CastToGizmo() { return this; }

	// Overloaded from container

	WgGizmo *		FindGizmo( const WgCord& ofs, WgSearchMode mode );

private:

	class Hook : public WgGizmoHook
	{
		friend class WgMonotainer;

	public:
		// Standard Hook methods

		inline WgCord		Pos() const { return m_pParent->Pos(); }
		inline WgSize		Size() const { 	return m_pParent->Size(); }
		inline WgRect		Geo() const { return m_pParent->Geo(); }

		inline WgCord		ScreenPos() const { return m_pParent->ScreenPos(); }
		inline WgRect		ScreenGeo() const { return m_pParent->ScreenGeo(); }

		inline WgGizmoContainer* Parent() const { return m_pParent; }

		inline WgWidget*	GetRoot() { return 0; }			// Should in the future not return a widget, but a gizmo.

	protected:
		Hook( WgMonotainer * pParent ) : m_pParent(pParent) {}

		inline void		RequestRender() { return m_pParent->RequestRender(); }
		inline void		RequestRender( const WgRect& rect ) { return m_pParent->RequestRender(rect); }
		inline void		RequestResize() { return m_pParent->RequestResize(); }

		WgGizmoHook *	_prevHook() const { return 0; }
		WgGizmoHook *	_nextHook() const { return 0; }
		WgGizmoContainer * _parent() const { return m_pParent; }

		WgMonotainer * 	m_pParent;

	};



	void			_onCollectRects( WgRectChain& rects, const WgRect& geo, const WgRect& clip );
	void			_onMaskRects( WgRectChain& rects, const WgRect& geo, const WgRect& clip );
	void			_onCloneContent( const WgGizmo * _pOrg );
	void			_onNewSize( const WgSize& size );

	void			_onEnable();
	void			_onDisable();

	void			_castDirtyRect( const WgRect& geo, const WgRect& clip, WgRectLink * pDirtIn, WgRectChain* pDirtOut );
	void			_renderDirtyRects( WgGfxDevice * pDevice, const WgRect& _canvas, const WgRect& _window, Uint8 _layer );
	void			_clearDirtyRects();


	WgGizmoHook*	_firstHook() const;
	WgGizmoHook*	_lastHook() const;

	Hook			m_hook;

};

#endif //WG_MONOTAINER_DOT_H