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
#ifndef WG_EVENTHANDLER_DOT_H
#define WG_EVENTHANDLER_DOT_H

#include <deque>
#include <map>
#include <vector>

#ifndef WG_EVENT_DOT_H
#	include <wg_event.h>
#endif

#ifndef WG_EVENTFILTER_DOT_H
#	include <wg_eventfilter.h>
#endif


#ifndef WG_KEY_DOT_H
#	include <wg_key.h>
#endif

class WgRoot;
class WgGizmoContainer;

class WgEventListener
{
public:
	virtual ~WgEventListener() {};

	virtual void OnEvent( const WgEvent::Event * pEvent ) = 0;
};



class WgEventHandler
{
friend class WgGizmo;

public:
	WgEventHandler( WgRoot * pRoot );
	~WgEventHandler();

	bool	QueueEvent( WgEvent::Event * pEvent );

	void	ProcessEvents();

	//----

	bool	SetFocusGroup( WgGizmoContainer * pFocusGroup );
	bool	SetKeyboardFocus( WgGizmo * pFocus );

	WgGizmoContainer *	FocusGroup() const;
	WgGizmo *			KeyboardFocus() const { return m_keyFocusGizmo.GetRealPtr(); }

	//----

	bool	IsMouseButtonPressed( int button );
	bool	IsKeyPressed( int native_keycode );

	bool	IsWindowFocused() const { return m_bWindowFocus; }

	//----

	void	AddCallback( void(*fp)( const WgEvent::Event * pEvent) );
	void	AddCallback( void(*fp)( const WgEvent::Event * pEvent, void * pParam), void * pParam );
	void	AddCallback( void(*fp)( const WgEvent::Event * pEvent, WgGizmo * pDest), WgGizmo * pDest );
	void	AddCallback( WgEventListener * pListener );

	void	AddCallback( const WgEventFilter& filter, void(*fp)( const WgEvent::Event * pEvent) );
	void	AddCallback( const WgEventFilter& filter, void(*fp)( const WgEvent::Event * pEvent, void * pParam), void * pParam );
	void	AddCallback( const WgEventFilter& filter, void(*fp)( const WgEvent::Event * pEvent, WgGizmo * pDest), WgGizmo * pDest );
	void	AddCallback( const WgEventFilter& filter, WgEventListener * pListener );

	int		DeleteCallbacksTo( const WgGizmo * pGizmo );
	int		DeleteCallbacksTo( const WgEventListener * pListener );
	int		DeleteCallbacksTo( void(*fp)( const WgEvent::Event * pEvent) );
	int		DeleteCallbacksTo( void(*fp)( const WgEvent::Event * pEvent, void * pParam) );

	int		DeleteCallbacksOn( const WgGizmo * pGizmo );
	int		DeleteCallbacksOn( const WgEventType type );
	int		DeleteCallbacksOn( const WgGizmo * pGizmo, WgEventType type );

	int		DeleteCallback( const WgEventFilter& filter, const WgGizmo * pGizmo );
	int		DeleteCallback( const WgEventFilter& filter, const WgEventListener * pListener );
	int		DeleteCallback( const WgEventFilter& filter, void(*fp)( const WgEvent::Event * pEvent) );
	int		DeleteCallback( const WgEventFilter& filter, void(*fp)( const WgEvent::Event * pEvent, void * pParam) );


	int		DeleteAllCallbacks();
	int		DeleteDeadCallbacks();


private:
	class	Callback;

	void 	_postTickEvents( int ticks );
	void 	_processEventQueue();


	void	_finalizeEvent( WgEvent::Event * pEvent );
	void	_processGeneralEvent( WgEvent::Event * pEvent );
	void	_processEventCallbacks( WgEvent::Event * pEvent );

	void	_processTick( WgEvent::Tick * pEvent );

	void	_processFocusGained( WgEvent::FocusGained * pEvent );
	void	_processFocusLost( WgEvent::FocusLost * pEvent );

	void	_processMouseEnter( WgEvent::MouseEnter * pEvent );
	void	_processMouseMove( WgEvent::MouseMove * pEvent );
	void	_processMousePosition( WgEvent::MousePosition * pEvent );
	void	_processMouseLeave( WgEvent::MouseLeave * pEvent );

	void	_processMouseButtonPress( WgEvent::MouseButtonPress * pEvent );
	void	_processMouseButtonRepeat( WgEvent::MouseButtonRepeat * pEvent );
	void	_processMouseButtonDrag( WgEvent::MouseButtonDrag * pEvent );
	void	_processMouseButtonRelease( WgEvent::MouseButtonRelease * pEvent );
	void	_processMouseButtonClick( WgEvent::MouseButtonClick * pEvent );
	void	_processMouseButtonDoubleClick( WgEvent::MouseButtonDoubleClick * pEvent );

	void	_processMouseWheelRoll( WgEvent::MouseWheelRoll * pEvent );

	void	_processKeyPress( WgEvent::KeyPress * pEvent );
	void	_processKeyRepeat( WgEvent::KeyRepeat * pEvent );
	void	_processKeyRelease( WgEvent::KeyRelease * pEvent );

	void	_processCharacter( WgEvent::Character * pEvent );

	bool	_isGizmoInList( const WgGizmo * pGizmo, const std::vector<WgGizmoWeakPtr>& list );

