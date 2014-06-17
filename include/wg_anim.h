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

#ifndef WG_ANIM_DOT_H
#define WG_ANIM_DOT_H

#ifndef	WG_TYPES_DOT_H
#	include <wg_types.h>
#endif

#ifndef WG_CHAIN_DOT_H
#	include <wg_chain.h>
#endif

#ifndef WG_POINTERS_DOT_H
#	include <wg_pointers.h>
#endif

class		WgKeyFrame;
struct	WgAnimPlayPos;


//____ Class WgKeyFrame _______________________________________________________

class WgKeyFrame : public WgLink
{
	friend class WgAnim;
public:
	bool	SetDuration( int ticks );
	int		Duration( void ) { return m_duration; };
	int		Timestamp( void ) { return m_timestamp; };

protected:
	int		m_timestamp;
	int		m_duration;
	LINK_METHODS( WgKeyFrame );
};

class WgAnim;
typedef	WgStrongPtr<WgAnim,WgObjectPtr>		WgAnimPtr;
typedef	WgWeakPtr<WgAnim,WgObjectWeakPtr>	WgAnimWeakPtr;

//____ Class WgAnim ______________________________________________________

class WgAnim : public WgObject
{
	friend class WgKeyFrame;

public:
	bool				IsInstanceOf( const char * pClassName ) const;
	const char *		ClassName( void ) const;
	static const char	CLASSNAME[];
	static WgAnimPtr	Cast( const WgObjectPtr& pObject );

	bool				SetPlayMode( WgAnimMode mode );
	bool				SetTimeScaler( float scale );

	bool				DeleteKeyFrame( int pos );
	bool				DeleteKeyFrame( WgKeyFrame * pKeyFrame );
	void				Clear( void ) { m_keyframes.Clear(); };

	int					Duration( void ) { return m_duration; };
	WgAnimMode			PlayMode( void ) { return m_playMode; };
	float				TimeScaler( void ) { return m_scale; };
	int					DurationScaled( void ) { return (int) (m_duration * m_scale); };
	int					TimeToOfs( int64_t ticks );			/// Convert play-time to offset in animation by scaling with timeScaler and unwinding loops.

protected:
	WgAnim();
	virtual ~WgAnim();

	// Meant to be overloaded with methods by the same name that builds up their 
	// WgKeyFrame-derived class.

	bool				_insertKeyFrame( int pos, WgKeyFrame * pFrame, int duration );
	bool				_insertKeyFrame( WgKeyFrame * pBefore, WgKeyFrame * pFrame, int duration );
	bool				_addKeyFrame( WgKeyFrame * pFrame, int duration );

	// Meant to be overloaded with public methods returning right type.

	WgKeyFrame *		_firstKeyFrame( void ) {return m_keyframes.First();};
	WgKeyFrame *		_lastKeyFrame( void ) {return m_keyframes.Last();};

	//

	WgAnimPlayPos		_playPos( int64_t ticks, WgKeyFrame * pProximity = 0 ) const;		// ticks gets scaled.

	WgKeyFrame *		_keyFrame( int64_t ticks, WgKeyFrame * pProximity = 0 ) const;

private:
	
	float				m_scale;			// Only used for getKeyFrame
	int					m_duration;
	WgAnimMode			m_playMode;	
	WgChain<WgKeyFrame>	m_keyframes;
};

//____ Struct WgAnimPlayPos ___________________________________________________

struct WgAnimPlayPos
{
	WgKeyFrame *	pKeyFrame1;					// KeyFrame we are transitioning from.
	WgKeyFrame *	pKeyFrame2;					// KeyFrame we are transitioning towards.
	float			transition;					// 0 -> 1.0, current position. (1.0 only when anim has ended).
	int				animOffset;					// Offset in ticks from start of animation (unwinding loops etc).
};


#endif //WG_ANIM_DOT_H
