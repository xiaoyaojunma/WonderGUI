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

#include <vector>
#include <wg_panel.h>

#include <wg_patches.h>

#ifndef WG_GFXDEVICE_DOT_H
#	include <wg_gfxdevice.h>
#endif

//____ Constructor _____________________________________________________________

WgPanel::WgPanel() : m_bFocusGroup(false), m_bRadioGroup(false), m_bTooltipGroup(false), m_maskOp(WG_MASKOP_RECURSE),
	m_bSiblingsOverlap(true)
{
}



//____ IsWidget() ______________________________________________________________

bool WgPanel::IsWidget() const
{
	return true;
}

//____ IsPanel() ______________________________________________________________

bool WgPanel::IsPanel() const
{
	return true;
}


//____ IsRoot() _______________________________________________________________

bool WgPanel::IsRoot() const
{
	return false;
}

//____ CastToWidget() _______________________________________________________

WgWidget * WgPanel::CastToWidget()
{
	return this;
}

const WgWidget * WgPanel::CastToWidget() const
{
	return this;
}


//____ CastToPanel() _______________________________________________________

WgPanel * WgPanel::CastToPanel()
{
	return this;
}

const WgPanel * WgPanel::CastToPanel() const
{
	return this;
}


//____ CastToRoot() ___________________________________________________________

WgRootPanel * WgPanel::CastToRoot()
{
	return 0;
}

const WgRootPanel * WgPanel::CastToRoot() const
{
	return 0;
}

//____ SetMaskOp() _____________________________________________________________

void WgPanel::SetMaskOp( WgMaskOp operation )
{
	if( operation != m_maskOp )
	{
		m_maskOp = operation;
		_requestRender();
	}
}

//____ FindWidget() ____________________________________________________________

WgWidget * WgPanel::FindWidget( const WgCoord& ofs, WgSearchMode mode )
{
	WgRect childGeo;
	WgHook * pHook = _lastHookWithGeo( childGeo );
	WgWidget * pResult = 0;

	while( pHook && !pResult )
	{
		if( pHook->IsVisible() && childGeo.Contains( ofs ) )
		{
			if( pHook->Widget()->IsPanel() )
			{
				pResult = pHook->Widget()->CastToPanel()->FindWidget( ofs - childGeo.Pos(), mode );
			}
			else
			{
				switch( mode )
				{
					case WG_SEARCH_ACTION_TARGET:
					case WG_SEARCH_MARKPOLICY:
						if( pHook->Widget()->MarkTest( ofs - childGeo.Pos() ) )
							pResult = pHook->Widget();
						break;
					case WG_SEARCH_GEOMETRY:
						pResult = pHook->Widget();
						break;
				}
			}
		}
		pHook = _prevHookWithGeo( childGeo, pHook );
	}

	// Return us if search mode is GEOMETRY

	if( !pResult && mode == WG_SEARCH_GEOMETRY )
		pResult = this;

	return pResult;
}

//____ _focusRequested() _______________________________________________________

bool WgPanel::_focusRequested( WgHook * pBranch, WgWidget * pWidgetRequesting )
{
	WgHook * p = Hook();
	if( p )
		return p->Parent()->_focusRequested( p, pWidgetRequesting );
	else
		return false;
}

//____ _focusReleased() ________________________________________________________

bool WgPanel::_focusReleased( WgHook * pBranch, WgWidget * pWidgetReleasing )
{
	WgHook * p = Hook();
	if( p )
		return p->Parent()->_focusReleased( p, pWidgetReleasing );
	else
		return false;
}


WgModalPanel *  WgPanel::_getModalPanel() const
{
	const WgWidgetHolder * p = Parent();

	if( p )
		return p->_getModalPanel();
	else
		return 0;
}

WgMenuPanel * WgPanel::_getMenuPanel() const
{
	const WgWidgetHolder * p = Parent();

	if( p )
		return p->_getMenuPanel();
	else
		return 0;
}



//____ _onEnable() _____________________________________________________________

void WgPanel::_onEnable()
{
	WgWidget * p = FirstWidget();
	while( p )
	{
		p->SetEnabled(true);
		p = p->NextSibling();
	}
}

//____ _onDisable() ____________________________________________________________

void WgPanel::_onDisable()
{
	WgWidget * p = FirstWidget();
	while( p )
	{
		p->SetEnabled(false);
		p = p->NextSibling();
	}
}

//____ _renderPatches() _____________________________________________________
// Default implementation for panel rendering patches.
class WidgetRenderContext
{
public:
	WidgetRenderContext() : pWidget(0) {}
	WidgetRenderContext( WgWidget * pWidget, const WgRect& geo ) : pWidget(pWidget), geo(geo) {}

