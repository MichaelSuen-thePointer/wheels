#pragma once

#ifndef FUNCTION_H
#define FUNCTION_H
#include "Pointer.h"

namespace pl
{
namespace invoker_impl
{

template<typename TRet, typename... TArgs>
class Invoker: public Object
{
public:
    virtual TRet operator()(TArgs&&... args) = 0;
};

template<typename TRet, typename... TArgs>
class NormalInvoker: public Invoker<TRet, TArgs...>
{
    using FunctionType = TRet(*)(TArgs...);
private:
    FunctionType _function;
public:
    NormalInvoker(FunctionType function)
        : _function(function)
    {}

    virtual TRet operator()(TArgs&&... args) override
    {
        return _function(PerfectForward<TArgs>(args)...);
    }
};

template<typename TClass, typename TRet, typename... TArgs>
class MemberInvoker: public Invoker<TRet, TArgs...>
{
    using FunctionType = TRet(TClass::*)(TArgs...);
private:
    TClass* _object;
    FunctionType _function;
public:
    MemberInvoker(TClass* object, FunctionType function)
        : _object(object)
        , _function(function)
    {}
    virtual TRet operator()(TArgs&&... args) override
    {
        return (_object->*_function)(PerfectForward<TArgs>(args)...);
    }
};

template<typename TCallable, typename TRet, typename... TArgs>
class CallableInvoker: public Invoker<TRet, TArgs...>
{
    using FunctionType = TRet(*)(TArgs...);
private:
    TCallable _object;
public:
    CallableInvoker(const TCallable& object)
        : _object(object)
    {}
    virtual TRet operator()(TArgs&&... args) override
    {
        return _object(PerfectForward<TArgs>(args)...);
    }
};

} //namespace invoker_impl

template<typename T>
class Callable
{};

template<typename TRet, typename... TArgs>
class Callable<TRet(TArgs...)>: public Object
{
    using CallableType = TRet(*)(TArgs...);
protected:
    SharedPtr<invoker_impl::Invoker<TRet, TArgs...>> _invoker;
public:
    Callable(TRet(*function)(TArgs...))
        : _invoker(new invoker_impl::NormalInvoker<TRet, TArgs...>(function))
    {}

    template<typename TClass>
    Callable(TClass* object, TRet(TClass::*function)(TArgs...))
        : _invoker(new invoker_impl::MemberInvoker<TClass, TRet, TArgs...>(object, function))
    {}

    template<typename TCallable>
    Callable(TCallable& object)
        : _invoker(new invoker_impl::CallableInvoker<TCallable, TRet, TArgs...>(object))
    {}

    Callable(const Callable& callable)
        : _invoker(callable._invoker)
    {}

    TRet operator()(TArgs&&... args) const
    {
        return (*_invoker)(PerfectForward<TArgs>(args)...);
    }

    bool operator==(const Callable& callable)
    {
        return _invoker == callable._invoker;
    }

    bool operator!=(const Callable& callable)
    {
        return _invoker != callable._invoker;
    }
};

}

#endif // !FUNCTION_H
