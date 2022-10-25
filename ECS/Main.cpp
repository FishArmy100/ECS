#include <iostream>
#include "BitSet.h"
#include "Utils.h"
#include "Tuple.h"
#include "Optional.h"
#include "EntityData.h"
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
	EntityDataList<std::string, Pos> list;
}