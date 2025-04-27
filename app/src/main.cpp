#include "bintlib.h"

using namespace std;

int main() {
	BigInt number1 = BigInt("12345678901234567890");
	BigInt number2 = BigInt("12345");
	std::cout << number1.to_double();
	return 0;
}
