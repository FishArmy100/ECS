#include <iostream>
#include "BitSet.h"
#include "Tuple.h"
#include "Optional.h"
#include "Array.h"
#include "List.h"
#include "EntityRegistry.h"

using namespace ECS;
using namespace ECS::Utils;

struct Pos
{
	float X = 0;
	float Y = 0;
	float Z = 0;

	Pos() = default;

	Pos(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

class Tester
{
public:
	Tester(const std::string& message) : m_Message(message)
	{
		std::cout << "Tester constructed\n";
	}

	Tester(const Tester& other) : m_Message(other.m_Message)
	{
		std::cout << "Tester copy constructed\n";
	}

	Tester& operator=(const Tester& other)
	{
		if (this == &other)
			return *this;

		m_Message = other.m_Message;
		std::cout << "Tester copy assigned\n";
		return *this;
	}

	Tester(Tester&& other) : m_Message(std::move(other.m_Message))
	{
		std::cout << "Tester move constructed\n";
	}

	Tester& operator=(Tester&& other)
	{
		if (this == &other)
			return *this;

		m_Message = std::move(other.m_Message);
		std::cout << "Tester move assigned\n";

		return *this;
	}

	~Tester()
	{
		std::cout << "Tester destructed\n";
	}

	std::string& GetMessage()
	{
		return m_Message;
	}

private:
	std::string m_Message;
};

int main()
{
	std::cout << std::boolalpha;

	EntityRegistry<std::string, Pos, Tester> list;

	EntityId e1 = list.CreateEntity();
	std::cout << "Entity created\n\n";

	/*list.AddComponent<Tester>(e1, "Hello World!");
	std::cout << "Tester component added\n\n";

	Tester& tester = list.GetComponent<Tester>(e1);
	std::cout << tester.GetMessage() << "\n";
	std::cout << "Tester component gotten\n\n";

	list.RemoveComponent<Tester>(e1);
	std::cout << "Tester component removed\n\n";*/

	list.DestroyEntity(e1);
	std::cout << "Entity destroied\n\n";

	Tuple<int, float> tuple = { 5, 5.4f };
}