#pragma once
#include "Utils.h"

namespace ECS
{
    template<typename...TArgs>
    struct TupleLeaf
    {

    };

    template<typename T>
    struct TupleLeaf<T>
    {
        TupleLeaf(T&& v) : Value(v) {}
        TupleLeaf() = default;

        T Value;
    };

    template<typename TFirst, typename TSecond, typename... TRest>
    struct TupleLeaf<TFirst, TSecond, TRest...> : TupleLeaf<TSecond, TRest...>
    {
        TupleLeaf(TFirst&& first, TSecond&& second, TRest&&... rest) :
            TupleLeaf<TSecond, TRest...>(std::forward<TSecond>(second), std::forward<TRest>(rest)...),
            Value(first) {}

        TupleLeaf() = default;

        TFirst Value;
    };

    template<size_t index, typename TFirst, typename... TArgs>
    struct TupleLeafGetter
    {
        using LeafType = typename TupleLeafGetter<index - 1, TArgs...>::LeafType;
    };

    template<typename TFirst, typename... TArgs>
    struct TupleLeafGetter<0, TFirst, TArgs...>
    {
        using LeafType = TupleLeaf<TFirst, TArgs...>;
    };


    template<typename... Args>
    class Tuple : TupleLeaf<Args...>
    {
    public:
        Tuple(Args&&... args) : TupleLeaf<Args...>(std::forward<Args>(args)...) {}

        Tuple() = default;
        Tuple(const Tuple&) = default;
        Tuple& operator=(const Tuple&) = default;
        Tuple(Tuple&&) = default;
        Tuple& operator=(Tuple&&) = default;

        template<size_t index>
        auto& Get()
        {
            return static_cast<typename TupleLeafGetter<index, Args...>::LeafType*>(this)->Value;
        }
    };
}