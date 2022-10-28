#pragma once

namespace ECS
{
	namespace Utils
	{
		template<typename T>
		class Ref
		{
		public:
			Ref(T& dataRef) : m_Data(&dataRef) {}
			Ref() = delete;
			Ref(const Ref<T>&) = default; 
			~Ref() = default;

			T* Get() { return m_Data; }
			const T* Get() const { return m_Data; }

			T* operator->()
			{
				return m_Data;
			}

			const T* operator->() const
			{
				return m_Data;
			}

			const T& operator*() const
			{
				return *m_Data;
			}

			T& operator*()
			{
				return *m_Data;
			}

			bool operator==(const Ref& other) const
			{
				return m_Data == other.m_Data;
			}

			bool operator!=(const Ref& other) const
			{
				return m_Data != other.m_Data;
			}

		private:
			T* m_Data;
		};
	}
}