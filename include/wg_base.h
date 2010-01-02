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

#ifndef	WG_BASE_DOT_H
#define	WG_BASE_DOT_H

/*
#ifndef WG_TEXTMGR_DOT_H
#	include <wg_textmgr.h>
#endif
*/

class WgFont;


class WgBase
{
public:
	static void Init();
	static void Exit();


//	static void SetDefaultTextManager( const WgTextMgrPtr& pManager );
//	static const WgTextMgrPtr& GetDefaultTextManager();

	static void SetDefaultFont( WgFont * pFont );
	static WgFont * GetDefaultFont();

private:
	static WgFont *		s_pDefFont;
//	WgTextMgrPtr	m_pDefTextMgr;


};


#endif //WG_BASE_DOT_H
