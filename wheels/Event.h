#pragma once
#ifndef EVENT_H
#define EVENT_H

#include "Base.h"
#include "Pointer.h"
#include "Function.h"
#include <vector>

#define DEFINE_VOID_EVENT(EventName, args, TArgs) using EventName = Event<TArgs>;
#define IMPLEMENT_VOID_EVENT(a, b, c)

namespace pl
{

template<typename... TArgs>
using EventHandler = Callable<void(TArgs...)>;

template<typename... TArgs>
class Event: public Object, private NotCopyable
{
protected:
    std::vector<Callable<void(TArgs...)>> handlers;
public:
    using Handler = Callable<void(TArgs...)>;

    friend class Handler;

    Event()
        : handlers()
    {
    }
    template<class TClass>
    void Bind(TClass* object, void(TClass::*function)(TArgs...))
    {
        this->operator+=(Handler(object, function));
    }
    template<class TCallable>
    void Bind(TCallable& callable)
    {
        this->operator+=(Handler(callable));
    }
    void Bind(void(*function)(TArgs...))
    {
        this->operator+=(Handler(function));
    }
    template<class TClass>
    void Unbind(TClass* object, void(TClass::*function)(TArgs...))
    {
        this->operator-=(Handler(object, function));
    }
    template<class TCallable>
    void Unbind(TCallable& callable)
    {
        this->operator-=(Handler(callable));
    }
    void Unbind(void(*function)(TArgs...))
    {
        this->operator-=(Handler(function));
    }
    Event& operator+=(Handler& handler)
    {
        if (std::find(handlers.begin(), handlers.end(), handler) == handlers.end())
        {
            handlers.push_back(handler);
        }
        return *this;
    }
    Event& operator-=(Handler& handler)
    {
        auto place = std::find(handlers.begin(), handlers.end(), handler);
        if (place != handlers.end())
        {
            handlers.erase(place);
        }
        return *this;
    }
    void operator()(TArgs... args) const
    {
        for (auto&& handler : handlers)
        {
            handler(PerfectForward<TArgs>(args)...);
        }
    }
};

}
#endif // !EVENT_H