	void	_addCallback( const WgEventFilter& filter, Callback * pCallback );
	int		_deleteCallbacksTo( const void * pReceiver );
	int		_deleteCallbacksOnType( WgEventType type, WgChain<Callback> * pChain );
	int		_deleteCallback( const WgEventFilter& filter, const void * pReceiver );

	void 	_updateMarkedGizmos(bool bPostMouseMoveEvents);
	void	_addTickReceiver( WgGizmo * pGizmo );
	//

	WgRoot *		m_pRoot;

	std::deque<WgEvent::Event*>				m_eventQueue;
	bool									m_bIsProcessing;	// Set when we are inside ProcessEvents().
	std::deque<WgEvent::Event*>::iterator	m_insertPos;		// Position where we insert events being queued when processing.

	int64_t			m_time;
	WgCoord			m_pointerPos;
	WgModifierKeys	m_modKeys;

	std::vector<WgGizmoWeakPtr>	m_vTickGizmos;		// Gizmos that have requested periodic tick-events (i.e. on every processEvents() ).

	// Current mouse state

	std::vector<WgGizmoWeakPtr>	m_vMarkedGizmos;	// Gizmos the pointer currently is "inside". Empty if outside a modal gizmo.

	// Current button states

	bool						m_bButtonPressed[WG_MAX_BUTTONS+1];

	WgEvent::MouseButtonPress *		m_pLatestPressEvents[WG_MAX_BUTTONS+1];			// Saved info for the last time each button was pressed.
	WgEvent::MouseButtonRelease *	m_pLatestReleaseEvents[WG_MAX_BUTTONS+1];			// Saved info for the last time each button was released.

	std::vector<WgGizmoWeakPtr>	m_latestPressGizmos[WG_MAX_BUTTONS+1];		// List of gizmos who received the latest press, for each button.
	std::vector<WgGizmoWeakPtr>	m_previousPressGizmos[WG_MAX_BUTTONS+1];	// List of gizmos who received the second latest press, for each button,
																			// used for double-click handling.

	// Current keyboard state

	struct KeyDownInfo
	{
		WgEvent::KeyPress * 		pEvent;
		std::vector<WgGizmoWeakPtr>	vGizmos;				// Gizmos who received the press event.
	};

	std::vector<KeyDownInfo*>	m_keysDown;				// One entry for each currently depressed key, in order of being pressed.

	bool						m_bWindowFocus;			// Set if we have window focus.
	WgGizmoWeakPtr				m_keyFocusGroup;		// Current focus group (if any).
	WgGizmoWeakPtr				m_keyFocusGizmo;		// Gizmo currently having the keyboard focus.

	std::map<WgGizmoWeakPtr,WgGizmoWeakPtr>	m_focusGroupMap;	// Mapping focus groups (key) with their currently focused Gizmo (value).


	// Callbacks

	class Callback : public WgLink
	{
	public:
		virtual ~Callback() {};

		LINK_METHODS(Callback);

		virtual void 			ProcessEvent( const WgEvent::Event * pEvent ) = 0;
		virtual bool 			IsAlive() const = 0;
		virtual void * 			Receiver() const = 0;
		inline const WgEventFilter& 	Filter() const { return m_filter; }

	protected:
		WgEventFilter		m_filter;
	};


	class GizmoCallback : public Callback
	{
	public:
		GizmoCallback( const WgEventFilter& filter, void(*fp)(const WgEvent::Event * pEvent, WgGizmo * pDest), WgGizmo * pDest );

		void 	ProcessEvent( const WgEvent::Event * pEvent );
		bool 	IsAlive() const;
		void *	Receiver() const;

	private:
		void(*m_pFunction)(const WgEvent::Event * pEvent, WgGizmo * pDest);
		WgGizmoWeakPtr	m_pGizmo;		// Destination Gizmo, not source as in the event.
	};

	class FunctionCallbackParam : public Callback
	{
	public:
		FunctionCallbackParam( const WgEventFilter& filter, void(*fp)(const WgEvent::Event * pEvent, void * pParam), void * pParam );

		void 	ProcessEvent( const WgEvent::Event * pEvent );
		bool 	IsAlive() const;
		void *	Receiver() const;

	private:
		void(*m_pFunction)( const WgEvent::Event * pEvent, void * pParam);
		void *			m_pParam;
	};

	class FunctionCallback : public Callback
	{
	public:
		FunctionCallback( const WgEventFilter& filter, void(*fp)(const WgEvent::Event * pEvent) );

		void 	ProcessEvent( const WgEvent::Event * pEvent );
		bool 	IsAlive() const;
		void *	Receiver() const;

	private:
		void(*m_pFunction)( const WgEvent::Event * pEvent);
	};

	class ListenerCallback : public Callback
	{
	public:
		ListenerCallback( const WgEventFilter& filter, WgEventListener * pListener );

		void 	ProcessEvent( const WgEvent::Event * pEvent );
		bool 	IsAlive() const;
		void *	Receiver() const;

	private:
		WgEventListener *	m_pListener;
	};


	WgChain<Callback>						m_globalCallbacks;	// Callbacks called for every event.

	std::map<WgGizmoWeakPtr,WgChain<Callback> >	m_gizmoCallbacks;	// Callbacks for Gizmo-specific events.


};




#endif //WG_EVENTHANDLER_DOT_H
