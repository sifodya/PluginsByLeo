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

template <typename T>
class LeoActionEvent
{
    LeoActionEvent::LeoActionEvent() = default;
    //TODO convert vector to map
    std::vector<std::function<void(T)>> subscribers;
    public:
    void invoke(T input)
    {
        for (const auto sub : subscribers)
            sub(input);
    }

    void operator += (std::function<void(T)> rhs)
    {
        subscribers.push_back(rhs);
    }

    void operator -= (std::function<void(T)> rhs)
    {
        subscribers.erase(subscribers.begin(), subscribers.end());
    }
};
