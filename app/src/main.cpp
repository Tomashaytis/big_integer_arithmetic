#include "bintlib.h"

using namespace std;

int main()
{
	BigInt number = BigInt("12345678901234567890000000000000");
	std::cout << number;
	return 0;
}
