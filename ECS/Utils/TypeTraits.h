#pragma once

namespace ECS
{
	namespace Utils
	{
		// type list indexer
		template<size_t index, typename TFirst, typename... Args>
		struct TypeListIndexer
		{
			using Type = typename TypeListIndexer<index - 1, Args...>::Type;
		};

		template<typename TFirst, typename... Args>
		struct TypeListIndexer<0, TFirst, Args...>
		{
			using Type = TFirst;
		};

		// Are types the same
		template<typename TFirst, typename TSecond>
		struct IsSameInternal
		{
			bool Value = false;
		};

		template<typename T>
		struct IsSameInternal<T, T>
		{
			bool Value = true;
		};

		template<typename TFirst, typename TSecond>
		constexpr bool IsSame = IsSameInternal<TFirst, TSecond>().Value;

		// Are all unique types
		template<typename... TArgs>
		struct AllUniqueTypesInternal;

		template<typename T>
		struct AllUniqueTypesInternal<T>
		{
			static constexpr bool Value = true;
		};

		template<typename TFirst, typename TSecond, typename... TRest>
		struct AllUniqueTypesInternal<TFirst, TSecond, TRest...>
		{
			static constexpr bool Value = !IsSame<TFirst, TSecond> &&
				AllUniqueTypesInternal<TFirst, TRest...>::Value &&
				AllUniqueTypesInternal<TSecond, TRest...>::Value;
		};

		template<typename... Types>
		constexpr bool AreAllTypesUnique = AreAllTypesUnique<Types...>::Value;

		template<typename T>
		struct IndexGetterInternal
		{
			template<size_t index, typename...>
			struct Getter;

			template<size_t index, typename TFirst, typename... TRest>
			struct Getter<index, TFirst, TRest...>
			{
				static constexpr size_t Index()
				{
					if constexpr (IsSame<TFirst, T>)
						return index;
					else
						return Getter<index + 1, TRest...>::Index();
				}
			};

			template<size_t index>
			struct Getter<index>;
		};

		template<typename T>
		struct IndexOf
		{
			template<typename... TypeList>
			static constexpr size_t InList = IndexGetterInternal<T>::template Getter<0, TypeList...>::Index();
		};

		template<typename T, typename TFirst, typename... TRest>
		constexpr bool ExistsInList()
		{
			if constexpr (IsSame<T, TFirst>)
				return true;
			else if constexpr (sizeof...(TRest) == 0)
				return false;
			else
				return ExistsInList<T, TRest...>();
		}
	}
}