#include "bintlib.h"

using namespace std;

int main() {
	BigInt number1 = BigInt("12312312312312321");
	BigInt number2 = BigInt("12312312312312320");
	auto [q, r] = BigInt::div(number1, number2);
	std::cout << BigInt::karatsuba_square(number1) << endl;
	return 0;
}
