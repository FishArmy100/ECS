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
};

int main()
{
	std::cout << std::boolalpha;

	EntityRegistry<std::string, Pos> list;

	EntityId e1 = list.CreateEntity();
	list.AddComponent<Pos>(e1);

	list.DestroyEntity(e1);
}