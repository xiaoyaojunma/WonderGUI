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

namespace wg 
{
	
	
	class UniformSizeBroker;
	typedef	StrongPtr<UniformSizeBroker,SizeBroker_p>		UniformSizeBroker_p;
	typedef	WeakPtr<UniformSizeBroker,SizeBroker_wp>	UniformSizeBroker_wp;
	
	class UniformSizeBroker : public SizeBroker
	{
	public:
		static UniformSizeBroker_p	create() { return UniformSizeBroker_p(new UniformSizeBroker()); }
	
		bool				isInstanceOf( const char * pClassName ) const;
		const char *		className( void ) const;
		static const char	CLASSNAME[];
		static UniformSizeBroker_p	cast( const Object_p& pObject );
	    
		int setItemLengths( SizeBrokerItem * pItems, int nItems, int totalLength ) const;
		int setPreferredLengths( SizeBrokerItem * pItems, int nItems ) const;
		bool mayAlterPreferredLengths() const;
	
	protected:
	    UniformSizeBroker() {};
	    virtual ~UniformSizeBroker() {};
	
		int     _findLongestUnified( SizeBrokerItem * pItems, int nItems ) const;
	    void    _setOutputs( SizeBrokerItem * pItems, int nItems, int value ) const;
	};
	
	
	

} // namespace wg
#endif //WG_UNIFORM_SIZEBROKER_DOT_H
