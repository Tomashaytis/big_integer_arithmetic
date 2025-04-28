#include "bintlib.h"

using namespace std;

int main() {
	BigInt number1 = BigInt("12345678901234567890");
	BigInt number2 = BigInt("5");
	std::cout << BigInt::binary_pow(number1, number2) << endl;
	return 0;
}
