#pragma once
#include <cstdint>
#include <iostream>
#include <sstream>
#include "Utils/Func.h"

namespace ECS
{
	template<size_t size>
	class BitSet;

	namespace Utils
	{
		template<size_t s>
		BitSet<s> PerformOp(const BitSet<s>& a, const BitSet<s>& b, Utils::Func<char(char, char)> op);
	}

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

		bool Any()
		{
			for (size_t i = 0; i < GetByteCount(); i++)
			{
				if (m_Bytes[i] > 0)
					return true;
			}

			return false;
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
		friend BitSet<s> Utils::PerformOp(const BitSet<s>& a, const BitSet<s>& b, Utils::Func<char(char, char)> op);


		char m_Bytes[GetByteCount()] = {};
	};

	template<size_t s>
	BitSet<s> operator&(const BitSet<s>& a, const BitSet<s>& b)
	{
		return Utils::PerformOp(a, b, [](char a, char b) -> char { return a & b; });
	}

	template<size_t s>
	BitSet<s> operator|(const BitSet<s>& a, const BitSet<s>& b)
	{
		return Utils::PerformOp(a, b, [](char a, char b) -> char {return a | b; });
	}

	template<size_t s>
	BitSet<s> operator^(const BitSet<s>& a, const BitSet<s>& b)
	{
		return Utils::PerformOp(a, b, [](char a, char b) -> char { return a ^ b; });
	}

	template<size_t s>
	bool operator==(const BitSet<s>& a, const BitSet<s>& b)
	{
		return !(a ^ b).Any();
	}

	template<size_t s>
	bool operator!=(const BitSet<s>& a, const BitSet<s>& b)
	{
		return !(a == b);
	}

	namespace Utils
	{
		template<size_t s>
		BitSet<s> PerformOp(const BitSet<s>& a, const BitSet<s>& b, Utils::Func<char(char, char)> op)
		{
			BitSet<s> bitset;
			for (size_t i = 0; i < bitset.GetByteCount(); i++)
			{
				bitset.m_Bytes[i] = op(a.m_Bytes[i], b.m_Bytes[i]);
			}

			return bitset;
		}
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