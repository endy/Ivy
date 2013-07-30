///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSTEMEVENTDISPATCHER_H_
#define _SYSTEMEVENTDISPATCHER_H_

#include <list>
#include "IvyInput.h"

/****
TODO
1. How to pass event data?
2. OO crazy: Event type enum + Typed receiver + Typed Sender + Dispatcher?
    -- Typing everything and using interfaces per event type saves from a single blobby dispatcher
       but is probably way too verbose and inflexible 

3. What about threading?
   -Critical section for initalizing the sub

Simple design:

  App pumps the platform msg queue, queuing up events in an internal buffer
  App then polls the system dispatcher itself to grab all events across all systems and handles them

****/

#include "IvyEvents.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// SystemEventDispatcher Class
///////////////////////////////////////////////////////////////////////////////////////////////////
class SystemEventDispatcher
{
public:
    static SystemEventDispatcher* Get();

    virtual ~SystemEventDispatcher();

    void RegisterReceiver(IEventReceiver* pReceiver);
    void UnregisterReceiver(IEventReceiver* pReceiver);

    void Dispatch(const Event* pEvent);

private:
    SystemEventDispatcher();

    std::list<IEventReceiver*> m_receivers;
};

#endif // _SYSTEMEVENTDISPATCHER_H_
