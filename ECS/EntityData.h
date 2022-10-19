#pragma once
#include "BitSet.h"
#include "Tuple.h"
#include "Optional.h"

namespace ECS
{
	template<typename... TComponents>
	class EnityData
	{
	public:


	private:
		static constexpr size_t s_NumComponents = sizeof...(TComponents);
		BitSet<s_NumComponents> m_ComponentFlags;
		Tuple<Optional<TComponents>...> m_Components;
	};
}