	WgWidget *	pWidget;
	WgRect		geo;
	WgPatches	patches;
};

void WgPanel::_renderPatches( WgGfxDevice * pDevice, const WgRect& _canvas, const WgRect& _window, WgPatches * _pPatches, Uint8 _layer )
{

	// We start by eliminating dirt outside our geometry

	WgPatches 	patches( _pPatches->Size() );								// TODO: Optimize by pre-allocating?

	for( const WgRect * pRect = _pPatches->Begin() ; pRect != _pPatches->End() ; pRect++ )
	{
		if( _canvas.IntersectsWith( *pRect ) )
			patches.Push( WgRect(*pRect,_canvas) );
	}

	//

	WgRect	dirtBounds = patches.Union();

	if( m_bSiblingsOverlap )
	{

		// Create WidgetRenderContext's for siblings that might get dirty patches

		std::vector<WidgetRenderContext> renderList;

		WgRect childGeo;
		WgHook * p = _firstHookWithGeo( childGeo );
		while(p)
		{
			WgRect geo = childGeo + _canvas.Pos();

			if( p->IsVisible() && geo.IntersectsWith( dirtBounds ) )
				renderList.push_back( WidgetRenderContext(p->Widget(), geo ) );

			p = _nextHookWithGeo( childGeo, p );
		}

		// Go through WidgetRenderContexts in reverse order (topmost first), push and mask dirt

		for( int i = renderList.size()-1 ; i >= 0 ; i-- )
		{
			WidgetRenderContext * p = &renderList[i];

			p->patches.Push( &patches );

			p->pWidget->_onMaskPatches( patches, p->geo, p->geo, pDevice->GetBlendMode() );		//TODO: Need some optimizations here, grandchildren can be called repeatedly! Expensive!

			if( patches.IsEmpty() )
				break;
		}

		// Go through WidgetRenderContexts and render the patches

		for( int i = 0 ; i < (int) renderList.size() ; i++ )
		{
			WidgetRenderContext * p = &renderList[i];
			p->pWidget->_renderPatches( pDevice, p->geo, p->geo, &p->patches, _layer );
		}

	}
	else
	{
		WgRect childGeo;
		WgHook * p = _firstHookWithGeo( childGeo );

		while(p)
		{
			WgRect canvas = childGeo + _canvas.Pos();
			if( p->IsVisible() && canvas.IntersectsWith( dirtBounds ) )
				p->Widget()->_renderPatches( pDevice, canvas, canvas, &patches, _layer );
			p = _nextHookWithGeo( childGeo, p );
		}

	}
}


//____ _onAlphaTest() _________________________________________________________

bool WgPanel::_onAlphaTest( const WgCoord& ofs )
{
	return false;		// By default cointainers have nothing to display themselves.
}

//____ _onCloneContent() _______________________________________________________

void WgPanel::_onCloneContent( const WgPanel * _pOrg )
{
	m_bFocusGroup 		= _pOrg->m_bFocusGroup;
	m_bRadioGroup 		= _pOrg->m_bRadioGroup;
	m_bTooltipGroup 	= _pOrg->m_bTooltipGroup;
	m_bSiblingsOverlap 	= _pOrg->m_bSiblingsOverlap;
	m_maskOp 			= _pOrg->m_maskOp;
}

//____ _onCollectPatches() _______________________________________________________

void WgPanel::_onCollectPatches( WgPatches& container, const WgRect& geo, const WgRect& clip )
{
	WgRect childGeo;
	WgHook * p = _firstHookWithGeo( childGeo );

	while(p)
	{
		if( p->IsVisible() )
			p->Widget()->_onCollectPatches( container, childGeo + geo.Pos(), clip );
		p = _nextHookWithGeo( childGeo, p );
	}
}

//____ _onMaskPatches() __________________________________________________________

void WgPanel::_onMaskPatches( WgPatches& patches, const WgRect& geo, const WgRect& clip, WgBlendMode blendMode )
{
	switch( m_maskOp )
	{
		case WG_MASKOP_RECURSE:
		{
			WgRect childGeo;
			WgHook * p = _firstHookWithGeo( childGeo );

			while(p)
			{
				if( p->IsVisible() )
					p->Widget()->_onMaskPatches( patches, childGeo + geo.Pos(), clip, blendMode );
				p = _nextHookWithGeo( childGeo, p );
			}
			break;
		}
		case WG_MASKOP_SKIP:
			break;
		case WG_MASKOP_MASK:
			patches.Sub( WgRect(geo,clip) );
			break;
	}
}