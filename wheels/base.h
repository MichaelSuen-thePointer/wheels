#ifndef BASE_H
#define BASE_H

namespace pl
{

class Object
{
public:
    ~Object() {}
};

class NotCopyable
{
protected:
    NotCopyable() {}
    ~NotCopyable() {}
private:
    NotCopyable(const NotCopyable&) = delete;
    NotCopyable(NotCopyable&&) = delete;
    NotCopyable& operator=(const NotCopyable&) = delete;
    NotCopyable& operator=(NotCopyable&&) = delete;
};

class Interface: NotCopyable
{
public:
    virtual ~Interface() {}
};


template <typename T>
inline
T Max(T& a, T& b)
{
    return a > b ? a : b;
}

template <typename T>
inline
T Min(T& a, T& b)
{
    return a < b ? a : b;
}


}
#endif // !BASE_H
