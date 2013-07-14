///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2013, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IVYEVENTS_H_
#define _IVYEVENTS_H_

#include "IvyInput.h"

enum EventType
{
    EventTypeWindowResize,
    EventTypeMouseButtonDown,
    EventTypeMouseButtonUp,
    EventTypeMouseDoubleClick,
    EventTypeMouseMove,
    EventTypeKeyDown,
    EventTypeKeyUp
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/// Event Class
///////////////////////////////////////////////////////////////////////////////////////////////////
class Event
{
public:
    virtual ~Event() {}

    EventType GetType() const { return m_type; }

protected:
    Event(EventType type) { m_type = type; }

private:
    EventType m_type;
};


template <EventType et> class TEventNoData : public Event
{
public:
    TEventNoData() : Event(et) { }
    virtual ~TEventNoData() { }
};

template <EventType et, typename T> class TEventWithData : public Event
{
public:
    TEventWithData(T data) : Event(et) { m_data = data; }
    virtual ~TEventWithData() { }

    T GetData() const { return m_data; }

private:
    T m_data;
};

class IEventReceiver
{
public:
    virtual void ReceiveEvent(const Event* pEvent) = 0;

protected:
    IEventReceiver(){};
    virtual ~IEventReceiver(){};
};


struct EventDataKey
{
    KeyboardKey key;
};

struct EventDataMouseButton
{
    MouseButton button;
};

typedef TEventNoData<EventTypeWindowResize> EventWindowResize;
typedef TEventNoData<EventTypeMouseMove> EventMouseMove;
typedef TEventWithData<EventTypeMouseButtonDown, EventDataMouseButton> EventMouseButtonDown;
typedef TEventWithData<EventTypeMouseButtonUp, EventDataMouseButton> EventMouseButtonUp;
typedef TEventWithData<EventTypeMouseDoubleClick, EventDataMouseButton> EventMouseDoubleClick;
typedef TEventWithData<EventTypeKeyDown, EventDataKey> EventKeyDown;
typedef TEventWithData<EventTypeKeyUp, EventDataKey> EventKeyUp;


#endif // _IVYEVENTS_H_

