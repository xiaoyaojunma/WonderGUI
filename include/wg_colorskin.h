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
#ifndef WG_COLORSKIN_DOT_H
#define WG_COLORSKIN_DOT_H

#ifndef WG_SKIN_DOT_H
#	include <wg_skin.h>
#endif

#ifndef WG_COLOR_DOT_H
#	include <wg_color.h>
#endif

class WgColorSkin;

typedef	WgSmartChildPtr<WgColorSkin,WgSkinPtr>	WgColorSkinPtr;


class WgColorSkin : public WgSkin
{
public:
	static WgColorSkinPtr Create( WgColor col );
	
	~WgColorSkin() {};
	
	void Render( WgGfxDevice * pDevice, const WgRect& _canvas, const WgRect& _clip );
	bool IsOpaque() const;
	
private:
	WgColorSkin( WgColor col );

	WgColor		m_color;
	
};

#endif //WG_COLORSKIN_DOT_H