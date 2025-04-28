#include "bintlib.h"

using namespace std;

int main() {
	BigInt number1 = BigInt("-10");
	BigInt number2 = BigInt("3");
	std::cout << number1 % number2;
	return 0;
}
