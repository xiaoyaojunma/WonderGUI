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
#ifndef WG_GIZMO_DUMMY_DOT_H
#define WG_GIZMO_DUMMY_DOT_H


#ifndef WG_GIZMO_DOT_H
#	include <wg_gizmo.h>
#endif


//____ WgGizmoDummy ____________________________________________________________

class WgGizmoDummy : public WgGizmo
{
public:
	WgGizmoDummy();
	virtual ~WgGizmoDummy();

	virtual const char *Type( void ) const;
	static const char * GetMyType();

protected:

	void	OnCloneContent( const WgGizmo * _pOrg );
	void	OnRender( WgGfxDevice * pDevice, const WgRect& _window, const WgRect& _clip, Uint8 _layer );
	bool	OnMarkTest( const WgCord& ofs );

};


#endif //WG_GIZMO_DUMMY_DOT_H