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
	data.AddComponent<std::string>("Hello World!");
	std::cout << std::boolalpha 
		<< data.HasComponents<std::string>() 
		<< ", Number: " << data.GetComponent<std::string>();
}