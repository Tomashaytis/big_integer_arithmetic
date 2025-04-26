#include <bintlib.h>

std::vector<uint32_t> BigInt::parse_number(std::string number, uint64_t base)
{
	std::vector<uint32_t> chunks;
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
			if (remainder > base - 1)
			{
				quotient += (char)(remainder / base + '0');
				remainder = remainder % base;
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

		chunks.push_back((uint32_t)remainder);
		number = quotient;
	}
	return chunks;
}

std::string BigInt::concat_number(std::vector<uint32_t> chunks, bool is_negative, uint64_t base)
{
	std::vector<bool> bits;
	for (auto chunk : chunks)
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

	if (is_negative)
		number += '-';

	std::reverse(number.begin(), number.end());

	return number;
}

BigInt::BigInt(std::string number)
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

	_chunks = parse_number(number, BASE);
}

std::ostream& operator <<(std::ostream& os, const BigInt& number)
{
	os << BigInt::concat_number(number._chunks, number._is_negative, BigInt::BASE);
	return os;
}

BigInt BigInt::sum(const BigInt& lhs, const BigInt& rhs)
{
	std::vector<uint32_t> result;

	size_t max_size = std::max(lhs._chunks.size(), rhs._chunks.size());
	result.reserve(max_size + 1);  

	uint64_t carry = 0;

	for (size_t i = 0; i < max_size; ++i) {
		uint64_t a = (i < lhs._chunks.size()) ? static_cast<uint64_t>(lhs._chunks[i]) : 0;
		uint64_t b = (i < rhs._chunks.size()) ? static_cast<uint64_t>(rhs._chunks[i]) : 0;

		uint64_t sum = a + b + carry;

		result.push_back(static_cast<uint32_t>(sum % BASE));

		carry = sum / BASE;
	}

	if (carry > 0) {
		result.push_back(static_cast<uint32_t>(carry));
	}

	BigInt res("0");
	res._chunks = result;
	res._is_negative = false;
	return res;
}

BigInt BigInt::simple_mul(const BigInt& lhs, const BigInt& rhs)
{
	// TODO: умножение больших чисел "в столбик"
	throw std::logic_error("Not implemented");
}

BigInt BigInt::karatsuba_mul(const BigInt& lhs, const BigInt& rhs)
{
	// TODO: умножение больших чисел методом Карацубы
	throw std::logic_error("Not implemented");
}

std::pair<BigInt, BigInt> BigInt::div(const BigInt& lhs, const BigInt& rhs)
{
	// TODO: деление нацело больших чисел (возвращает частное и остаток)
	throw std::logic_error("Not implemented");
}

BigInt BigInt::mod(const BigInt& lhs, const BigInt& rhs)
{
	// TODO: остаток от деления больших чисел
	throw std::logic_error("Not implemented");
}

std::tuple<BigInt, BigInt, BigInt> BigInt::extended_gcd(const BigInt& lhs, const BigInt& rhs)
{
	// TODO: поиск НОД расширенным алгоритмом Евклида
	throw std::logic_error("Not implemented");
}

BigInt BigInt::gcd(const BigInt& lhs, const BigInt& rhs)
{
	// TODO: поиск НОД алгоритмом Евклида (сначала реализовать расширенный алгоритм Евклида)
	throw std::logic_error("Not implemented");
}

BigInt BigInt::left_shift(const BigInt& number, int shift)
{
	// TODO: левый сдвиг большого числа на заданное число разрядов (деление на степень 2)
	throw std::logic_error("Not implemented");
}

BigInt BigInt::right_shift(const BigInt& number, int shift)
{
	// TODO: правый сдвиг большого числа на заданное число разрядов (умножение на степень 2)
	throw std::logic_error("Not implemented");
}

BigInt BigInt::montgomery_mul(const BigInt& rhs, const BigInt& lhs, const BigInt& module)
{
	// TODO: умножение больших чисел методом Монтгомери по модулю
	throw std::logic_error("Not implemented");
}

BigInt BigInt::pow(const BigInt& number, const BigInt& degree, int base)
{
	// TODO: возведение в степень для больших чисел бинарным (default) и q-арным алгоритмом
	throw std::logic_error("Not implemented");
}

BigInt BigInt::montgomery_pow(const BigInt& rhs, const BigInt& lhs, const BigInt& module, int base)
{
	// TODO: возведение в степень по модулю для больших чисел методом Монтгомери с использованием бинарного/q-арного алгоритма
	throw std::logic_error("Not implemented");
}