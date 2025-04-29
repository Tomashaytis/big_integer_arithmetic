#include "bintlib.h"

using namespace std;

int main() {
	BigInt number1("202520252025202520252025202520252025");
	BigInt number2("64136413641364136413641364136413");
	BigInt number3("2904202529042025290420252904202529042025");
	BigInt module("10000000000000000000000000000000007");
	//std::cout << BigInt::pow(number1, number2, 8) % 1000000007 << endl;
	std::cout << BigInt::montgomery_pow(number1, number3, module, 8) << endl;
	return 0;
}
