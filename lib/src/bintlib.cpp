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

std::string BigInt::get_number() {
	return BigInt::concat_number(this->_chunks, this->_is_negative);
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

BigInt::BigInt(std::vector<uint32_t> chunks, bool is_negative = false)
{
	this->_chunks = chunks;
	this->_is_negative = is_negative;
}

std::ostream& operator <<(std::ostream& os, const BigInt& number)
{
	os << BigInt::concat_number(number._chunks, number._is_negative, BigInt::BASE);
	return os;
}

BigInt BigInt::sum(const BigInt& lhs, const BigInt& rhs)
{
	BigInt res("0");

	if (lhs._is_negative == rhs._is_negative) {
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
		res._chunks = result;
		res._is_negative = lhs._is_negative;
	}
	else
	{
		size_t option = BigInt::_compare_abs_bigints(lhs, rhs);

		switch (option)
		{
		case 1:
			res._is_negative = lhs._is_negative;
			res._chunks = BigInt::_sub_chunks(lhs, rhs)._chunks;
			break;
		case 2:
			return res;
			break;
		case 3:
			res._is_negative = rhs._is_negative;
			res._chunks = BigInt::_sub_chunks(rhs, lhs)._chunks;
			break;
		default:
			break;
		}
	}

	return res;
}

// a - b = (-b) + a
BigInt BigInt::sub(const BigInt& lhs, const BigInt& rhs)
{
	BigInt tmp("0");
	tmp._chunks = rhs._chunks;
	tmp._is_negative = !rhs._is_negative;
	return BigInt::sum(lhs, tmp);
}

BigInt BigInt::simple_mul(const BigInt& lhs, const BigInt& rhs)
{
	BigInt result("0");

	size_t result_size = lhs._chunks.size() + rhs._chunks.size();
	std::vector<uint32_t> res_chunks(result_size, 0);

	for (size_t i = 0; i < lhs._chunks.size(); ++i) {
		uint64_t carry = 0;

		for (size_t j = 0; j < rhs._chunks.size(); ++j) {
			uint64_t mul = static_cast<uint64_t>(lhs._chunks[i]) * rhs._chunks[j];
			uint64_t sum = static_cast<uint64_t>(res_chunks[i + j]) + mul + carry;

			res_chunks[i + j] = static_cast<uint32_t>(sum % BASE);
			carry = sum / BASE;
		}

		res_chunks[i + rhs._chunks.size()] += static_cast<uint32_t>(carry);
	}

	while (res_chunks.size() > 1 && res_chunks.back() == 0) {
		res_chunks.pop_back();
	}

	result._chunks = res_chunks;
	result._is_negative = lhs._is_negative ^ rhs._is_negative;
	return result;
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

// Compare abs 2 numbers
// return 1 when number1 > number2, 2 when number1 = number2 and 3 when number1 < number2
size_t BigInt::_compare_abs_bigints(const BigInt& number1, const BigInt& number2)
{
	if (number1._chunks.size() > number2._chunks.size())
	{
		return 1;
	}
	else if (number1._chunks.size() <  number2._chunks.size())
	{
		return 3;
	}
	else
	{
		for (size_t i = number1._chunks.size() - 1; i > 0; --i)
		{
			if (number1._chunks[i] > number2._chunks[i])
			{
				return 1;
			}
			else if (number1._chunks[i] < number2._chunks[i])
			{
				return 3;
			}
		}
	}
	return 2;
}

// this method only used when abs(lhs) > abs(rhs)
BigInt BigInt::_sub_chunks(const BigInt& lhs, const BigInt& rhs) {
	BigInt res("0");
	std::vector<uint32_t> result;
	result.reserve(lhs._chunks.size());

	int64_t borrow = 0;

	for (size_t i = 0; i < lhs._chunks.size(); ++i) {
		int64_t a = static_cast<int64_t>(lhs._chunks[i]);
		int64_t b = (i < rhs._chunks.size()) ? static_cast<int64_t>(rhs._chunks[i]) : 0;

		int64_t diff = a - b - borrow;

		if (diff < 0) {
			diff += BigInt::BASE;
			borrow = 1;
		}
		else {
			borrow = 0;
		}

		result.push_back(static_cast<uint32_t>(diff));
	}

	while (result.size() > 1 && result.back() == 0) {
		result.pop_back();
	}

	res._chunks = result;
	res._is_negative = false;
	return res;
}


BigInt& BigInt::operator=(const BigInt& other)
{
	if (this != &other) { 
		this->_is_negative = other._is_negative;
		this->_chunks = other._chunks;
	}
	return *this;
}

BigInt BigInt::operator+(const BigInt& other) const 
{
	return BigInt::sum(*this, other);
}

BigInt BigInt::operator-(const BigInt& other) const
{
	return BigInt::sub(*this, other);
}

BigInt BigInt::operator*(const BigInt& other) const
{
	return BigInt::simple_mul(*this, other);
}

BigInt BigInt::operator%(const BigInt& other) const
{
	return BigInt::mod(*this, other);
}