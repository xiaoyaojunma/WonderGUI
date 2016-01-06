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

#ifndef	WG_TYPES_DOT_H
#define WG_TYPES_DOT_H

#include <stdint.h>			// Use the C99 official header

namespace wg 
{
		
	#ifndef INT64_MIN
	#define INT64_MIN (-(9223372036854775807 ## L)-1)
	#endif
	
	#ifndef INT64_MAX
	#define INT64_MAX ((9223372036854775807 ## L))__
	#endif


#if defined(_WIN32) 	
#	if defined(_M_X64) || defined(_M_IX86)
#		define IS_BIG_ENDIAN 0
#		define IS_LITTLE_ENDIAN 1
#	endif
#else
	#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && defined(__ORDER_LITTLE_ENDIAN__)
	#	 if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	#		define IS_BIG_ENDIAN 1
	#		define IS_LITTLE_ENDIAN 0
	#	elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	#		define IS_BIG_ENDIAN 0
	#		define IS_LITTLE_ENDIAN 1
	#	endif
	#else
	#	ifdef __BIG_ENDIAN__
	#		if __BIG_ENDIAN__
	#			define IS_BIG_ENDIAN 1
	#			define IS_LITTLE_ENDIAN 0
	#		endif
	#	endif

	#	ifdef __LITTLE_ENDIAN__
	#		if __LITTLE_ENDIAN__
	#			define IS_BIG_ENDIAN 0
	#			define IS_LITTLE_ENDIAN 1
	#		endif
	#	endif
	#endif
#endif


	#ifndef IS_BIG_ENDIAN
	#error Could not detect endianness. You need to define IS_BIG_ENDIAN and IS_LITTLE_ENDIAN in wg_types.h
	#define IS_BIG_ENDIAN 0
	#define IS_LITTLE_ENDIAN 0
	#endif
	
	template<typename T> inline T min(const T &a, const T &b) { return a < b ? a : b; }
	template<typename T> inline T min(const T &a, const T &b, const T &c) { if( a < b ) return a < c ? a : c; else return b < c ? b : c; }
	template<typename T> inline T max(const T &a, const T &b) { return a > b ? a : b; }
	template<typename T> inline T max(const T &a, const T &b, const T &c) { if( a > b ) return a > c ? a : c; else return b > c ? b : c; }
	template<typename T> inline T abs(T x)					{ return x >= 0 ? x : -x; }
//	template<typename T> inline void swap(T &a, T &b) { T c = a; a = b; b = c; }
	
	#define		LIMIT(x, min, max)	if( x < min) x = min; if( x > max) x = max;
	
	#define	FONTSIZE_INHERIT 0x80000000
	
	enum class StateEnum : uint8_t
	{
		Normal					= 0,			///< Element is neither hovered, pressed, selected or focused.
		Selected				= 1,			///< Element is in a selected state, like a selected checkbox or item in a list. 
		Focused					= 2,			///< Element has keyboard focus
		FocusedSelected			= 2+1,
		Hovered					= 4,			///< Mouse pointer is hovering over element.
		HoveredSelected			= 4+1,
		HoveredFocused			= 4+2,
		HoveredFocusedSelected	= 4+2+1,
		Pressed					= 8+4,			///< Mouse button (usually left one) is pressed on element.
		PressedSelected			= 8+4+1,
		PressedFocused			= 8+4+2,
		PressedFocusedSelected	= 8+4+2+1,
		Disabled				= 16,			///< Element is disabled and can't be focused or pressed.
		DisabledSelected		= 16+1,
	};
	
	#define	WG_NB_STATES		14			// Number of states
	#define	WG_MAX_STATE_VALUE	17			// Highest value for StateEnum
	
	class State 
	{
	public:
		State() { m_state = (uint8_t) StateEnum::Normal; }
		State( StateEnum state ) { m_state = (uint8_t) state; }
	
