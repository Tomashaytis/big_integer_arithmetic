#include "bintlib.h"

using namespace std;

int main() {
	BigInt number1 = BigInt("2");
	BigInt number2 = BigInt("20");
	std::cout << BigInt::pow(number1, number2, 8) << endl;
	return 0;
}
