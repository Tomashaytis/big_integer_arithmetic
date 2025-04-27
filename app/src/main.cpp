#include "bintlib.h"

using namespace std;

int main()
{
	BigInt number = BigInt("12345678901234567890");
	std::cout << number * -number;
	return 0;
}