	//	void		set( WgModeEnum state ) { m_state = state; }
	//	WgModeEnum	getEnum() const { return (WgModeEnum) m_state; }
	
		bool	setEnabled(bool bEnabled) { if(bEnabled) m_state &= ~ ((uint8_t)StateEnum::Disabled); else m_state = (m_state & ((uint8_t)StateEnum::Selected)) | ((uint8_t)StateEnum::Disabled); return true; }
		bool	setSelected(bool bSelected) { if(bSelected) m_state |= ((uint8_t)StateEnum::Selected); else m_state &= ~((uint8_t)StateEnum::Selected); return true; }
		bool	setFocused(bool bFocused) { if( m_state == ((uint8_t)StateEnum::Disabled) ) return false; if(bFocused) m_state |= ((uint8_t)StateEnum::Focused); else m_state &= ~((uint8_t)StateEnum::Focused); return true; }
		bool	setHovered(bool bHovered) { if( m_state == ((uint8_t)StateEnum::Disabled) ) return false; if(bHovered) m_state |= ((uint8_t)StateEnum::Hovered); else m_state &= ~((uint8_t)StateEnum::Pressed); return true; }
		bool	setPressed(bool bPressed) { if( m_state == ((uint8_t)StateEnum::Disabled) ) return false; if(bPressed) m_state |= ((uint8_t)StateEnum::Pressed); else m_state &= ~(((uint8_t)StateEnum::Pressed) - ((uint8_t)StateEnum::Hovered)); return true; }
	
	
		bool	isEnabled() const { return (m_state & ((uint8_t)StateEnum::Disabled)) == ((uint8_t)StateEnum::Normal); }
		bool	isSelected() const { return (m_state & ((uint8_t)StateEnum::Selected)) == ((uint8_t)StateEnum::Selected); }
		bool	isFocused() const { return (m_state & ((uint8_t)StateEnum::Focused)) == ((uint8_t)StateEnum::Focused); }
		bool	isHovered() const { return (m_state & ((uint8_t)StateEnum::Hovered)) == ((uint8_t)StateEnum::Hovered); }
		bool	isPressed() const { return (m_state & ((uint8_t)StateEnum::Pressed)) == ((uint8_t)StateEnum::Pressed); }
	
		inline bool operator==(StateEnum state) const { return m_state == ((uint8_t)state); }
		inline bool operator!=(StateEnum state) const { return m_state != ((uint8_t)state); }
	
		inline void operator=(StateEnum state) { m_state = ((uint8_t)state); }
	
		operator StateEnum() const { return (StateEnum) m_state; }
	
	private:
		uint8_t		m_state;
	};
	
	
	typedef unsigned int	RouteId;
	
	
	//____ TxtAttr ______________________________________________________________
	
	// Bitmask for various text attributes.
	
	enum class TxtAttr
	{
		Underlined	= 0x1,
		Selected		= 0x2
	};
	
	
	//____ ExtChar __________________________________________________________
	
	
	//0x1b
	
	// Double escape codes should give the escape-code character.
	
	/*
			NEW ONES
	
			{prop}		Set the named property. Named properties should start with a-z/A-Z.
						If property is unnamed you get {123} where the number is the current handle for the prop.
	
			-			break permitted
			=			hyphen break permitted
			n			linebreak (like \n).
	
			Predefined properties
	
			{n}			empty property (normal/default)
			{b}			bold
			{i}			italic
			{u}			underlined
			{b-i}		bold italic
			{b-u}		bold underlined
			{b-i-u}		bold italic underlined
			{i-u}		italic underlined
			{h1}		heading 1
			{h2}		heading 2
			{h3}		heading 3
			{h4}		heading 4
			{h5}		heading 5
			{u1}		user defined style 1
			{u2}		user defined style 2
			{u3}		user defined style 3
			{u4}		user defined style 4
			{u5}		user defined style 5
	
			{super}		superscript		// Includes top positioning
			{sub}		subscript		// Includes bottom positioning
			{mono}		monospaced		// Includes monospacing
	
			{black}		black text
			{white}		white text
	
	*/
	
	
	
