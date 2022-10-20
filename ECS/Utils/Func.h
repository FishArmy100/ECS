#pragma once

namespace ECS
{
	namespace Utils
	{
		// Func
		template<typename T>
		class Func { static_assert("T must be a function"); };

		template<typename TRet, typename... TArgs>
		class Func<TRet(TArgs...)>
		{
		public:
			Func(TRet(*func)(TArgs...)) : m_Func(func) {}

			template<typename TFunc>
			Func(TFunc func) : m_Func(static_cast<TRet(*)(TArgs...)>(func)) {}

			inline TRet operator()(TArgs... args) { return std::move(m_Func(std::forward<TArgs>(args)...)); }

		private:
			TRet(*m_Func)(TArgs...);
		};

		template<typename... TArgs>
		class Func<void(TArgs...)>
		{
		public:
			Func(void(*func)(TArgs...)) : m_Func(func) {}

			template<typename TFunc>
			Func(TFunc func) : m_Func(func) {}

			inline void operator()(TArgs... args) { m_Func(std::forward<TArgs>(args)...); }

		private:
			void(*m_Func)(TArgs...);
		};
	}
}