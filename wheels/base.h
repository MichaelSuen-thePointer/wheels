#ifndef BASE_H
#define BASE_H

namespace pl
{

class UnCopyable
{
public:
	UnCopyable() = default;
	virtual ~UnCopyable() = default;

	UnCopyable(const UnCopyable&) = delete;
	UnCopyable(UnCopyable&&) = delete;
	UnCopyable& operator=(const UnCopyable&) = delete;
	UnCopyable& operator=(UnCopyable&&) = delete;
};

class Object
{
public:
	Object() = default;
	virtual ~Object() = default;
};

class Interface
{
public:
	Interface() = default;
	virtual ~Interface() = default;
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
