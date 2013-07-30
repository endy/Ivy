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
    static SystemEventDispatcher* pDispatcher = NULL;

    if (pDispatcher == NULL)
    {
        pDispatcher = new SystemEventDispatcher();
    }

    return pDispatcher;
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
    m_receivers.remove(pReceiver);
    m_receivers.push_back(pReceiver);
}

void SystemEventDispatcher::UnregisterReceiver(
    IEventReceiver* pReceiver)
{
    m_receivers.remove(pReceiver);
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


