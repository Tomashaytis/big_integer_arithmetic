#include <bintlib.h>

void BigInt::_parse_number(std::string number)
{
	std::string digits = "0123456789";
	while (number.size() > 0)
	{
		uint64_t remainder = 0;
		std::string quotient = "";

		for (size_t i = 0; i < number.size(); i++)
		{
			char symbol = number[i];

			if (digits.find(symbol) == std::string::npos)
			{
				throw new std::invalid_argument("Error parsing big number: " + std::to_string(symbol) + " on position " + std::to_string(i));
			}

			uint64_t digit = (uint64_t)symbol - '0';
			remainder = remainder * 10 + digit;
			if (remainder > _base - 1)
			{
				quotient += (char)(remainder / _base + '0');
				remainder = remainder % _base;
			}
			else
			{
				quotient += '0';
			}
		}

		while (!quotient.empty() && quotient.front() == '0')
		{
			quotient.erase(quotient.begin());
		}

		_chunks.push_back((uint32_t)remainder);
		number = quotient;
	}
}

std::string BigInt::_concat_number() const
{
	std::vector<bool> bits;
	for (auto chunk : _chunks)
	{
		for (size_t i = 0; i < 32; i++)
		{
			bits.push_back((chunk >> i) & 1);
		}
	}
	std::reverse(bits.begin(), bits.end());

	std::string number = "";

	while (bits.size() > 0)
	{
		uint8_t remainder = 0;
		std::vector<bool> quotient;

		for (size_t i = 0; i < bits.size(); i++)
		{
			bool bit = bits[i];

			remainder = remainder * 2 + bit;
			if (remainder > 9)
			{
				quotient.push_back(true);
				remainder -= 10;
			}
			else 
			{
				quotient.push_back(false);
			}
		}

		while (!quotient.empty() && quotient.front() == false)
		{
			quotient.erase(quotient.begin());
		}

		number += remainder + '0';
		bits = quotient;
	}

	std::reverse(number.begin(), number.end());

	return number;
}

BigInt::BigInt(std::string number = "0")
{
	if (number.size() == 0)
	{
		throw new std::invalid_argument("Big number is undefined");
	}

	_is_negative = number[0] == '-';
	if (_is_negative)
	{
		number = number.substr(1);
	}

	_base = (uint64_t)UINT32_MAX + 1;

	_parse_number(number);
}

std::ostream& operator <<(std::ostream& os, const BigInt& number)
{
	if (number._is_negative)
		os << '-';
	os << number._concat_number();
	return os;
}

BigInt BigInt::sum(const BigInt& lhs, const BigInt& rhs)
{
	// TODO: сложение больших чисел
}

BigInt BigInt::sub(const BigInt& lhs, const BigInt& rhs)
{
	// TODO: вычитание больших чисел (обратное сложению)
}

BigInt BigInt::simple_mul(const BigInt& lhs, const BigInt& rhs)
{
	// TODO: умножение больших чисел "в столбик"
}

BigInt BigInt::karatsuba_mul(const BigInt& lhs, const BigInt& rhs)
{
	// TODO: умножение больших чисел методом Карацубы
}

std::pair<BigInt, BigInt> BigInt::div(const BigInt& lhs, const BigInt& rhs)
{
	// TODO: деление нацело больших чисел (возвращает частное и остаток)
}

BigInt BigInt::mod(const BigInt& lhs, const BigInt& rhs)
{
	// TODO: остаток от деления больших чисел
}

std::tuple<BigInt, BigInt, BigInt> BigInt::extended_gcd(const BigInt& lhs, const BigInt& rhs)
{
	// TODO: поиск НОД расширенным алгоритмом Евклида
}

BigInt BigInt::gcd(const BigInt& lhs, const BigInt& rhs)
{
	// TODO: поиск НОД алгоритмом Евклида (сначала реализовать расширенный алгоритм Евклида)
}

BigInt BigInt::left_shift(const BigInt& number, int shift)
{
	// TODO: левый сдвиг большого числа на заданное число разрядов (деление на степень 2)
}

BigInt BigInt::right_shift(const BigInt& number, int shift)
{
	// TODO: правый сдвиг большого числа на заданное число разрядов (умножение на степень 2)
}

BigInt BigInt::montgomery_mul(const BigInt& rhs, const BigInt& lhs, const BigInt& module)
{
	// TODO: умножение больших чисел методом Монтгомери по модулю
}

BigInt BigInt::pow(const BigInt& number, const BigInt& degree, int base = 2)
{
	// TODO: возведение в степень для больших чисел бинарным (default) и q-арным алгоритмом
}

BigInt BigInt::montgomery_pow(const BigInt& rhs, const BigInt& lhs, const BigInt& module, int base = 2)
{
	// TODO: возведение в степень по модулю для больших чисел методом Монтгомери с использованием бинарного/q-арного алгоритма
}