	/*
		{[rrggbbaa]		begin color
		}				end color
	
		[123			begin size, exactly 3 decimal digits sets the size.
		]				end size
	
		_				begin underlined
		| 				end underlined
	
		:[0-4]			set break level
		;				end break level
	
	
		-				break permitted
		=				hyphen break permitted
	
		d				begin normal (default)
		b				begin bold
		i				begin italic
		I				begin bold italic
		s				begin subscript
		S				begin superscript
		m				begin monospace
		h[1-5]			begin heading
		u[1-5]			begin userdefined style
	
		#				end style
	
		(prop)			set a new text property, looked up from a ResDB.
						Other style/color settings are applied ontop of this text property.
						Setting prop as (null) sets an empty prop.
	
	
	
	*/
	
	
	enum class ExtChar
	{
		// Code inside WonderGUI assumes these are all
		// in the range 0x80-0xA0, but that might change in the future.
	
		BreakPermitted			= 0x82,
		HyphenBreakPermitted	= 0x83,
		NoBreakSpace			= 0xA0,
	
		Ellipsis					= 0x2026
	};
	
	//____ CodePage ______________________________________________________________
	
	enum class CodePage
	{
		Latin1,
		_1250,		///< Windows Cental Europe
		_1251,		///< Windows Cyrillic
		_1252,		///< Windows Latin-1 extended
		_1253,		///< Windows Greek
		_1254,		///< Windows Turkish
		_1255,		///< Windows Hebrew
		_1256,		///< Windows Arabic
		_1257,		///< Windows Baltic
		_1258,		///< Windows Vietnam
		_874,		///< Windows Thai
	};
	
	#define WG_NB_CODEPAGES 11
	
	//____ BreakRules ____________________________________________________________
	
	enum class BreakRules
	{
		NoBreak		= 0,
		BreakBefore	= 16,
		BreakOn		= 32,
		BreakAfter	= 64
	};
	
	//____ BlendMode ____________________________________________________________
	
	// BlendModes control how blits and fills are blended against the background.
	
	enum class BlendMode //: uint8_t
	{
		Opaque,			///< Completely opaque blitting, ignoring alpha of source and tint-color.
		Blend,				///< Normal mode, alpha of source and tint-color is taken into account.
		Add,				///< RGBA Additive, alpha of source and tint-color is taken into account.
		Multiply,			///< RGBA Multiply, alpha of source and tint-color is taken into account.
		Invert				///< Inverts destination RGB values where alpha of source is non-zero. Ignores RBG components. Uses alpha of tint-color.
	};
	
	//____ FontAlt ____________________________________________________________
	
	enum class FontAlt
	{
		// Needs to stay in sync with ExtChar::WG_BEGIN_NORMAL and following enums!
	
		Normal		= 0,
		Bold		= 1,
		BoldItalic  = 2,
		Italic		= 3,
		Superscript	= 4,
		Subscript	= 5,
		Monospace	= 6,
		Heading_1	= 7,
		Heading_2	= 8,
		Heading_3	= 9,
		Heading_4	= 10,
		Heading_5	= 11,
	
		User_1		= 12,
		User_2		= 13,
		User_3		= 14,
		User_4		= 15,
		User_5		= 16,
	
		Inherit	= 99,
	};
	
	#define WG_NB_FONTSTYLES	17
	
	
	//____ PointerStyle __________________________________________________________
	
	enum class PointerStyle
	{
		Arrow,						// default arrow
		Default = Arrow,	// default arrow
		Hourglass,					// hourglass
		Hand,							// hand
		Crosshair,					// crosshair
		Help,							// help
		Ibeam,						// I-beam
		Stop,							// slashed circle
		UpArrow,						// arrow pointing up
		ResizeAll,						// four-pointed arrow in all directions
		ResizeNeSw,					// double-pointed arrow pointing northeast and southwest
		ResizeNwSe,					// double-pointed arrow pointing northwest and southeast
		ResizeNS,						// double-pointed arrow pointing north and south
		ResizeWE,						// double-pointed arrow pointing west and east
	};
	
