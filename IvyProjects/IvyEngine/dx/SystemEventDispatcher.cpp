///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#include "SystemEventDispatcher.h"
#include "IvyUtils.h"

SystemEventDispatcher* SystemEventDispatcher::Get()
{
    static SystemEventDispatcher dispatcher;

    return &dispatcher;
}


SystemEventDispatcher::SystemEventDispatcher()
{

}

SystemEventDispatcher::~SystemEventDispatcher()
{

}

void SystemEventDispatcher::RegisterReceiver(
    IEventReceiver* pReceiver)
{
    ///@todo Assert if item is intersted multiple times
    m_receivers.push_back(pReceiver);
}

void SystemEventDispatcher::UnregisterReceiver(
    IEventReceiver* pReceiver)
{

}

void SystemEventDispatcher::Dispatch(
    const Event* pEvent)
{
    std::list<IEventReceiver*>::iterator iter = m_receivers.begin();

    while (iter != m_receivers.end())
    {
        (*iter)->ReceiveEvent(pEvent);
        iter++;
    }
}


