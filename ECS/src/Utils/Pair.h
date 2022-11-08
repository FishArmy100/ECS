#pragma once
#include <utility>

namespace ECS::Utils
{
	template<typename TFirst, typename TSecond>
	struct Pair
	{
		TFirst First;
		TSecond Second;

		template<typename T1, typename T2>
		Pair(T1&& first, T2&& second) : First(std::forward<T1>(first)), Second(std::forward<T2>(second)) {}
		Pair() = default;

		Pair(const Pair&) = default;
		Pair(Pair&&) = default;
		Pair& operator=(const Pair&) = default;
		Pair& operator=(Pair&&) = default;
	};
}