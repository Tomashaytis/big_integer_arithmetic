#include "bintlib.h"

using namespace std;

int main() {
	BigInt number1 = BigInt("98765432101234567890123456789");
	BigInt number2 = BigInt("6413641364136413641364136413");
	BigInt module = BigInt("20252025202520252025202520252025");
	std::cout << BigInt::montgomery_mul_module(number1, number2, module) << endl;
	std::cout << (number1 * number2) % module << endl;
	return 0;
}
