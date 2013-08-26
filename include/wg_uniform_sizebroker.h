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
#ifndef	WG_UNIFORM_SIZEBROKER_DOT_H
#define	WG_UNIFORM_SIZEBROKER_DOT_H

#ifndef WG_SIZEBROKER_DOT_H
#	include <wg_sizebroker.h>
#endif


class WgUniformSizeBroker;
typedef	WgSmartPtr<WgUniformSizeBroker,WgSizeBrokerPtr>		WgUniformSizeBrokerPtr;
typedef	WgWeakPtr<WgUniformSizeBroker,WgSizeBrokerWeakPtr>	WgUniformSizeBrokerWeakPtr;

class WgUniformSizeBroker : public WgSizeBroker
{
public:
	static WgUniformSizeBrokerPtr	Create() { return WgUniformSizeBrokerPtr(new WgUniformSizeBroker()); }

	bool				IsInstanceOf( const char * pClassName ) const;
	const char *		ClassName( void ) const;
	static const char	CLASSNAME[];
	static WgUniformSizeBrokerPtr	Cast( const WgObjectPtr& pObject );
    
	int SetItemLengths( WgSizeBrokerItem * pItems, int nItems, int totalLength ) const;
	int SetPreferredLengths( WgSizeBrokerItem * pItems, int nItems ) const;
	bool MayAlterPreferredLengths() const;

protected:
    WgUniformSizeBroker() {};
    virtual ~WgUniformSizeBroker() {};

	int     _findLongestUnified( WgSizeBrokerItem * pItems, int nItems ) const;
    void    _setOutputs( WgSizeBrokerItem * pItems, int nItems, int value ) const;
};



#endif //WG_UNIFORM_SIZEBROKER_DOT_H