#pragma once
#include <cstdlib>
#include <type_traits>

namespace ECS
{
	template<typename T>
	class List
	{
	public:
		class Iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = value_type*;
			using reference = value_type&;
		public:
			Iterator(pointer ptr) : m_Ptr(ptr) {}

			reference operator->() { return *m_Ptr; }
			reference operator*() { return *m_Ptr; }
			Iterator& operator++() { m_Ptr++; return *this; }

			Iterator operator++(int)
			{
				Iterator temp = *this;
				++(*this);
				return temp;
			}

			bool operator==(const Iterator& other)
			{
				return m_Ptr == other.m_Ptr;
			}

			bool operator!=(const Iterator& other)
			{
				return !(*this == other);
			}

		private:
			pointer m_Ptr;
		};

		using iterator = Iterator;

	public:
		List() : m_Length(0), m_Capacity(0), m_Data(nullptr) {}
		
		List(const List& other) : 
			m_Length(other.m_Length), 
			m_Capacity(other.m_Capacity)
		{
			m_Data = (T*)std::malloc(sizeof(T) * other.m_Capacity);
			CopyData(other.m_Data, m_Data, m_Length);
		}

		List(List&& other) :
			m_Length(other.m_Length),
			m_Capacity(other.m_Capacity),
			m_Data(other.m_Data)
		{
			other.m_Length = 0;
			other.m_Capacity = 0;
			other.m_Data = nullptr;
		}

		~List()
		{
			std::free(m_Data);
		}

		List& operator=(const List& other)
		{
			if (this == &other)
				return *this;

			m_Capacity = other.m_Capacity;
			m_Length = other.m_Length;
			std::free(m_Data);

			m_Data = (T*)std::malloc(sizeof(T) * other.m_Capacity);
			CopyData(other.m_Data, m_Data, m_Length);

			return *this;
		}

		List& operator=(List&& other)
		{
			if (this == &other)
				return *this;

			m_Capacity = other.m_Capacity;
			m_Length = other.m_Length;
			m_Data = other.m_Data;

			other.m_Capacity = 0;
			other.m_Length = 0;
			other.m_Data = nullptr;
		}

		T& Add(const T& value)
		{
			CheckAllocation(m_Length + 1);
			T* ptr = new (&m_Data[m_Length]) T(value);
			m_Length++;

			return *ptr;
		}

		template<typename... TArgs>
		T& EmplaceAdd(TArgs&&... args)
		{
			CheckAllocation(m_Length + 1);
			T* ptr = new (&m_Data[m_Length]) T(std::forward<TArgs>(args)...);
			m_Length++;

			return *ptr;
		}

		void Reserve(size_t size)
		{
			CheckAllocation(m_Length + size);
		}

		void Erase(size_t index)
		{
			if (index >= m_Length)
				return;

			for (int i = index; i < m_Length - 1; i++)
				std::swap((*this)[i], (*this)[i + 1]);

			(*this)[m_Length - 1].~T(); // delete the last element;
			m_Length--;
		}

		size_t GetLength() const { return m_Length; }
		size_t GetCapacity() const { return m_Capacity; }
		
		T& operator[](size_t index)
		{
			return m_Data[index];
		}

		T& At(size_t index)
		{
			if (index >= m_Length)
				throw std::out_of_range("Index is out of range of the lists length");

			return (*this)[index];
		}

		iterator begin()
		{
			return Iterator(m_Data);
		}

		iterator end()
		{
			return Iterator(m_Data + m_Length);
		}

	private:
		void CheckAllocation(size_t requiredSize)
		{
			if (m_Capacity < requiredSize)
			{
				size_t newSize = (m_Capacity + 1) * 2;
				newSize = newSize < requiredSize ? requiredSize : newSize;
				Realloc(newSize);
			}

		}

		void Realloc(size_t size)
		{
			T* oldData = m_Data;
			m_Data = (T*)std::malloc(sizeof(T) * size);

			if (oldData != nullptr)
			{
				CopyData(oldData, m_Data, m_Length);
				std::free(oldData);
			}

			m_Capacity = size;
		}

		void CopyData(T* oldData, T* newData, size_t copyLength) const
		{
			for (int i = 0; i < copyLength; i++)
			{
				T* newPtr = &newData[i];
				T& old = oldData[i];
				new (newPtr) T(old);
			}
		}

	private:
		size_t m_Length = 0;
		size_t m_Capacity = 0;

		T* m_Data;
	};
}