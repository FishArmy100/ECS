#pragma once
#include "Optional.h"
#include "List.h"
#include "Array.h"
#include "Utils/Pair.h"
#include "Utils/Ref.h"
#include "EntityId.h"
#include "Utils/TypeTraits.h"
#include "BitSet.h"
#include "Optional.h"
#include "Tuple.h"
#include <iterator>

namespace ECS
{
	struct NullEntityException : public std::exception {};
	struct EntityAlreadyHasComponentException : public std::exception {};
	struct EntityDoesNotHaveComponentException : public std::exception {};

	template<typename... TComponents>
	class EntityRegistry
	{
	public:
		static constexpr size_t NumberOfComponents() { return sizeof...(TComponents); }
	private:
		using ComponentFlags = BitSet<NumberOfComponents()>;
		using EntityFlags = Optional<ComponentFlags>;
		using TupleType = Tuple<Optional<TComponents>...>;
		using EntityData = Optional<Tuple<Optional<TComponents>...>>;

	public:
		EntityRegistry() = default;
		EntityRegistry(const EntityRegistry&) = delete;
		EntityRegistry& operator=(const EntityRegistry&) = delete;

		template<typename... TViewComponents>
		friend class View;

	public:
		EntityId CreateEntity()
		{
			EntityId id = GetFirstInvalid();
			m_ComponentDatas[id] = { TupleType{} };
			m_Entities[id] = { ComponentFlags{} };
			m_EntityCount++;
			return id;
		}

		void DestroyEntity(EntityId id)
		{
			if (m_Entities[id].HasValue())
			{
				m_Entities[id].Get() = {};
				m_ComponentDatas[id].Get() = {};
			}
			else
			{
				throw NullEntityException();
			}
		}

		size_t EntityCount() const
		{
			return m_EntityCount;
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

		Optional<BitSet<NumberOfComponents()>> GetEntityFlags(EntityId e)
		{
			if (e < m_Entities.GetLength())
				return m_Entities[e];

			return {};
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

		template<typename... Ts>
		bool HasComponents(EntityId e)
		{
			return (HasComponent<Ts>() && ...);
		}

		template<typename T>
		void RemoveComponent(EntityId e)
		{
			if (HasComponent<T>(e))
			{
				m_ComponentDatas[e].Get().Get<IndexOf<T>()>() = {};
				m_Entities[e].Get().Set(IndexOf<T>(), false);
			}
			else
			{
				throw EntityDoesNotHaveComponentException();
			}
		}

	public:
		template<typename... TViewComponents>
		class View
		{
		private:
			friend class Iterator;
		public:
			class Iterator
			{
			public:
				Iterator(Utils::Ref<EntityRegistry<TComponents...>> registry, size_t index) : m_Registry(registry), m_Index(index), m_Filter({})
				{
					Array<size_t, sizeof...(TViewComponents)> arr = { IndexOf<TViewComponents>()... };
					for (size_t i = 0; i < arr.Size(); i++)
						m_Filter.Set(arr[i], true);

					while (m_Index < m_Registry->EntityCount())
					{
						Optional<ComponentFlags> flags = m_Registry->m_Entities[m_Index];

						// if the current index is valid, and has all the components
						if (flags.HasValue() && (flags.Get() & m_Filter) == m_Filter)
							break;
						m_Index++;
					}
				}

				Iterator(const Iterator&) = default;
				Iterator& operator=(const Iterator&) = default;
				Iterator(Iterator&&) = default;
				Iterator& operator=(Iterator&&) = default;

				Tuple<Utils::Ref<TViewComponents>...> operator*()
				{
					EntityData& componentData = m_Registry->m_ComponentDatas[m_Index];
					return Tuple<Utils::Ref<TViewComponents>...>(Utils::Ref<TViewComponents>(componentData.Get().Get<IndexOf<TViewComponents>()>().Get())...);
				}

				Iterator& operator++()
				{
					while (m_Index < m_Registry->EntityCount())
					{
						m_Index++;
						Optional<ComponentFlags> flags = m_Registry->m_Entities[m_Index];
						// if the current index is valid, and has all the components
						if (flags.HasValue() && (flags.Get() & m_Filter) == m_Filter)
							return *this;
					}

					return *this;
				}

				Iterator operator++(int)
				{
					Iterator it = *this;
					++(*this);
					return it;
				}

				friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_Index == b.m_Index && a.m_Registry == b.m_Registry; };
				friend bool operator!= (const Iterator& a, const Iterator& b) { return !(a == b); };

			private:
				size_t m_Index;
				Utils::Ref<EntityRegistry<TComponents...>> m_Registry;
				BitSet<sizeof...(TComponents)> m_Filter;
			};

			View(Utils::Ref<EntityRegistry<TComponents...>> registry) :
				m_Registry(registry)
			{

			}

			Iterator begin()
			{
				return Iterator(m_Registry, 0);
			}

			Iterator end()
			{
				return Iterator(m_Registry, m_Registry->EntityCount());
			}

		private:
			Utils::Ref<EntityRegistry<TComponents...>> m_Registry;
		};

		template<typename... TViewComponents>
		View<TViewComponents...> GetView()
		{
			return View<TViewComponents...>(*this);
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
		List<EntityFlags> m_Entities = {};
		List<EntityData> m_ComponentDatas = {};
		size_t m_EntityCount = {};
	};
}