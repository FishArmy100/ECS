#pragma once
#include <cstdint>
#include <iostream>
#include <sstream>

namespace ECS
{
	template<size_t size>
	class BitSet
	{
		static_assert(size > 0, "Size must be a number greater than 0");

	private:
		static constexpr size_t GetByteCount()
		{
			size_t byteCount = size / CHAR_BIT;
			if constexpr (size % CHAR_BIT > 0)
				byteCount += 1;
			return byteCount;
		}

	public:
		BitSet() = default;
		BitSet(char values[GetByteCount()])
		{
			for (int i = 0; i < GetByteCount(); i++)
			{
				m_Bytes[i] = values[i];
			}
		}

		BitSet(const BitSet&) = default;
		BitSet& operator=(const BitSet&) = default;
		BitSet(BitSet&&) = default;
		BitSet& operator=(BitSet&&) = default;
		~BitSet() = default;

		void Set(size_t index, bool value)
		{
			if (index >= size)
				return;

			size_t byteIndex = index / CHAR_BIT;
			size_t bitIndex = index % CHAR_BIT;

			if (value)
			{
				std::uint8_t setter = 1 << bitIndex;
				m_Bytes[byteIndex] |= setter;
			}
			else
			{
				std::uint8_t setter = 0 << bitIndex;
				m_Bytes[byteIndex] &= setter;
			}
		}

		void Any()
		{
			for (size_t i = 0; i < GetByteCount(); i++)
			{

			}
		}

		bool Get(size_t index)
		{
			if (index >= size)
				return false;

			size_t byteIndex = index / CHAR_BIT;
			size_t bitIndex = index % CHAR_BIT;
			std::uint8_t finder = 1 << bitIndex;

			return (m_Bytes[byteIndex] & finder) > 0;
		}

	private:
		template<size_t s>
		friend std::ostream& operator<<(std::ostream& out, const BitSet<s>& bitset);

		template<size_t s>
		friend BitSet<s> operator&(const BitSet<s>& a, const BitSet<s>& b);

		template<size_t s>
		friend BitSet<s> operator|(const BitSet<s>& a, const BitSet<s>& b);
		
		template<size_t s>
		friend BitSet<s> operator^(const BitSet<s>& a, const BitSet<s>& b);

		template<size_t s>
		friend BitSet<s> operator==(const BitSet<s>& a, const BitSet<s>& b);

		template<size_t s>
		friend BitSet<s> operator==(const BitSet<s>& a, const BitSet<s>& b);


		char m_Bytes[GetByteCount()] = {};
	};

	template<size_t s>
	BitSet<s> operator&(const BitSet<s>& a, const BitSet<s>& b)
	{
		BitSet<s> bitset;
		for (size_t i = 0; i < bitset.GetByteCount(); i++)
		{
			bitset.m_Bytes[i] = a.m_Bytes[i] & b.m_Bytes[i];
		}

		return bitset;
	}

	template<size_t size>
	std::ostream& operator<<(std::ostream& out, const BitSet<size>& bitset)
	{
		std::stringstream ss;

		if (bitset.GetByteCount() > 1)
		{
			for (size_t byte = 0; byte < bitset.GetByteCount() - 1; byte++)
			{
				for (size_t bit = 0; bit < CHAR_BIT; bit++)
				{
					ss << ((bitset.m_Bytes[byte] >> bit) & 1);
				}
			}
		}

		if (size % CHAR_BIT == 0)
		{
			for (size_t bit = 0; bit < CHAR_BIT; bit++)
				ss << ((bitset.m_Bytes[bitset.GetByteCount() - 1] >> bit) & 1);
		}
		else
		{
			for (size_t bit = 0; bit < size % CHAR_BIT; bit++)
			{
				ss << ((bitset.m_Bytes[bitset.GetByteCount() - 1] >> bit) & 1);
			}
		}

		out << ss.str();
		return out;
	}
}