	//____ MouseButton _________________________________________________________
	
	enum class MouseButton
	{
		None = 0,
		Left,
		Middle,
		Right,
		X1,
		X2,
	};
	
	#define	WG_MAX_BUTTONS		((int)MouseButton::X2)+1	
	
	
	//____ AnimMode _____________________________________________________________
	
	enum class AnimMode
	{
		Forward,
		Backward,
		Looping,
		BackwardLooping,
		PingPong,
		BackwardPingPong
	};
	
	
	//____ TintMode _____________________________________________________________
	
	// TintMode controls how TintColors are blended hierarchically.
	
	enum class TintMode
	{
		Opaque,
		Multiply
	};
	
	
	/*
	enum	ExtChar
	{
			BREAK_PERMITTED_HERE	= 0x82,
	//		BEGIN_SELECTED_AREA		= 0x86,
	//		END_SELECTED_AREA		= 0x87,
			HYPHEN_BREAK_PERMITTED_HERE	= 0x91,
	//		BEGIN_UNDERLINE			= 0x91,
	//		END_UNDERLINE			= 0x92,
	
	//		BEGIN_BOLD				= 0x93,
	//		END_BOLD				= 0x94,
	//		BEGIN_ITALIC			= 0x95,
	//		END_ITALIC				= 0x96,
	//		BEGIN_USEREFFECT1		= 0x97,
	//		END_USEREFFECT1			= 0x98,
	//		BEGIN_USEREFFECT2		= 0x99,
	//		END_USEREFFECT2			= 0x9A,
	
	
	//		END_COLOR				= 0x9C,
	//		START_OF_OS_COMMAND		= 0x9D,
	//		END_OF_OS_COMMAND		= 0x9E
	};
	*/
	
	
	//____ SearchMode _____________________________________________________________
	
	enum class SearchMode
	{
		MarkPolicy,			///< Perform a mark test on Widget.
		Geometry,				///< Goes strictly on geometry, ignores alpha.
		ActionTarget,		///< Like MARKPOLICY, but takes modality into account.
	};
	
	//____ Origo _____________________________________________________________
	
	enum class Origo
	{
		// Clockwise from upper left corner, center last. Must be in range 0-8
	
		NorthWest	= 0,
		North		= 1,
		NorthEast	= 2,
		East		= 3,
		SouthEast	= 4,
		South		= 5,
		SouthWest	= 6,
		West		= 7,
		Center		= 8
	};
	
	//____ Direction ____________________________________________________________
	
	enum class Direction
	{
		Up,
		Down,
		Left,
		Right
	};
	
	//____ Orientation __________________________________________________________
	
	enum class Orientation
	{
		Horizontal,
		Vertical
	};
	
	//____ SizePolicy ___________________________________________________________
	/**
		SizePolicy is used by certain containers, including FlexPanel and ScrollPanel, 
		to limit/control the geometry of children. Different SizePolicies can be set
		for horizontal and vertical size.
	
		It is used in combination with a size specified by parent. In the case of
		ScrollPanel it is the size of the window to the scrollarea. In the case of
		FlexPanel, it is a size specified in the childs hook.
	*/
	enum class SizePolicy
	{
		Default = 0,			///< Childs size is unaffected by size specified by parent, so it gets its preferred size.
		Bound,				///< Child is bound to the exact size specified by parent.
		Confined,			///< Childs size is limited to the size specified by parent.
		Expanded,			///< Childs size is set to at least the size specified by parent.
	};
	
	//____ MsgType ______________________________________________________________
	
	enum class MsgType
	{
		Dummy = 0,
		Tick,
		PointerChange,
	
		FocusGained,
		FocusLost,
	
