#include "bintlib.h"

using namespace std;

int main()
{
	BigInt number1 = BigInt("1234567890");
	BigInt number2 = BigInt("455675676762");
	std::cout << -number2;
	return 0;
}
