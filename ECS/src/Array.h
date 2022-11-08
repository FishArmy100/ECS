#pragma once
#include <initializer_list>
#include <iostream>

namespace ECS
{
	template<typename T, size_t size>
	class Array
	{
	public:
		constexpr static size_t Size() { return size; }
		
		Array(const Array&) = default;
		Array(Array&&) = default;
		Array& operator=(const Array&) = default;
		Array& operator=(Array&&) = default;

		Array(std::initializer_list<T> l)
		{
			for (size_t i = 0; i < size && i < l.size(); i++)
				(*this)[i] = l.begin()[i];
		}

		T& operator[](size_t index)
		{
			return m_Data[index];
		}

		const T& operator[](size_t index) const
		{
			return m_Data[index];
		}

	private:
		T m_Data[size] = {};
	};
}

template<typename T, size_t count>
std::ostream& operator<<(std::ostream& stream, const ECS::Array<T, count>& arr)
{
	for (int i = 0; i < arr.Size(); i++)
	{
		if (i != 0)
			std::cout << ", ";
		std::cout << arr[i];
	}

	return stream;
}