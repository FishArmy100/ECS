#include <iostream>
#include "BitSet.h"
#include "Utils.h"
#include "Tuple.h"
#include "Optional.h"

using namespace ECS;
using namespace ECS::Utils;

int main()
{
	using TestOption = Optional<std::string, true>;

	TestOption option = std::string("Hello World!");
	TestOption op2 = std::move(option);
	TestOption op3;
	op3 = std::move(op2);

	std::cout << std::boolalpha << option.HasValue() << ", " << op2.HasValue() << ", " << op3.Get() << "\n";
}