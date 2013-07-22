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

#ifndef WG_SHADERCAPSULE_DOT_H
#define WG_SHADERCAPSULE_DOT_H

#ifndef WG_CAPSULE_DOT_H
#	include <wg_capsule.h>
#endif

class WgShaderCapsule;
typedef	WgSmartChildPtr<WgShaderCapsule,WgCapsulePtr>		WgShaderCapsulePtr;
typedef	WgWeakChildPtr<WgShaderCapsule,WgCapsuleWeakPtr>	WgShaderCapsuleWeakPtr;

class WgShaderCapsule : public WgCapsule
{
public:
	static WgShaderCapsulePtr	Create() { return WgShaderCapsulePtr(new WgShaderCapsule()); }

	bool		IsInstanceOf( const char * pClassName ) const;
	const char *ClassName( void ) const;
	static const char	CLASSNAME[];
	static WgShaderCapsulePtr	Cast( const WgObjectPtr& pObject );

	void		SetColor( const WgColor& color);
	void		SetTintMode( WgTintMode mode );
	void		SetBlendMode( WgBlendMode mode );

	inline WgColor		Color() { return m_tintColor; }
	inline WgBlendMode	BlendMode() { return m_blendMode; }
	inline WgTintMode	TintMode() { return m_tintMode; }


protected:
	WgShaderCapsule();
	virtual ~WgShaderCapsule();
	virtual WgWidget* _newOfMyType() const { return new WgShaderCapsule(); };

	void		_renderPatches( WgGfxDevice * pDevice, const WgRect& _canvas, const WgRect& _window, WgPatches * _pPatches );
	void		_onCloneContent( const WgWidget * _pOrg );
	WgBlendMode _getBlendMode() const;

private:
	WgColor			m_tintColor;
	WgBlendMode		m_blendMode;
	WgTintMode		m_tintMode;

};

#endif //WG_SHADERCAPSULE_DOT_H
