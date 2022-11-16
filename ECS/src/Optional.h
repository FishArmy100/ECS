#pragma once
#include <exception>
#include <iostream>

namespace ECS
{
	struct InvalidOptionalExeption : std::exception
	{
		InvalidOptionalExeption(const char* message) : std::exception(message) {}
	};

	template<typename T>
	class Optional
	{
	public:
		bool HasValue() const { return m_HasValue; };

		T& Get()&
		{
			if (m_HasValue)
				return *reinterpret_cast<T*>(&m_Data);
			else
				throw InvalidOptionalExeption("Cannot access invalid option");
		}

		const T& Get() const&
		{
			if (m_HasValue)
				return *reinterpret_cast<const T*>(&m_Data);
			else
				throw InvalidOptionalExeption("Cannot access invalid option");
		}

		T&& Get()&&
		{
			if (m_HasValue)
				return std::move(*reinterpret_cast<T*>(&m_Data));
			else
				throw InvalidOptionalExeption("Cannot access invalid option");
		}

		void Clear()
		{
			if (m_HasValue)
			{
				Get()->~T();
				m_HasValue = false;
			}
		}

		Optional() : m_HasValue(false), m_Data() {}
		Optional(const T& obj) : m_HasValue(true)
		{
			new (reinterpret_cast<T*>(&m_Data)) T(obj);
		}

		Optional(T&& obj) : m_HasValue(true)
		{
			new (reinterpret_cast<T*>(&m_Data)) T(std::move(obj));
		}

		Optional(const Optional& other) : m_HasValue(other.m_HasValue)
		{
			if (other.m_HasValue)
				new (reinterpret_cast<T*>(&m_Data)) T(other.Get());
		}

		Optional& operator=(const Optional& other)
		{
			if (this == &other)
				return *this;

			if (other.m_HasValue)
			{
				if(this->m_HasValue)
					Get() = other.Get();
				else
					new (reinterpret_cast<T*>(&m_Data)) T(other.Get());
			}
			else
			{
				if(this->m_HasValue)
					Get().~T();
			}
			m_HasValue = other.m_HasValue;
			return *this;
		}

		Optional(Optional&& other) noexcept : m_HasValue(other.m_HasValue)
		{
			if (other.m_HasValue)
			{
				new (reinterpret_cast<T*>(&m_Data)) T(std::move(other.Get()));
				other.m_HasValue = false;
			}
		}

		Optional& operator=(Optional&& other) noexcept
		{
			if (this == &other)
				return *this;

			std::string name = typeid(T).name();

			if (other.m_HasValue)
			{
				if (m_HasValue)
				{
					other.m_HasValue = false;
					Get() = std::move(other.Get());
				}
				else
				{
					new (reinterpret_cast<T*>(&m_Data)) T(std::move(other.Get()));
					other.m_HasValue = false;
					m_HasValue = true;
				}
			}
			else
			{
				if (m_HasValue)
				{
					Get().~T();
					m_HasValue = false;
				}
			}

			return *this;
		}

		~Optional()
		{
			if (m_HasValue)
			{
				Get().~T();
			}
		}
	private:
		bool m_HasValue;
		alignas(T) unsigned char m_Data[sizeof(T)];
	};
}