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

#ifndef WG_GIZMO_MODAL_DOT_H
#define WG_GIZMO_MODAL_DOT_H

#ifndef WG_GIZMO_CONTAINER_DOT_H
#	include <wg_gizmo_container.h>
#endif


class WgGizmoModal;

class WgModalHook : public WgHook, protected WgLink
{
	friend class WgGizmoModal;
	friend class WgChain<WgModalHook>;

public:

	void	Top();								// Put us ontop of all our silbings.

	bool	SetGeo( const WgRect& geometry, WgOrientation origo = WG_NORTHWEST );
	bool	SetGeo( const WgCoord& ofs, WgOrientation origo = WG_NORTHWEST );

	bool	SetOfs( const WgCoord& ofs );
	bool	SetOfsX( int x );
	bool	SetOfsY( int y );

	bool	SetSize( WgSize sz );
	bool	SetWidth( int width );
	bool	SetHeight( int height );

	bool	Move( const WgCoord& ofs );
	bool	MoveX( int x );
	bool	MoveY( int y );


	// Standard Hook methods

	WgCoord			Pos() const { return m_realGeo.Pos(); }
	WgSize			Size() const { 	return m_realGeo.Size(); }
	WgRect			Geo() const { return m_realGeo; }

	WgCoord			ScreenPos() const;
	WgRect			ScreenGeo() const;

	WgModalHook *	Prev() const { return _prev(); }
	WgModalHook *	Next() const { return _next(); }

	WgGizmoContainer* Parent() const;

	WgWidget*	GetRoot();			// Should in the future not return a widget, but a gizmo.

protected:
	// TODO: Constructor should in the future call SetHook() on Gizmo, once we are totally rid of widgets...

	PROTECTED_LINK_METHODS( WgModalHook );

	WgModalHook( WgGizmoModal * pParent );

	bool		_refreshRealGeo();	// Return false if we couldn't get exactly the requested (floating) geometry.

	void		_requestRender();
	void		_requestRender( const WgRect& rect );
	void		_requestResize();

	WgHook *	_prevHook() const;
	WgHook *	_nextHook() const;
	WgGizmoContainer * _parent() const;


	WgGizmoModal * m_pParent;

	WgRect			m_realGeo;			// Gizmos geo relative parent

	WgOrientation	m_origo;
	WgRect			m_placementGeo;		// Gizmos geo relative anchor and hotspot. Setting width and height to 0 uses Gizmos DefaultSize() dynamically.
										// Setting just one of them to 0 uses Gizmos HeightForWidth() or WidthForHeight() dynamically.

	WgGizmoWeakPtr	m_pKeyFocus;		// Pointer at child that held focus when this modal was last on top.
};



class WgGizmoModal : public WgGizmo, public WgGizmoContainer
{
	friend class BaseHook;
	friend class WgModalHook;

public:
	WgGizmoModal();
	~WgGizmoModal();

	virtual const char *Type( void ) const;
	static const char * GetMyType();
	virtual WgGizmo * NewOfMyType() const { return new WgGizmoModal(); };
	

	WgHook *	SetBase( WgGizmo * pGizmo );
	WgGizmo *		Base();
	bool			DeleteBase();
	WgGizmo *		ReleaseBase();


	WgModalHook *	AddModal( WgGizmo * pGizmo, const WgRect& geometry, WgOrientation origo = WG_NORTHWEST );
	WgModalHook *	AddModal( WgGizmo * pGizmo, const WgCoord& pos, WgOrientation origo = WG_NORTHWEST ) { return AddModal( pGizmo, WgRect(pos,0,0), origo); }

	bool			DeleteAllModal();
	bool			ReleaseAllModal();

	bool			DeleteChild( WgGizmo * pGizmo );
	WgGizmo *		ReleaseChild( WgGizmo * pGizmo );

	bool			DeleteAllChildren();
	bool			ReleaseAllChildren();

	WgModalHook *	FirstModal();
	WgModalHook *	LastModal();


