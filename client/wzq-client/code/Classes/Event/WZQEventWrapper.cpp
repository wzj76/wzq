//
//  PHZEventWrapper.cpp
//  GouJi
//
//  Created by 曾琪 on 14-9-26.
//
//

#include "WZQEventWrapper.h"

USING_NS_CC;
using namespace std;

WZQEventWrapper::WZQEventWrapper()
:
m_pEventDispatcher(nullptr)
{
    m_pEventDispatcher = Director::getInstance()->getEventDispatcher();
    CC_SAFE_RETAIN(m_pEventDispatcher);
}

WZQEventWrapper::~WZQEventWrapper()
{
    removeAllEventListeners();
    CC_SAFE_RELEASE_NULL(m_pEventDispatcher);
}

void WZQEventWrapper::dispatchEvent(const string &sEventName, void *pUserData)
{
    if (m_pEventDispatcher) {
        m_pEventDispatcher->dispatchCustomEvent(sEventName, pUserData);
    }
}

void WZQEventWrapper::addEventListener(const string &sEventName, const function<void (EventCustom *)> &fnEventCallback)
{
    removeEventListener(sEventName);
    
    auto pListener = m_pEventDispatcher->addCustomEventListener(sEventName, fnEventCallback);
    m_obListenerMap.insert(sEventName, pListener);
}

void WZQEventWrapper::removeEventListener(const string &sEventName)
{
    auto iter = m_obListenerMap.find(sEventName);
    if (iter != m_obListenerMap.end())
    {
        m_pEventDispatcher->removeEventListener(iter->second);
        m_obListenerMap.erase(iter);
    }
    
}

void WZQEventWrapper::removeAllEventListeners()
{
    auto vKeys = m_obListenerMap.keys();
    for (auto i = 0; i < vKeys.size(); i++)
    {
        removeEventListener(vKeys[i]);
    }
}
