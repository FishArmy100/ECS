#pragma once


namespace ECS
{
	namespace Utils
	{
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

		/*template<size_t size, size_t allignment>
		struct Storage
		{
			struct Type
			{
				alignas(allignment) unsigned char data[size];
			};
		};

		template<typename T>
		using StorageOf = Storage<sizeof(T), alignof(T)>;

		template<typename T>
		void SetStorageData(T&& data, StorageOf<T>& storage)
		{
			*reinterpret_cast<T*>(&storage) = std::move(data);
		}

		template<typename T>
		void CloneStorage(const StorageOf<T>& src, StorageOf<T>& dest)
		{
			*reinterpret_cast<T*>(&dest) = *reinterpret_cast<T*>(&src);
		}

		template<typename T>
		T& GetStorage(StorageOf<T>& storage)
		{
			return *reinterpret_cast<T*>(&storage);
		}

		template<typename T>
		void MoveStorage(StorageOf<T>&& src, StorageOf<T>& dest)
		{
			*reinterpret_cast<T*>(&dest) = std::move(*reinterpret_cast<T*>(&src));
		}
		
		template<typename T>
		void DeleteStorage(StorageOf<T>& storage)
		{
			reinterpret_cast<T*>(&storage)->~T();
		}*/
	}
}