#pragma once
#include "BitSet.h"
#include "Tuple.h"
#include "Optional.h"
#include "Utils/TypeTraits.h"

namespace ECS
{
	template<typename... TComponents>
	class EnityData
	{
	public:
		template<typename... TArgs>
		bool HasComponent()
		{
			return (HasComponentInternal<TArgs>() && ...);
		}

	private:
		template<typename T>
		static constexpr size_t IndexOf()
		{
			static_assert(Utils::ExistsInList<T, TComponents...>(), "Component must exist in components list");
			return Utils::IndexOf<T>::InList<TComponents...>;
		}

		template<typename T>
		bool HasComponentInternal()
		{
			return m_Components.Get<IndexOf<T>()>().HasValue();
		}

	private:
		static constexpr size_t s_NumComponents = sizeof...(TComponents);
		BitSet<s_NumComponents> m_ComponentFlags;
		Tuple<Optional<TComponents>...> m_Components;
	};
}