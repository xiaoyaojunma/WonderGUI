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

#ifndef	WG_MODIFTEXTFIELD_DOT_H
#define WG_MODIFTEXTFIELD_DOT_H

#ifndef WG_TEXTFIELD_DOT_H
#	include <wg_textfield.h>
#endif


//____ WgModifTextHolder _______________________________________________________

class WgModifTextHolder : public WgTextHolder
{
};


class WgModifTextField;
typedef	WgCompStrongPtr<WgModifTextField,WgTextFieldPtr>	WgModifTextFieldPtr;
typedef	WgCompWeakPtr<WgModifTextField,WgTextFieldWeakPtr>	WgModifTextFieldWeakPtr;

//____ WgModifTextField ________________________________________________________

class WgModifTextField : public WgTextField
{
friend class WgModifTextHolder;
public:

	virtual bool				IsInstanceOf( const char * pClassName ) const;
	virtual const char *		ClassName( void ) const;
	static const char			CLASSNAME[];
	static WgModifTextFieldPtr		Cast( const WgComponentPtr& pComponent );

	WgModifTextFieldPtr			Ptr() { return WgModifTextFieldPtr(this); }

	void			_setHolder( WgModifTextHolder * pHolder ) { m_pHolder = pHolder; }
	
};

#endif //WG_MODIFTEXTFIELD_DOT_H