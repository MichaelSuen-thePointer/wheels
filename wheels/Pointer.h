#pragma once
#ifndef POINTER_H
#define POINTER_H

#include "Template.h"

namespace pl
{
template <typename T>
class Deleter
{
public:
    inline static void Default(T* pointer)
    {
        delete pointer;
    }
    inline static void Array(T* array)
    {
        delete[] array;
    }
};
template <typename T>
class SharedPtr
{
    template<typename TOther>
    friend class SharedPtr;
protected:
    using DeleterType = void(*)(T*);

    volatile size_t* _counter;
    T* _object;
    DeleterType _deleter;

    void RefInc()
    {
        if (_counter)
        {
            (*_counter)++;
        }
    }

    void RefDec()
    {
        if (_counter)
        {
            (*_counter)--;
            if (*_counter == 0)
            {
                _deleter(_object);
                delete _counter;
            }
        }
    }
public:
    SharedPtr()
        : _counter(nullptr)
        , _object(nullptr)
        , _deleter(nullptr)
    {
    }

    SharedPtr(T* pointer, DeleterType deleter = &(Deleter<T>::Default))
        : _counter(new size_t(0))
        , _object(pointer)
        , _deleter(deleter)
    {
        RefInc();
    }

    SharedPtr(const SharedPtr<T>& pointer)
        : _counter(pointer._counter)
        , _object(pointer._object)
        , _deleter(pointer._deleter)
    {
        RefInc();
    }

    SharedPtr(SharedPtr<T>&& pointer)
        : _counter(pointer._counter)
        , _object(pointer._object)
        , _deleter(pointer._deleter)
    {
        pointer._counter = nullptr;
        pointer._object = nullptr;
        pointer._deleter = nullptr;
    }
    template<typename C>
    SharedPtr(const SharedPtr<C>& pointer)
        : _counter(nullptr)
        , _object(nullptr)
        , _deleter(nullptr)
    {
        T* converted = pointer.Get();
        if (converted)
        {
            _counter = pointer._counter;
            _object = converted;
            _deleter = pointer._deleter;
            RefInc();
        }
    }
    SharedPtr::~SharedPtr()
    {
        RefDec();
    }

    SharedPtr& operator=(const SharedPtr<T>& pointer)
    {
        if (this != &pointer)
        {
            RefDec();
            _object = pointer._object;
            _counter = pointer._counter;
            _deleter = pointer._deleter;
            RefInc();
        }
        return *this;
    }
    SharedPtr& operator=(SharedPtr<T>&& pointer)
    {
        if (this != &pointer)
        {
            RefDec();
            _object = pointer._object;
            _counter = pointer._counter;
            _deleter = pointer._deleter;

            pointer._object = nullptr;
            pointer._counter = nullptr;
            pointer._deleter = nullptr;
        }
        return *this;
    }
    template<typename C>
    SharedPtr<T>& operator=(const SharedPtr<C>& pointer)
    {
        T* converted = pointer.Get();
        RefDec();
        if (converted)
        {
            _counter = pointer._counter;
            _object = pointer._object;
            _deleter = pointer._deleter;
            RefInc();
        }
        else
        {
            _counter = nullptr;
            _object = nullptr;
            _deleter = nullptr;
        }
        return *this;
    }
    bool operator==(const T* pointer)const { return _object == pointer; }
    bool operator!=(const T* pointer)const { return _object != pointer; }
    bool operator>(const T* pointer)const { return _object > pointer; }
    bool operator>=(const T* pointer)const { return _object >= pointer; }
    bool operator<(const T* pointer)const { return _object < pointer; }
    bool operator<=(const T* pointer)const { return _object <= pointer; }

    bool operator==(const SharedPtr<T>& pointer)const { return _object == pointer._object; }
    bool operator!=(const SharedPtr<T>& pointer)const { return _object != pointer._object; }
    bool operator>(const SharedPtr<T>& pointer)const { return _object > pointer._object; }
    bool operator>=(const SharedPtr<T>& pointer)const { return _object >= pointer._object; }
    bool operator<(const SharedPtr<T>& pointer)const { return _object < pointer._object; }
    bool operator<=(const SharedPtr<T>& pointer)const { return _object <= pointer._object; }

    T* Get() const
    {
        return _object;
    }

    void Reset(T* pointer, DeleterType deleter = &(Deleter<T>::Default))
    {
        RefDec();
        _counter = new size_t(0);
        _object = pointer;
        _deleter = deleter;
        RefInc();
    }

    explicit operator bool() const
    {
        return _object != nullptr;
    }

    T& operator*(void) const
    {
        return *_object;
    }

    T* operator->(void) const
    {
        return _object;
    }
};

template<typename T, typename... TArgs>
SharedPtr<T> MakeShared(TArgs&&... args)
{
    return new T(PerfectForward<TArgs>(args)...);
}

}

#endif // !POINTER_H
