#pragma once
#include "BitSet.h"
#include "Tuple.h"
#include "Optional.h"
#include "Utils/TypeTraits.h"

namespace ECS
{
	struct ComponentAlreadyExistsExeption : std::exception {};
	struct ComponentDoesNotExistExeption : std::exception {};

	template<typename... TComponents>
	class EntityData
	{
	private:
		static constexpr size_t s_NumComponents = sizeof...(TComponents);

	public:
		template<typename... TArgs>
		bool HasComponents() const
		{
			constexpr size_t argCount = sizeof...(TArgs);
			constexpr size_t indexes[argCount] = { IndexOf<TArgs>()... };

			BitSet<s_NumComponents> checker;
			for (int i = 0; i < argCount; i++)
			{
				checker.Set(indexes[i], true);
			}

			return HasComponentsInBitSet(checker);
		}

		bool HasComponentsInBitSet(BitSet<s_NumComponents> bitset) const
		{
			return (m_ComponentFlags & bitset) == bitset;
		}

		BitSet<s_NumComponents> GetBitSet() const
		{
			return m_ComponentFlags;
		}

		template<typename T, typename... TArgs>
		T& AddComponent(TArgs&&... args)
		{
			constexpr size_t index = IndexOf<T>();
			if (!m_Components.Get<index>().HasValue())
			{
				m_Components.Get<index>() = std::move(T(std::forward<TArgs>(args)...));
				m_ComponentFlags.Set(index, true);
				return m_Components.Get<index>().Get();
			}
			else
			{
				throw ComponentAlreadyExistsExeption();
			}
		}

		template<typename T>
		T& GetComponent()
		{
			constexpr size_t index = IndexOf<T>();
			if (m_Components.Get<index>().HasValue())
			{
				return m_Components.Get<index>().Get();
			}
			else
			{
				throw ComponentDoesNotExistExeption();
			}
		}

	private:
		template<typename T>
		static constexpr size_t IndexOf()
		{
			static_assert(Utils::ExistsInList<T, TComponents...>(), "Component must exist in components list");
			return Utils::template IndexOf<T>::template InList<TComponents...>;
		}

	private:
		BitSet<s_NumComponents> m_ComponentFlags;
		Tuple<Optional<TComponents>...> m_Components;
	};
}