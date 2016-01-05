#pragma once
#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "Base.h"

namespace pl
{

template<typename TPOD, TPOD V>
struct PODType
{
    static const TPOD Value = V;
    using Type = TPOD;
    using Self = PODType<TPOD, V>;
    //using Result = Self;
};

template<int VInt>
using Int = PODType<int, VInt>;

template<bool VBool>
using Bool = PODType<bool, VBool>;

template<char VChar>
using Char = PODType<char, VChar>;

using TrueType = Bool<true>;
using FalseType = Bool<false>;

template<class T>
struct Not;

template<>
struct Not<TrueType>
{
    using Result = FalseType;
};

template<>
struct Not<FalseType>
{
    using Result = TrueType;
};

template<typename TTest, typename TTrue, typename TFalse>
struct If;

template<typename TTrue, typename TFalse>
struct If<Bool<true>, TTrue, TFalse>
{
    using Result = TTrue;
};

template<typename TTrue, typename TFalse>
struct If<Bool<false>, TTrue, TFalse>
{
    using Result = TFalse;
};

namespace tml
{

#define TN typename
#define TT template
#define SS struct

#define DEFINE_BIN_META_OPERATION_HEAD(Name)	\
TT<TN T1, TN T2>								\
SS Name

#define DEFINE_BIN_META_OPERATION_BODY(Name, TType, VType, RType, op)	\
TT<VType V1, VType V2>													\
SS Name<TType<V1>, TType<V2>>											\
{																		\
    using Result = TN RType<(V1 op V2)>;								\
}

#define DEFINE_UNA_META_OPERATION_HEAD(Name)	\
TT<TN T1>										\
SS Name

#define DEFINE_UNA_META_OPERATION_BODY(Name, TType, VType, RType, op)	\
TT<VType V1>															\
SS Name<TType<V1>>														\
{																		\
    using Result = TN RType<(op V1)>;									\
}

DEFINE_BIN_META_OPERATION_HEAD(Add);
DEFINE_BIN_META_OPERATION_BODY(Add, Int, int, Int, +);
DEFINE_BIN_META_OPERATION_BODY(Add, Char, char, Char, +);
DEFINE_BIN_META_OPERATION_HEAD(Sub);
DEFINE_BIN_META_OPERATION_BODY(Sub, Int, int, Int, -);
DEFINE_BIN_META_OPERATION_BODY(Sub, Char, char, Char, -);
DEFINE_BIN_META_OPERATION_HEAD(Mul);
DEFINE_BIN_META_OPERATION_BODY(Mul, Int, int, Int, *);
DEFINE_BIN_META_OPERATION_BODY(Mul, Char, char, Char, *);
DEFINE_BIN_META_OPERATION_HEAD(Div);
DEFINE_BIN_META_OPERATION_BODY(Div, Int, int, Int, / );
DEFINE_BIN_META_OPERATION_BODY(Div, Char, char, Char, / );
DEFINE_BIN_META_OPERATION_HEAD(Mod);
DEFINE_BIN_META_OPERATION_BODY(Mod, Int, int, Int, %);
DEFINE_BIN_META_OPERATION_BODY(Mod, Char, char, Char, %);
DEFINE_BIN_META_OPERATION_HEAD(And);
DEFINE_BIN_META_OPERATION_BODY(And, Bool, bool, Bool, &&);
DEFINE_BIN_META_OPERATION_HEAD(Or);
DEFINE_BIN_META_OPERATION_BODY(Or, Bool, bool, Bool, || );
DEFINE_BIN_META_OPERATION_HEAD(Xor);
DEFINE_BIN_META_OPERATION_BODY(Xor, Bool, bool, Bool, ^);
DEFINE_BIN_META_OPERATION_HEAD(Eq);
DEFINE_BIN_META_OPERATION_BODY(Eq, Bool, bool, Bool, == );
DEFINE_BIN_META_OPERATION_BODY(Eq, Int, int, Bool, == );
DEFINE_BIN_META_OPERATION_BODY(Eq, Char, char, Bool, == );
DEFINE_BIN_META_OPERATION_HEAD(Neq);
DEFINE_BIN_META_OPERATION_BODY(Neq, Bool, bool, Bool, != );
DEFINE_BIN_META_OPERATION_BODY(Neq, Int, int, Bool, != );
DEFINE_BIN_META_OPERATION_BODY(Neq, Char, char, Bool, != );
DEFINE_BIN_META_OPERATION_HEAD(Gt);
DEFINE_BIN_META_OPERATION_BODY(Gt, Int, int, Bool, > );
DEFINE_BIN_META_OPERATION_BODY(Gt, Char, char, Bool, > );
DEFINE_BIN_META_OPERATION_HEAD(Lt);
DEFINE_BIN_META_OPERATION_BODY(Lt, Int, int, Bool, < );
DEFINE_BIN_META_OPERATION_BODY(Lt, Char, char, Bool, < );
DEFINE_BIN_META_OPERATION_HEAD(Ge);
DEFINE_BIN_META_OPERATION_BODY(Ge, Int, int, Bool, >= );
DEFINE_BIN_META_OPERATION_BODY(Ge, Char, char, Bool, >= );
DEFINE_BIN_META_OPERATION_HEAD(Le);
DEFINE_BIN_META_OPERATION_BODY(Le, Int, int, Bool, <= );
DEFINE_BIN_META_OPERATION_BODY(Le, Char, char, Bool, <= );
DEFINE_UNA_META_OPERATION_HEAD(Neg);
DEFINE_UNA_META_OPERATION_BODY(Neg, Int, int, Int, -);
DEFINE_UNA_META_OPERATION_BODY(Neg, Char, char, Char, -);
DEFINE_UNA_META_OPERATION_HEAD(Not);
DEFINE_UNA_META_OPERATION_BODY(Not, Bool, bool, Bool, !);

#undef TT
#undef TN
#undef SS

template<typename TTimes, template<typename TResult> typename TProc, typename TDefVal>
struct FOR_N;

template<template<typename TResult> typename TProc, typename TDefVal, int VTimes>
struct FOR_N<Int<VTimes>, TProc, TDefVal>
{
    using Result = typename TProc<
        typename FOR_N<
            Int<VTimes - 1>,
            TProc,
            TDefVal
        >::Result
    >::Result;
};

template<template<typename TResult> typename TProc, typename TDefVal>
struct FOR_N<Int<0>, TProc, TDefVal>
{
    using Result = typename TDefVal;
};

template<typename T>
struct ListGet;
template<typename... TArgs>
struct List;

template<typename T, typename... TArgs>
struct ListGet<List<T, TArgs...>>
{
    using First = typename T;
    using Rest = List<TArgs...>;
};

template<typename... TArgs>
struct List: ListGet<List<TArgs...>>
{
    using Base = ListGet<List<TArgs...>>;
    using Self = List<TArgs...>;
};

template<typename T>
struct ListIsEmpty;

template<typename T, typename... TArgs>
struct ListIsEmpty<List<T, TArgs...>>
{
    using Result = Bool<false>;
};
template<>
struct ListIsEmpty<List<>>
{
    using Result = Bool<true>;
};

template<typename A, template<typename... Args> typename B>
struct Change;

template<template<typename... Args>typename A, template<typename... Args>typename B, typename... Args>
struct Change<A<Args...>, B>
{
    using Result = B<Args...>;
};

template<typename... T>
struct ListConnect;


template<typename... TArgs1, typename... TArgs2>
struct ListConnect<List<TArgs1...>, List<TArgs2...>>
{
    using Result = typename List<TArgs1..., TArgs2...>;
};
template<typename... TArgs1, typename... TArgs2, typename... TArgs3>
struct ListConnect<List<TArgs1...>, List<TArgs2...>, List<TArgs3...>>
{
    using Result = typename List<TArgs1..., TArgs2..., TArgs3...>;
};

template<typename T>
struct ListReverse;

template<typename... TArgs>
struct ListReverse<List<TArgs...>>
{
    using Param = typename List<TArgs...>;
    using Result = typename ListConnect<
        typename ListReverse<
            typename Param::Rest
        >::Result,
        typename List<typename Param::First>
    >::Result;
};

template<typename T>
struct ListReverse<List<T>>
{
    using Result = typename List<T>;
};

template<template<typename ST> typename TApplicative, typename T1>
struct ListMap;

template<template<typename ST> typename TApplicative, typename... TArgs>
struct ListMap<TApplicative, List<TArgs...>>
{
    using Param = typename List<TArgs...>;
    using Result = typename ListConnect<
        List<
            typename TApplicative<
                typename Param::First
            >::Result
        >,
        typename ListMap<
            TApplicative,
            typename Param::Rest
        >::Result
    >::Result;
};

template<template<typename ST> typename TApplicative, typename TLast>
struct ListMap<TApplicative, List<TLast>>
{
    using Param = typename List<TLast>;
    using Result = typename List<typename TApplicative<TLast>::Result>;
};

template<typename TVal, template<typename TArg1, typename TArg2> typename TBinFunc, typename T>
struct ListFoldLeft;

template<typename TVal, template<typename TArg1, typename TArg2> typename TBinFunc, typename... TArgs>
struct ListFoldLeft<TVal, TBinFunc, List<TArgs...>>
{
    using Param = List<TArgs...>;
    using Result = typename ListFoldLeft<
        typename TBinFunc<
            TVal,
            typename Param::First
        >::Result,
        TBinFunc,
        typename Param::Rest		
    >::Result;
};

template<typename TVal, template<typename TArg1, typename TArg2> typename TBinFunc, typename TLast>
struct ListFoldLeft<TVal, TBinFunc, List<TLast>>
{
    using Param = List<TLast>;
    using Result = typename TBinFunc<TVal, typename Param::First>::Result;
};

template<typename TVal, template<typename TArg1, typename TArg2> typename TBinFunc, typename T>
struct ListScanLeft;

template<typename TVal, template<typename TArg1, typename TArg2> typename TBinFunc, typename... TArgs>
struct ListScanLeft<TVal, TBinFunc, List<TArgs...>>
{
    using Param = List<TArgs...>;
    using Value = typename TBinFunc<
        TVal,
        typename Param::First
    >::Result;
    using Result = typename ListConnect<
        List<Value>,
        typename ListScanLeft<
            Value,
            TBinFunc,
            typename Param::Rest
        >::Result
    >::Result;
};

template<typename TVal, template<typename TArg1, typename TArg2> typename TBinFunc, typename TLast>
struct ListScanLeft<TVal, TBinFunc, List<TLast>>
{
    using Param = List<TLast>;
    using Value = typename TBinFunc<
        TVal,
        typename Param::First
    >::Result;
    using Result = List<Value>;
};

template<template<typename TArg> typename TFilter, typename T>
struct ListFilter;

template<template<typename TArg> typename TFilter, typename... TArgs>
struct ListFilter<TFilter, List<TArgs...>>
{
    using Param = List<TArgs...>;
    using Result = typename ListConnect<
        typename If<
            typename TFilter<typename Param::First>::Result,
            List<typename Param::First>,
            List<>
        >::Result,
        typename ListFilter<
            TFilter, 
            typename Param::Rest
        >::Result
    >::Result;
};

template<template<typename TArg> typename TFilter, typename TLast>
struct ListFilter<TFilter, List<TLast>>
{
    using Param = List<TLast>;
    using Result = typename If<
        typename TFilter<typename Param::First>::Result,
        List<typename Param::First>,
        List<>
    >::Result;
};

template<typename T>
struct MapNone
{
    using Result = T;
};

template<typename T>
struct FilterNone
{
    using Result = Bool<true>;
};

template<template<typename TVal> typename TProcToVal, template<typename TVal> typename TFilterProc, typename... TSeq>
struct ListGeneration;

template<template<typename TVal> typename TProcToVal, template<typename TVal> typename TFilterProc, typename... TSeq>
struct ListGeneration<TProcToVal, TFilterProc, List<TSeq...>>
{
    using FilterResult = typename ListFilter<TFilterProc, List<TSeq...>>::Result;
    using Result = typename ListMap<TProcToVal, FilterResult>::Result;
};

template<typename TType, TType VBegin, TType VEnd, TType VCurr = VBegin>
struct Range
{
    using Result = typename ListConnect<
        List<PODType<TType, VCurr>>, 
        typename If<
            Bool<VCurr < VEnd>, 
            typename Range<TType, VBegin, VEnd, VCurr + 1>::Result,
            List<>
        >::Result
    >::Result;
};

template<typename TVal>
struct ListSort;

template<>
struct ListSort<List<>>
{
    using Result = List<>;
};

template<typename TVal>
struct ListSort<List<TVal>>
{
    using Result = List<TVal>;
};

template<typename... TArgs>
struct ListSort<List<TArgs...>>
{
    using Pivot = typename ListGet<List<TArgs...>>::First;
    template<typename TVal>
    struct FilterLt
    {
        using Result = typename Lt<TVal, Pivot>::Result;
    };
    template<typename TVal>
    struct FilterEq
    {
        using Result = typename Eq<TVal, Pivot>::Result;
    };
    template<typename TVal>
    struct FilterGt
    {
        using Result = typename Gt<TVal, Pivot>::Result;
    };
    using Left = typename ListFilter<FilterLt, List<TArgs...>>::Result;
    using Middle = typename ListFilter<FilterEq, List<TArgs...>>::Result;
    using Right = typename ListFilter<FilterGt, List<TArgs...>>::Result;
    using SortedLeft = typename ListSort<Left>::Result;
    using SortedRight = typename ListSort<Right>::Result;
    using Result = typename ListConnect<
        typename ListConnect<SortedLeft, Middle, SortedRight>::Result
    >::Result;
};


template<typename T, T V, typename... TRest>
void PrintList(List<PODType<T, V>, TRest...>)
{
    std::cout << V << ' ';
    PrintList(List<TRest...>());
}

template<typename T, T V>
void PrintList(List<PODType<T, V>>)
{
    std::cout << V;
}

}//namespace tml

#ifdef ENABLE_TYPE_ALIASES

template<class TValue, class TPtr, class TConstPtr, class TRef, class TConstRef, class TSize, class TDiff>
struct TypeAliases
{
    using ValueType = TValue;
    using Ptr = TPtr;
    using ConstPtr = TConstPtr;
    using Ref = TRef;
    using ConstRef = TConstRef;
    using SizeType = TSize;
    using DiffType = TDiff;
};

template<typename TValue>
struct BasicTypeAliases: TypeAliases<TValue, TValue*, const TValue*, TValue&, const TValue&, size_t, ptrdiff_t>
{};

#endif //ENABLE_TYPE_ALIASES

template<class T>
struct PtrTraits
{
};

template<class T>
struct PtrTraits<T*>
{
    using IsConst = FalseType;
    using IsVolatile = FalseType;
};

template<class T>
struct PtrTraits<const T*>
{
    using IsConst = TrueType;
    using IsVolatile = FalseType;
};

template<class T>
struct PtrTraits<volatile T*>
{
    using IsConst = FalseType;
    using IsVolatile = TrueType;
};

template<class T>
struct PtrTraits<const volatile T*>
{
    using IsConst = TrueType;
    using IsVolatile = TrueType;
};

struct EmptyType
{};

template<bool VTest, typename T = void>
struct EnableIf
{};

template<typename T>
struct EnableIf<true, T>
{
    using Type = typename T;
};

template<typename T1, typename T2>
struct TypeEq
{
    using Result = FalseType;
};

template<typename T>
struct TypeEq<T, T>
{
    using Result = TrueType;
};

template<typename T1, typename T2>
struct TypeNeq
{
    using Result = typename Not<
        typename TypeEq<T1, T2>::Result
    >::Result;
};

template<typename TEnum>
struct EnumBase
{
    using Type = __underlying_type(TEnum);
};

template<typename T>
struct RemoveConst
{
    using Type = T;
};

template<typename T>
struct RemoveConst<const T>
{
    using Type = T;
};

template<typename T>
struct RemoveConst<const T[]>
{
    using Type = T[];
};

template<typename T, size_t N>
struct RemoveConst<const T[N]>
{
    using Type = T[N];
};

template<class T>
struct RemoveReference
{
    using Type = T;
};

template<class T>
struct RemoveReference<T&>
{
    using Type = T;
};

template<class T>
struct RemoveReference<T&&>
{
    using Type = T;
};

template<class T>
struct IsLvalueReference: FalseType
{};

template<class T>
struct IsLvalueReference<T&> : TrueType
{};

template<class _Ty>
struct IsFunction
{
    using Result = FalseType;
};

template<class TRet, class... TArgs>
struct IsFunction<TRet(TArgs...)>
{
    using Result = TrueType;
};

template<class T>
struct IsConst
{
    using Result = class tml::And<
        class PtrTraits<T*>::IsConst,
        class tml::Not<
            class IsFunction<T>::Result
        >::Result
    >::Result;
};

template<class T, size_t N>
struct IsConst<T[N]>
{
    using Result = FalseType;
};

template<class T, size_t N>
struct IsConst<const T[N]>
{
    using Result = TrueType;
};

template<class T>
struct IsConst<T&>
{
    using Result = FalseType;
};

template<class T>
struct IsConst<T&&>
{
    using Result = FalseType;
};

template<class T>
struct IsInteger
{
    using Result = FalseType;
};
template<>
struct IsInteger<char>
{
    using Result = TrueType;
};
template<>
struct IsInteger<signed char>
{
    using Result = TrueType;
};
template<>
struct IsInteger<unsigned char>
{
    using Result = TrueType;
};

template<>
struct IsInteger<int>
{
    using Result = TrueType;
};

template<>
struct IsInteger<unsigned int>
{
    using Result = TrueType;
};

template<>
struct IsInteger<long>
{
    using Result = TrueType;
};

template<>
struct IsInteger<unsigned long>
{
    using Result = TrueType;
};

template<>
struct IsInteger<long long>
{
    using Result = TrueType;
};

template<>
struct IsInteger<unsigned long long>
{
    using Result = TrueType;
};

template<>
struct IsInteger<char16_t>
{
    using Result = TrueType;
};

template<>
struct IsInteger<char32_t>
{
    using Result = TrueType;
};

template<>
struct IsInteger<wchar_t>
{
    using Result = TrueType;
};

template<class T>
struct IsFloat
{
    using Result = FalseType;
};

template<>
struct IsFloat<float>
{
    using Result = TrueType;
};

template<>
struct IsFloat<double>
{
    using Result = TrueType;
};

template<>
struct IsFloat<long double>
{
    using Result = TrueType;
};

template<class T>
struct TypeEqFact
{
    template<class TTest>
    using Get = typename TypeEq<T, TTest>;
};

template<class T>
struct IsCString
{
    using Result = typename TypeEq<typename RemoveConst<T>::Type, char*>::Result;
};

template<class T> inline
T* _AddressOf(T& value, TrueType) noexcept
{
    return value;
}

template<class T> inline
T* _AddressOf(T& value, FalseType) noexcept
{
    return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(value)));
}

