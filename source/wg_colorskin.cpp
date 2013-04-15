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

#include <wg_colorskin.h>
#include <wg_gfxdevice.h>
#include <wg_geo.h>



WgColorSkinPtr WgColorSkin::Create( WgColor col )
{
	return WgColorSkinPtr(new WgColorSkin(col));
}


WgColorSkin::WgColorSkin( WgColor col )
{
	m_color = col;
}

	
void WgColorSkin::Render( WgGfxDevice * pDevice, const WgRect& _canvas, const WgRect& _clip )
{
	pDevice->Fill( _clip, m_color );
}
	
bool WgColorSkin::IsOpaque() const
{
	return m_color.a == 255 ? true : false;
}
