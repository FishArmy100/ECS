#pragma once


namespace ECS
{
	namespace Utils
	{
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