
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


#include <wg_base.h>
#include <wg_msgrouter.h>

#include <wg_textpropmanager.h>
#include <wg_texttool.h>
#include <wg_memstack.h>
#include <wg_hook.h>
#include <wg_standardpresenter.h>

#ifdef USE_FREETYPE
#	include <ft2build.h>
#	include <wg_vectorglyphs.h>
#	include FT_FREETYPE_H
#endif

namespace wg 
{
	
	Base::Data *			Base::s_pData = 0;
	
	
	//____ init() __________________________________________________________________
	
	void Base::init()
	{
		assert( s_pData == 0 );
		assert( sizeof( WeakPtrHub ) == sizeof( Hook_pHub ) );			// Need to be same as we are sharing object stack!
		s_pData = new Data;
		
		s_pData->pDefaultCursor = 0;
		s_pData->pPtrPool = new MemPool( 128, sizeof( WeakPtrHub ) );
		s_pData->pMemStack = new MemStack( 4096 );
	
		s_pData->pDefaultPresenter = StandardPresenter::create();
		s_pData->pDefaultStyle = TextStyle::create();
		
		s_pData->pMsgRouter = MsgRouter::create();
		s_pData->pInputHandler = InputHandler::create();
	
	#ifdef USE_FREETYPE
		s_pData->bFreeTypeInitialized = false;
	#endif
	
		TextTool::setDefaultBreakRules();
	}
	
	//____ exit() __________________________________________________________________
	
	int Base::exit()
	{
		
		if( s_pData == 0 )
			return -1;					// Base already exited or not intialized.
	
		if( !s_pData->pPtrPool->isEmpty() )
			return -2;					// There are weak pointers or hook pointers left.
	
		if( !s_pData->pMemStack->isEmpty() )
			return -3;					// There is data left in memstack.
	
	#ifdef USE_FREETYPE
	
		VectorGlyphs::setSurfaceFactory(0);
		VectorGlyphs::clearCache();
	
		if( s_pData->bFreeTypeInitialized )
			FT_Done_FreeType( s_pData->freeTypeLibrary );
	#endif
		s_pData->pDefaultPresenter = 0;
		s_pData->pDefaultStyle = 0;
	
		delete s_pData->pPtrPool;
		delete s_pData->pMemStack;
		delete s_pData;
		s_pData = 0;
		return 0;
	}
	
	//____ allocWeakPtrHub() ______________________________________________________
	
	WeakPtrHub * Base::allocWeakPtrHub()
	{
		assert( s_pData != 0 );
		return (WeakPtrHub*) s_pData->pPtrPool->allocEntry();
	}
	
	//____ freeWeakPtrHub() _______________________________________________________
	
	void Base::freeWeakPtrHub( WeakPtrHub * pHub )
	{
		assert( s_pData != 0 );
		s_pData->pPtrPool->freeEntry( pHub );
	}
	
	//____ allocHookPtrHub() ______________________________________________________
	
	Hook_pHub * Base::allocHookPtrHub()
	{
		assert( s_pData != 0 );
		return (Hook_pHub*) s_pData->pPtrPool->allocEntry();
	}
	
	//____ freeHookPtrHub() _______________________________________________________
	
	void Base::freeHookPtrHub( Hook_pHub * pHub )
	{
		assert( s_pData != 0 );
		s_pData->pPtrPool->freeEntry( pHub );
	}
	
	//____ initFreeType() _________________________________________________________
	
	#ifdef USE_FREETYPE
	bool Base::initFreeType()
	{
		assert( s_pData != 0 );
		if( s_pData->bFreeTypeInitialized )
			return true;
	
		FT_Error err = FT_Init_FreeType( &s_pData->freeTypeLibrary );
		if( err == 0 )
		{
			s_pData->bFreeTypeInitialized = true;
			return true;
		}
	
		return false;
	}
	#endif
	
	
	
	//____ setDefaultTextManager() _________________________________________________
	
	/*void Base::setDefaultTextManager( const TextMgrPtr& pManager )
	{
		m_pDefTextMgr = pManager;
	}
	
	
	//____ getDefaultTextManager() _________________________________________________
	
	const TextMgrPtr& Base::getDefaultTextManager()
	{
		return m_pDefTextMgr;
	}
	*/
	
	//____ setDefaultCaret() _______________________________________________________
	
	void Base::setDefaultCaret( const Caret2_p& pCaret )
	{
		assert( s_pData != 0 );
		s_pData->pDefaultCaret = pCaret;	
	}
	
	//____ setDefaultPresenter() ___________________________________________________
	
	void Base::setDefaultPresenter( const TextPresenter_p& pPresenter )
	{
		assert( s_pData != 0 );
		s_pData->pDefaultPresenter = pPresenter;	
	}
	
	//____ setDefaultStyle() _______________________________________________________
	
	void Base::setDefaultStyle( const TextStyle_p& pStyle )
	{
		assert( s_pData != 0 );
		s_pData->pDefaultStyle = pStyle;	
	}
	
	
	
	//____ setDefaultTextprop() ___________________________________________________
	
	void Base::setDefaultTextprop( const Textprop_p& pProp )
	{
		assert( s_pData != 0 );
		s_pData->pDefaultTextprop = pProp;
	}
	
	//____ setDefaultSelectionProp() ___________________________________________________
	
	void Base::setDefaultSelectionProp( const Textprop_p& pProp )
	{
		assert( s_pData != 0 );
		s_pData->pDefaultSelectionProp = pProp;
	}
	
	//____ setDefaultLinkProp() ___________________________________________________
	
	void Base::setDefaultLinkProp( const Textprop_p& pProp )
	{
		assert( s_pData != 0 );
		s_pData->pDefaultLinkProp = pProp;
	}
	
	
	//____ setDefaultCursor() ___________________________________________________
	
	void Base::setDefaultCursor( const Caret_p& pCursor )
	{
		assert( s_pData != 0 );
		s_pData->pDefaultCursor = pCursor;
	}
	
	
	//____ memStackAlloc() ________________________________________________________
	
	char * Base::memStackAlloc( int bytes )
	{ 
		assert(s_pData!=0); 
		return s_pData->pMemStack->alloc(bytes);
	}
	
	//____ memStackRelease() ______________________________________________________
	
	void Base::memStackRelease( int bytes )
	{	assert(s_pData!=0); 
		return s_pData->pMemStack->release(bytes); 
	}

} // namespace wg