		MouseEnter,
		MouseMove,
		MouseLeave,
	
		MousePress,
		MouseRepeat,
		MouseDrag,
		MouseRelease,
		MouseClick,
		MouseDoubleClick,
	
		KeyPress,
		KeyRepeat,
		KeyRelease,
		TextInput,
		WheelRoll,
	
		Select,						// Non-value widget triggered, like a button being pressed.
		Toggle,						// Boolean value widget toggled, like for checkboxes, radiobuttons etc.
		ValueUpdate,					// Value of widget changed, like for editvalue, animations, sliders etc
		RangeUpdate,					// Range widget updated, such as scrollbar, scrollpanel, rangesliders etc.
	
		TextEdit,						// Text widget edited, like texteditor, editline, etc.
	
	
		ItemToggle,
	//	ItemMouseEnter,
	//	ItemMouseLeave,
		ItemMousePress,
	//	ItemMouseDrag,
	//	ItemMouseRepeat,
	//	ItemMouseRelease,
	//	ItemMouseClick,
	//	ItemMouseDoubleClick,
		ItemsSelect,
		ItemsUnselect,
	
		PopupClosed,
	
		ModalMoveOutside,
		ModalBlockedPress,
		ModalBlockedRelease,
	
		LinkSelect,
		LinkMouseEnter,
		LinkMouseLeave,
		LinkMousePress,
		LinkMouseRepeat,
		LinkMouseRelease,
		LinkMouseClick,
		LinkMouseDoubleClick,
		
		Max
	};
	
	
	//____ SortOrder ____________________________________________________________
	
	enum class SortOrder
	{
		Ascending,
		Descending
	};
	
	//____ Unit ____________________________________________________________
	
	enum class Unit
	{
		Px,
		Em
	};
	
	
	//____ SelectMode ___________________________________________________________
	
	enum class SelectMode
	{
		Unselectable,			///< Entries can not be selected.
		SingleEntry,		///< Only a single entry can be selected at a time.
		MultiEntries,		///< Multiple entries can be selected at the same time.
		FlipOnSelect			///< Multiple entries can be selected at the same time and are switched individually.
	};
	
	//____ TextEditMode _________________________________________________________
	
	enum class TextEditMode
	{
		Static,
		Selectable,
		Editable,
	};
	
	
	//____ TextPos ______________________________________________________________
	
	class TextPos
	{
	public:
		TextPos() : line(0), col(0) {}
		TextPos( int line, int col ) : line(line), col(col) {}
	
		int	line;
		int col;
	};
	
	//___  TextDecoration ________________________________________________________
	
	enum class TextDecoration
	{
		Inherit,
		None,
		Underline
	};
	
	//____ Range ____________________________________________________________
	
	class Range
	{
	public:
		Range() : ofs(0), len(0) {}
		Range( int ofs, int len ) : ofs(ofs), len(len) {}
	
		inline int	begin() const { return ofs; }
		inline int	end() const { return ofs+len; }
	
		int ofs;
		int len;
	};
	
	
	//____ SortContext __________________________________________________________
	
	struct SortContext
	{
	public:
		bool	bAscend;
	};
	
	//____ AccessMode ____________________________________________________________
	
	enum class AccessMode
	{
		None,
		ReadOnly,
		WriteOnly,
		ReadWrite
	};
	
	//____ PixelType _____________________________________________________________
	
