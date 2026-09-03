//
// Created by vboxuser on 03.09.2026.
//

#pragma once
#include <vector>

class LeoEventManager;

class LeoEvent
{
    public:
    virtual void trigger() = 0;
    virtual void triggerManager(LeoEventManager& manager) = 0;
};
class LeoListener
{
    public:
    virtual void onEvent() = 0;
    //virtual void onEvent(LeoEvent& e) = 0;
};
class LeoEventManager
{
    std::vector<LeoListener*> listeners;
    public:
    void addListener(LeoListener* listener)
    {
        listeners.push_back(listener);
    }

    void notifyListeners()
    {
        for (const auto listener : listeners)
            listener->onEvent();
    }
};
