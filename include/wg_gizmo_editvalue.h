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

#ifndef	WG_GIZMO_EDITVALUE_DOT_H
#define	WG_GIZMO_EDITVALUE_DOT_H

#ifndef	WG_GIZMO_DOT_H
#	include <wg_gizmo.h>
#endif

#ifndef	WG_TEXT_DOT_H
#	include <wg_text.h>
#endif

#ifndef WG_VALUEFORMAT_DOT_H
#	include <wg_valueformat.h>
#endif

#ifndef WG_INTERFACE_VALUEHOLDER_DOT_H
#	include <wg_interface_valueholder.h>
#endif


class	WgCursorInstance;
class	WgFont;
class	WgTextManager;

class WgGizmoEditvalue : public WgGizmo, public Wg_Interface_ValueHolder
{
	public:
		WgGizmoEditvalue();
		virtual ~WgGizmoEditvalue();

		virtual const char * Type() const;
		static const char * GetMyType();
		virtual WgGizmo * NewOfMyType() const { return new WgGizmoEditvalue(); };


		//____ Methods __________________________________________
		
		bool	SetMaxInputChars( int max );
		int		MaxInputChars() const { return m_maxInputChars; }
		void	SetTextAlignment( const WgOrigo& origo );
		WgOrigo	GetTextAlignment( ) const;
		void	SetTextColor(WgColor color);
		WgColor GetTextColor() const;
		bool	SetTextProp( const WgTextPropPtr& _pProp );
		WgTextPropPtr GetTextProp( ) const;
		void	SetTextManager(WgTextManager * _pManager);
		WgTextManager * GetTextManager() const;
		void	SetFormat( const WgValueFormat& format );
		const WgValueFormat& GetFormat() const { return m_format; }
		void	Clear();									// Sets value to 0 and clears input field.

		bool	SelectAllText();

		WgSize	DefaultSize() const;


		virtual bool		IsInputField() const	{ return true; }

	protected:

		void	_onUpdate( const WgUpdateInfo& _updateInfo );
		void	_onCloneContent( const WgGizmo * _pOrg );
		void	_onRender( WgGfxDevice * pDevice, const WgRect& _canvas, const WgRect& _window, const WgRect& _clip, Uint8 _layer );
		void	_onAction( WgInput::UserAction action, int button_key, const WgActionDetails& info, const WgInput& inputObj );
		void	_onEnable();
		void	_onDisable();
		void	_onGotInputFocus();
		void	_onLostInputFocus();
		void	_onRefresh();

	private:
		void	_limitCursor();					///< Make sure cursor or selection is not in prefix or suffix part of text.
		void	_selectAll();					///< Our own select all that doesn't include prefix or suffix.

		void	_valueModified();				///< Called when value has been modified.
		void	_rangeModified();				///< Called when range (and thus fractional value) has been modified.

		bool	_parseValueFromInput( int64_t * wpResult );

		void	_regenText();

		bool				m_bRegenText;
		WgValueFormat		m_format;			///< Value format specified by user
		WgValueFormat		m_useFormat;		///< Value format currently used (affected by user typing in values).
		WgText				m_text;
		WgOrigo				m_textOrigo;
		int					m_buttonDownOfs;
		bool				m_bSelectAllOnRelease;
		int					m_maxInputChars;
		WgCoord				m_viewOfs;
};



#endif // WDG_VALUE_DOT_H
