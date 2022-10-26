#include <iostream>
#include "BitSet.h"
#include "Utils.h"
#include "Tuple.h"
#include "Optional.h"
#include "Array.h"
#include "List.h"
#include "EntityDataList.h"

using namespace ECS;
using namespace ECS::Utils;

struct Pos
{
	float X = 0;
	float Y = 0;
	float Z = 0;
};

int main()
{
	std::cout << std::boolalpha;

	EntityDataList<std::string, Pos> list;
	EntityId e = list.CreateEntity();
	std::string& name = list.AddComponent<std::string>(e, "Hello World!");
	std::cout << list.GetComponent<std::string>(e) << "\n";
	list.RemoveComponent<std::string>(e);
	std::cout << list.HasComponent<std::string>(e) << "\n";
}