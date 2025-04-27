#include "bintlib.h"

using namespace std;

int main() {
	BigInt number1 = BigInt("12345678901234567890");
	BigInt number2 = BigInt("1234567890");
	std::cout << number2 / number1 << std::endl;
	std::cout << number2 % number1;
	number1 += number2;
	std::cout << number1;
	return 0;
}