	// Overloaded from WgGizmo

	int				HeightForWidth( int width ) const;
	int				WidthForHeight( int height ) const;

	WgSize			DefaultSize() const;

	bool			IsView() const { return false; }
	bool			IsContainer() const { return true; }

	WgGizmoContainer * CastToContainer() { return this; }
	const WgGizmoContainer * CastToContainer() const { return this; }

	WgGizmo*		CastToGizmo() { return this; }


	// Overloaded from container

	WgGizmo *		FindGizmo( const WgCoord& ofs, WgSearchMode mode );

private:

	class BaseHook : public WgHook
	{
		friend class WgGizmoModal;

	public:
		// Standard Hook methods

		WgCoord		Pos() const { return m_pParent->Pos(); }
		WgSize		Size() const { 	return m_pParent->Size(); }
		WgRect		Geo() const { return m_pParent->Geo(); }

		WgCoord		ScreenPos() const { return m_pParent->ScreenPos(); }
		WgRect		ScreenGeo() const { return m_pParent->ScreenGeo(); }

		WgGizmoModal* Parent() const { return m_pParent; }

		WgWidget*	GetRoot() { return 0; }			// Should in the future not return a widget, but a gizmo.

	protected:
		BaseHook( WgGizmoModal * pParent ) : m_pParent(pParent) {}

		void		_requestRender();
		void		_requestRender( const WgRect& rect );
		void		_requestResize();

		WgHook *	_prevHook() const { return 0; }
		WgHook *	_nextHook() const { return m_pParent->FirstModal(); }
		WgGizmoParent * _parent() const { return m_pParent; }

		WgGizmoModal * 	m_pParent;
		WgGizmoWeakPtr	m_pKeyFocus;		// Pointer at child that held focus before any modal was shown.
	};

	void			_updateKeyboardFocus();

	// These are needed until WgGizmoContainer inherits from WgGizmo

	void			_renderPatches( WgGfxDevice * pDevice, const WgRect& _canvas, const WgRect& _window, WgPatches * _pPatches, Uint8 _layer )
									{ WgGizmoContainer::_renderPatches( pDevice, _canvas, _window, _pPatches, _layer ); }
	void			_onCollectPatches( WgPatches& container, const WgRect& geo, const WgRect& clip )
									{ WgGizmoContainer::_onCollectPatches(container, geo, clip); }
	void			_onMaskPatches( WgPatches& patches, const WgRect& geo, const WgRect& clip )
									{ WgGizmoContainer::_onMaskPatches(patches, geo, clip); }
	void			_onEnable() { WgGizmoContainer::_onEnable(); }
	void			_onDisable() { WgGizmoContainer::_onDisable(); }
	bool 			_onAlphaTest( const WgCoord& ofs ) { return WgGizmoContainer::_onAlphaTest(ofs); }

	//

	void			_onCloneContent( const WgGizmo * _pOrg );
	void			_onNewSize( const WgSize& size );
	void			_onAction( WgInput::UserAction action, int button_key, const WgActionDetails& info, const WgInput& inputObj );

	void			_onRequestRender( const WgRect& rect, const WgModalHook * pHook );	// rect is in our coordinate system.

	WgHook*	_firstHook() const;		// Fist Hook returned is the normal child, then follows the modal ones.
	WgHook*	_lastHook() const;		//

	WgHook *	_firstHookWithGeo( WgRect& geo ) const;
	WgHook *	_nextHookWithGeo( WgRect& geo, WgHook * pHook ) const;

	WgHook *	_lastHookWithGeo( WgRect& geo ) const;
	WgHook *	_prevHookWithGeo( WgRect& geo, WgHook * pHook ) const;

	BaseHook				m_baseHook;
	WgChain<WgModalHook>	m_modalHooks;		// First modal gizmo lies at the bottom.

	WgSize					m_size;

};

#endif //WG_GIZMO_MODAL_DOT_H
