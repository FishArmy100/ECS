#include <iostream>
#include "BitSet.h"
#include "Utils.h"
#include "Tuple.h"
#include "Optional.h"
#include "EntityData.h"

using namespace ECS;
using namespace ECS::Utils;

int main()
{
	EnityData<std::string, int> data;
	std::cout << data.HasComponent<int>();
}