	enum class PixelType
	{
		Unknown,			///< Pixelformat is unkown or can't be expressed in a PixelFormat struct.
		Custom,				///< Pixelformat has no PixelType enum, but is fully specified through the PixelFormat struct.
		BGR_8,				///< One byte of blue, green and red respectively in memory in exactly that order.
		BGRA_8				///< One byte of blue, green, red and alpha respectively in memory in exactly that order.
	};
	
	
	//____ PixelFormat __________________________________________________________
	/**
	 * @brief Describes the format of a pixel.
	 *
	 * Describes the format of a pixel.
	 *
	 * The format of the pixel is described in three ways by a PixelFormat object:
	 *
	 * First a PixelType enum that contains predefined values for common pixel formats.
	 * This allows for human readable information and quick lockups.
	 *
	 * Secondly a set of variables containing the number of bits for each pixel and the
	 * individual red, green, blue and alpha channels. This allows for quickly and easily
	 * checking the size of a pixel and the color depths and presence and depth of the alpha channel.
	 *
	 * Thirdly a pair of mask and shift variables for each individual channel, specifies the position
	 * of each channel and allows for quick conversion to and from the default 32-bit RGBA format used by Color.
	 *
	 * Not all pixel formats (like those of index/palette-based surfaces) can
	 * be fully described by a PixelFormat object. In that case the member type is set to Unknown.
	 *
	 * As long as the type member is not set to Unknown, you can extract the value of any channel of a
	 * pixel by applying the mask and shift variables. I.e. to extract the value of red from a pixel
	 * as an 8-bit value in the range 0-255, you use the formula:
	 *
	 * redValue = (pixel & R_mask) >> R_shift
	 *
	 * Thus you can convert any specified pixel type to a Color structure using the following routine:
	 *
	 * uint32_t	pixel;
	 * PixelFormat * pFormat;
	 *
	 * 	Color col( (pixel & pFormat->R_mask) >> pFormat->R_shift,
	 *				 (pixel & pFormat->G_mask) >> pFormat->G_shift,
	 *				 (pixel & pFormat->B_mask) >> pFormat->B_shift,
	 *				 (pixel & pFormat->A_mask) >> pFormat->A_shift );
	 *
	 * To convert a Color object to a pixel value you can use:
	 *
	 * Color color;
	 * PixelFormat * pFormat;
	 *
	 * 	uint32_t pix = ((color.r << pFormat->R_shift) & pFormat->R_mask) |
	 *				 ((color.g << pFormat->G_shift) & pFormat->G_mask) |
	 *				 ((color.b << pFormat->B_shift) & pFormat->B_mask) |
	 *				 ((color.a << pFormat->A_shift) & pFormat->A_mask);
	 *
	 * This is essentially what the default implementation for Surface::colorToPixel() and Surface::pixelToColor() does.
	 *
	 **/
	
	
	struct PixelFormat
	{
		PixelType	type;			///< Enum specifying the format if it exacty matches a predefined format, otherwise set to CUSTOM or UNKNOWN.
		int			bits;			///< Number of bits for the pixel, includes any non-used padding bits.
	
		uint32_t	R_mask;				///< bitmask for getting the red bits out of the pixel
		uint32_t	G_mask;				///< bitmask for getting the green bits out of the pixel
		uint32_t	B_mask;				///< bitmask for getting the blue bits out of the pixel
		uint32_t	A_mask;				///< bitmask for getting the alpha bits out of the pixel
	
		int		R_shift;			///< amount to shift the red bits to get an 8-bit representation of red. This can be negative.
		int		G_shift;			///< amount to shift the green bits to get an 8-bit representation of red. This can be negative.
		int		B_shift;			///< amount to shift the blue bits to get an 8-bit representation of red. This can be negative.
		int		A_shift;			///< amount to shift the alpha bits to get an 8-bit representation of red. This can be negative.
	
		uint8_t	R_bits;				///< number of bits for red in the pixel
		uint8_t	G_bits;				///< number of bits for green in the pixel
		uint8_t	B_bits;				///< number of bits for blue in the pixel
		uint8_t	A_bits;				///< number of bits for alpha in the pixel
	};
	
	
	//____ MaskOp ____________________________________________________________
	
	enum class MaskOp
	{
		Recurse = 0,	///< Recurse through children, let them mask background individually.
		Skip = 1,		///< Do not mask background against container or children.
		Mask = 2		///< Mask background against whole container.
	};
	
	

} // namespace wg
#endif // WG_TYPES_DOT_H
