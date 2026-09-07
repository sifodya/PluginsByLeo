//
// Created by vboxuser on 03.09.2026.
//

#pragma once
#include <vector>
#include <type_traits>

class LeoEventManager;

class LeoEvent
{
    public:
    virtual void trigger() = 0;
};
class LeoListener
{
    public:
    virtual ~LeoListener() = default;
    virtual void onEvent() = 0;
};
class LeoEventManager
{
    std::vector<LeoListener*> listeners;
    public:
    void addListener(LeoListener* listener)
    {
        listeners.push_back(listener);
        DBG("Pushed Back Listener");
    }

    void notifyListeners()
    {
        for (const auto listener : listeners)
            listener->onEvent();
        DBG("notifyListeners");
    }
};

template <typename T1>
class LeoActionEvent
{
    //LeoActionEvent::LeoActionEvent() = default;
    //TODO convert vector to map
    //std::map<std::hash,std::function> map;
    std::vector<std::function<void(T1)>> subscriptions;
    public:
    void invoke(T1 input)
    {
        for (const auto sub : subscriptions)
            sub(input);
    }
    void operator += (std::function<void(T1)> rhs)
    {
        subscriptions.push_back(rhs);
    }

    void operator -= (std::function<void(T1)> rhs)
    {
        subscriptions.erase(subscriptions.begin(), subscriptions.end());
    }
};

class LeoVoidActionEvent
{
    std::vector<std::function<void()>> subscribers;
    std::unordered_map<std::string, std::function<void()>> subscriptions;
    //std::hash<Key> hash;
public:
    void invoke()
    {
        for (const auto sub : subscribers)
            sub();
    }

    void operator += (std::function<void()> rhs)
    {
        subscribers.push_back(rhs);
    }

    void operator -= (std::function<void()> rhs)
    {
        subscribers.erase(subscribers.begin(), subscribers.end());
    }
};

template <typename T>
class LeoActionEventTest
{
public:
    LeoActionEventTest()
    {
        if (std::is_void_v<T>)
        {
            static_cast<LeoVoidActionEvent>(*this);
            //static_cast<LeoVoidActionEvent>(*this);
        }
        else
        {
            reinterpret_cast<LeoActionEvent<T>>(*this);
            //static_cast<LeoActionEvent<T>>(*this);
        }
    }
};