template<class T> inline
T* AddressOf(T& value) noexcept
{
    return _AddressOf(value, IsFunction<T>());
}

template<class T> inline constexpr 
typename RemoveReference<T>::Type&& RvalueCast(T&& value) noexcept
{
    return (static_cast<typename RemoveReference<T>::Type&&>(value));
}

template<class T> inline constexpr
T&& PerfectForward(typename RemoveReference<T>::Type& value) noexcept
{
    return static_cast<T&&>(value);
}

template<class T> inline constexpr
T&& PerfectForward(typename RemoveReference<T>::Type&& value) noexcept
{
    static_assert(!IsLvalueReference<T>::Value, "bad forward call");
    return (static_cast<T&&>(value));
}

template<int N>
using State = Int<N>;

template<int N, class TStatement, template<class T> class TPred, template<class T> class... TPreds>
struct MultiStateImpl
{
    using Result = typename If<
        typename TPred<TStatement>::Result,
        State<N>,
        typename MultiStateImpl<
            N + 1,
            TStatement,
            TPreds...
        >::Result
    >::Result;
};

template<int N, class TStatement, template<class T> class TPred>
struct MultiStateImpl<N, TStatement, TPred>
{
    using Result = typename If<
        typename TPred<TStatement>::Result,
        State<N>,
        FalseType
    >::Result;
};

template<class TStatement, template<class T> class... TPreds>
struct MultiState
{
    using Result = typename MultiStateImpl<0, TStatement, TPreds...>::Result;
};

template<template<class T> class... TPreds>
struct PredOrFact
{
    template<class TStatement>
    using Get = typename TypeNeq<
        typename MultiState<TStatement, TPreds...>::Result,
        FalseType
    >;
};

struct POD
{};
struct NotPOD
{};

template<typename T>
struct IsPOD: Bool<__is_pod(T)>
{
    using Result = typename If<Self, POD, NotPOD>::Result;
};


/*SFINAE*/

//template<typename T>
//struct IsClass
//{
//private:
//	template<typename TClass> static Bool<true> test(int TClass::*) {}
//	template<typename TClass> static Bool<false> test(...) {}
//public:
//	using Result = decltype(test<T>(0));
//};


}

#endif // !TEMPLATE_H
