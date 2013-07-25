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
#ifndef WG_BLOCKSKIN_DOT_H
#define WG_BLOCKSKIN_DOT_H

#ifndef WG_EXTENDEDSKIN_DOT_H
#	include <wg_extendedskin.h>
#endif

class WgSurface;

class WgBlockSkin;
typedef	WgSmartPtr<WgBlockSkin,WgExtendedSkinPtr>	WgBlockSkinPtr;


class WgBlockSkin : public WgExtendedSkin
{
	//TODO: Add sanity-checking to all Set-methods.
	//TODO: Optimize rendering based on invisibleSections and opaqueSections!

public:
	static WgBlockSkinPtr Create();
	static WgBlockSkinPtr CreateStatic( WgSurface * pSurface, WgRect block, WgBorders frame = WgBorders(0) );
	static WgBlockSkinPtr CreateEnable( WgSurface * pSurface, WgSize blockSize, WgCoord ofsEnabled, WgCoord ofsDisabled, WgBorders frame = WgBorders(0) );
	static WgBlockSkinPtr CreateClickable( WgSurface * pSurface, WgSize blockGeo, WgCoord blockStartOfs, WgSize blockPitch, WgBorders blockFrame = WgBorders(0) );
	static WgBlockSkinPtr CreateSelectable( WgSurface * pSurface, WgSize blockGeo, WgCoord blockStartOfs, WgSize blockPitch, WgBorders blockFrame = WgBorders(0) );
	static WgBlockSkinPtr CreateClickSelectable( WgSurface * pSurface, WgSize blockGeo, WgCoord blockStartOfs, WgSize blockPitch, WgBorders blockFrame = WgBorders(0) );
	static WgBlockSkinPtr CreateClickSelectableWidget( WgSurface * pSurface, WgSize blockGeo, WgCoord blockStartOfs, WgSize blockPitch, WgBorders blockFrame = WgBorders(0) );

	static WgBlockSkinPtr CreateStaticFromSurface( WgSurface * pSurface, WgBorders frame = WgBorders(0) );
	static WgBlockSkinPtr CreateEnableFromSurface( WgSurface * pSurface, int blockSpacing, WgBorders blockFrame = WgBorders(0) );
	static WgBlockSkinPtr CreateClickableFromSurface( WgSurface * pSurface, int blockSpacing, WgBorders blockFrame = WgBorders(0) );
	static WgBlockSkinPtr CreateSelectableFromSurface( WgSurface * pSurface, int blockSpacing, WgBorders blockFrame = WgBorders(0) );
	static WgBlockSkinPtr CreateClickSelectableFromSurface( WgSurface * pSurface, int blockSpacing, WgBorders blockFrame = WgBorders(0) );


	~WgBlockSkin() {};

	void	SetSurface( WgSurface * pSurf );
	bool	SetBlockGeo( WgSize size, WgBorders frame = WgBorders(0) );
	void	SetStateBlock( WgStateEnum state, const WgCoord& ofs );

	void	SetAllBlocks( WgCoord& ofs );
	void	SetDisabledBlock( WgCoord& ofs );
	void	SetHoveredBlocks( WgCoord& ofs );
	void	SetPressedBlocks( WgCoord& ofs );
	void	SetSelectedBlocks( WgCoord& ofs );

	void	SetTiled( bool bTiled );
	void	SetTiledTopBorder( bool bTiled );
	void	SetTiledBottomBorder( bool bTiled );
	void	SetTiledLeftBorder( bool bTiled );
	void	SetTiledRightBorder( bool bTiled );
	void	SetTiledCenter( bool bTiled );

	void	OptimizeRenderMethods();

	void	Render( WgGfxDevice * pDevice, const WgRect& _canvas, WgState state, const WgRect& _clip ) const;

	WgSize	MinSize() const;
	WgSize	PreferredSize() const;

	WgSize	SizeForContent( const WgSize contentSize ) const;

	bool	MarkTest( const WgCoord& ofs, const WgRect& canvas, WgState state, int opacityTreshold ) const;

	bool	IsOpaque() const;
	bool	IsOpaque( WgState state ) const;
	bool	IsOpaque( const WgRect& rect, const WgSize& canvasSize, WgState state ) const;
	
	bool	IsStateIdentical( WgState state, WgState comparedTo ) const;

private:
	struct StateData
	{
		WgCoord	ofs;
		int		opaqueSections;
		int		invisibleSections;
	};

	WgBlockSkin();
	void	_setBitFlag( int& bitmask, int bit, bool bSet );
	void	_renderNoClip( WgGfxDevice * pDevice, const StateData * pState, const WgRect& _canvas ) const;
	void	_scanStateBlockSectionArea( StateData * pState, WgOrigo section, const WgRect& sectionArea );

	static const int ALL_SECTIONS = 0x1FF;

	WgSurface *	m_pSurface;
	WgSize		m_dimensions;
	WgBorders	m_frame;
	int			m_tiledSections;
	bool		m_bIsOpaque;

	StateData	m_state[WG_NB_STATES];
};

#endif //WG_BLOCKSKIN_DOT_H