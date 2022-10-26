#pragma once
#include "Optional.h"
#include "List.h"
#include "Utils/Pair.h"
#include "Utils/Ref.h"
#include "EntityId.h"
#include "Utils/TypeTraits.h"

namespace ECS
{
	struct NullEntityException : public std::exception {};
	struct EntityAlreadyHasComponentException : public std::exception {};
	struct EntityDoesNotHaveComponentException : public std::exception {};

	template<typename... TComponents>
	class EntityDataList
	{
	public:
		static constexpr size_t NumberOfComponents() { return sizeof...(TComponents); }
	private:
		using ComponentFlags = BitSet<NumberOfComponents()>;
		using EntityFlags = Optional<ComponentFlags>;
		using TupleType = Tuple<Optional<TComponents>...>;
		using EntityData = Optional<Tuple<Optional<TComponents>...>>;
	public:
		EntityDataList() = default;
		EntityDataList(const EntityDataList&) = delete;
		EntityDataList& operator=(const EntityDataList&) = delete;

	public:
		EntityId CreateEntity()
		{
			EntityId id = GetFirstInvalid();
			m_ComponentDatas[id] = { TupleType{} };
			m_Entities[id] = { ComponentFlags{} };
			return id;
		}

		template<typename T, typename... TArgs> 
		T& AddComponent(EntityId e, TArgs&&... args)
		{
			if (!HasComponent<T>(e))
			{
				EntityData& data = m_ComponentDatas[e];
				data.Get().Get<IndexOf<T>()>() = T(std::forward<TArgs>(args)...);
				m_Entities[e].Get().Set(IndexOf<T>(), true);
				return data.Get().Get<IndexOf<T>()>().Get();
			}
			else
			{
				throw EntityAlreadyHasComponentException();
			}
		}

		template<typename T>
		T& GetComponent(EntityId e)
		{
			if (HasComponent<T>(e))
			{
				return m_ComponentDatas[e].Get().Get<IndexOf<T>()>().Get();
			}
			else
			{
				throw EntityDoesNotHaveComponentException();
			}
		}

		template<typename T>
		bool HasComponent(EntityId e)
		{
			EntityFlags flags = m_Entities[e];
			if (flags.HasValue())
			{
				ComponentFlags checker{};
				checker.Set(IndexOf<T>(), true);
				return (flags.Get() & checker).Any();
			}
			else
			{
				throw NullEntityException();
			}
		}

		template<typename T>
		void RemoveComponent(EntityId e) 
		{
			if (HasComponent<T>(e))
			{
				m_ComponentDatas[e].Get().Get<IndexOf<T>()>().Get().~T(); 
				m_Entities[e].Get().Set(IndexOf<T>(), false);
			}
			else
			{
				throw EntityDoesNotHaveComponentException();
			}
		}

	private:
		EntityId GetFirstInvalid()
		{
			size_t index = 0;
			for (EntityFlags& flags : m_Entities)
			{
				if (!flags.HasValue())
					return index;
			}

			// no empty spots
			return ConstructInvalidEntity();
		}

		template<typename T>
		static constexpr size_t IndexOf()
		{
			static_assert(Utils::template ExistsInList<T, TComponents...>(), "Type T must exist in TComponents type list.");
			return Utils::template IndexOf<T>::template InList<TComponents...>;
		}

		EntityId ConstructInvalidEntity()
		{
			m_Entities.Add({});
			m_ComponentDatas.Add({});
			return m_Entities.GetLength() - 1;
		}

	private:
		List<EntityFlags> m_Entities;
		List<EntityData> m_ComponentDatas;
	};
}