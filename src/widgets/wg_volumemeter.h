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

#ifndef WG_VOLUMEMETER_DOT_H
#define WG_VOLUMEMETER_DOT_H

#ifndef WG_WIDGET_DOT_H
#	include <wg_widget.h>
#endif

#ifndef WG_COLOR_DOT_H
#	include <wg_color.h>
#endif

#ifndef WG_GEO_DOT_H
#	include <wg_geo.h>
#endif

#include <vector>

namespace wg
{
	class VolumeMeter;
	typedef	StrongPtr<VolumeMeter,Widget_p>		VolumeMeter_p;
	typedef	WeakPtr<VolumeMeter,Widget_wp>		VolumeMeter_wp;

	
	//____ VolumeMeter ____________________________________________________________

	class VolumeMeter : public Widget
	{
	public:
		static VolumeMeter_p	create() { return VolumeMeter_p(new VolumeMeter()); }
	
		bool					isInstanceOf( const char * pClassName ) const;
		const char *			className( void ) const;
		static const char		CLASSNAME[];
		static VolumeMeter_p	cast( const Object_p& pObject );
	
				
		//____ Methods __________________________________________
		
		void			setLEDColors( Color bottomOn, Color middleOn, Color topOn, 
								   Color bottomOff, Color middleOff, Color topOff );
		void			setNbLEDs( int bottomSection, int middleSection, int topSection );
		void			setLEDSpacing( float spacing );
		void			setValue( float peak, float hold );
		void			setDirection( Direction direction );
		void            setSidePadding( float padding ) { m_fSidePadding = padding; }
		void            setZeroInMiddle(bool mb) { m_bZeroInMiddle = mb; }
		void            enableFades(bool b) { m_bUseFades = b; }
		Size			preferredSize() const;

		
	protected:
		VolumeMeter();
		virtual ~VolumeMeter();
		virtual Widget* _newOfMyType() const { return new VolumeMeter(); };
		
		void			_receive( const Msg_p& pMsg );
		void			_render( GfxDevice * pDevice, const Rect& _canvas, const Rect& _window, const Rect& _clip );
		void			_cloneContent( const Widget * _pOrg );
		bool			_alphaTest( const Coord& ofs );
		void			_setSize( const Size& size );
		
		RouteId			m_tickRouteId;
				
		Direction		m_direction;
		Color			m_LEDColors[3][2];
		int				m_nSectionLEDs[3];
		int				m_nLEDs;
		float			m_LEDSpacing;
		
		int			    m_iPeak;
		
		int			    m_iHold;
		float           m_fPeak;
		
		float           m_fSidePadding;
		int             m_iSidePadding;
		
		bool            m_bZeroInMiddle;
		float           d;
		float           d2;
		
		std::vector<float> m_LEDStates;
		
		bool            m_bUseFades;
	};

} // namespage wg
#endif //WG_VOLUMEMETER_DOT_H
