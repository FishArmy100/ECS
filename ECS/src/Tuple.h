#pragma once
#include "Utils/TypeTraits.h"
#include <tuple>

namespace ECS
{
    template<typename...TArgs>
    struct TupleLeaf
    {

    };

    template<typename T>
    struct TupleLeaf<T>
    {
        template<typename TArg>
        TupleLeaf(TArg&& v) : Value(std::forward<TArg>(v)) {}
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
        template<typename... CtorArgs>
        Tuple(CtorArgs&&... args) : TupleLeaf<Args...>(std::forward<CtorArgs>(args)...) {}

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

        template <size_t index>
        auto& get()& {
            return static_cast<typename TupleLeafGetter<index, Args...>::LeafType*>(this)->Value;
        }

        template <size_t index>
        auto const& get() const& {
            return static_cast<typename TupleLeafGetter<index, Args...>::LeafType*>(this)->Value;
        }

        template <size_t index>
        auto&& get()&& {
            return std::move(static_cast<typename TupleLeafGetter<index, Args...>::LeafType*>(this)->Value);
        }
    };
}

namespace std
{
    template<typename... TTypes>
    struct tuple_size<ECS::Tuple<TTypes...>> : std::integral_constant<size_t, sizeof...(TTypes)> {};

    template<size_t index, typename... TTypes>
    struct tuple_element<index, ECS::Tuple<TTypes...>> { using type = ECS::Utils::TypeListIndexer<index, TTypes...>::Type; };
}