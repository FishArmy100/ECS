#pragma once
#include "EntityData.h"
#include "Optional.h"
#include "List.h"

namespace ECS
{
	template<typename... TComponents>
	class EntityDataList
	{
	public:
		static constexpr size_t NumberOfComponents() { return sizeof...(TComponents); }
		using EntityFlags = Optional<BitSet<NumberOfComponents()>>();
		using EntityComponentData = Optional<EntityData<TComponents...>>;
	private:
		List<EntityFlags> m_Entities;
		List<EntityComponentData> m_ComponentDatas;
	};
}