﻿#include <bintlib.h>

std::vector<uint32_t> BigInt::parse_number(const std::string& number_str, uint64_t base) {
	std::string number = number_str;
	std::vector<uint32_t> chunks;
	std::string digits = "0123456789";
	while (number.size() > 0) {
		uint64_t remainder = 0;
		std::string quotient = "";

		for (size_t i = 0; i < number.size(); i++) {
			char symbol = number[i];

			if (digits.find(symbol) == std::string::npos)
				throw new std::invalid_argument("Error parsing big number: " + std::to_string(symbol) + " on position " + std::to_string(i));

			uint64_t digit = (uint64_t)symbol - '0';
			remainder = remainder * 10 + digit;
			if (remainder > base - 1) {
				quotient += (char)(remainder / base + '0');
				remainder = remainder % base;
			}
			else
				quotient += '0';
		}

		while (!quotient.empty() && quotient.front() == '0')
			quotient.erase(quotient.begin());

		chunks.push_back((uint32_t)remainder);
		number = quotient;
	}
	return chunks;
}

std::string BigInt::concat_number(const std::vector<uint32_t>& chunks, bool is_negative, uint64_t base) {
	std::vector<bool> bits;
	for (auto chunk : chunks) {
		for (size_t i = 0; i < 32; i++)
			bits.push_back((chunk >> i) & 1);
	}
	std::reverse(bits.begin(), bits.end());

	std::string number = "";

	while (bits.size() > 0) {
		uint8_t remainder = 0;
		std::vector<bool> quotient;

		for (size_t i = 0; i < bits.size(); i++) {
			bool bit = bits[i];

			remainder = remainder * 2 + bit;
			if (remainder > 9) {
				quotient.push_back(true);
				remainder -= 10;
			}
			else 
				quotient.push_back(false);
		}

		while (!quotient.empty() && quotient.front() == false)
			quotient.erase(quotient.begin());

		number += remainder + '0';
		bits = quotient;
	}

	if (is_negative)
		number += '-';

	std::reverse(number.begin(), number.end());

	return number;
}

int BigInt::abs_cmp(const BigInt& number1, const BigInt& number2) {
	if (number1._chunks.size() > number2._chunks.size())
		return 1;
	else if (number1._chunks.size() < number2._chunks.size())
		return -1;
	else {
		for (int i = (int)number1._chunks.size() - 1; i >= 0; i--) {
			if (number1._chunks[i] > number2._chunks[i])
				return 1;
			else if (number1._chunks[i] < number2._chunks[i])
				return -1;
		}
	}
	return 0;
}

BigInt BigInt::sub_chunks(const BigInt& lhs, const BigInt& rhs) {
	BigInt res;
	std::vector<uint32_t> result;
	result.reserve(lhs._chunks.size());

	int64_t borrow = 0;

	for (size_t i = 0; i < lhs._chunks.size(); ++i) {
		int64_t a = (int64_t)lhs._chunks[i];
		int64_t b = (i < rhs._chunks.size()) ? (int64_t)rhs._chunks[i] : 0;

		int64_t diff = a - b - borrow;

		if (diff < 0) {
			diff += BigInt::BASE;
			borrow = 1;
		}
		else {
			borrow = 0;
		}

		result.push_back((uint32_t)diff);
	}

	while (result.size() > 1 && result.back() == 0) {
		result.pop_back();
	}

	res._chunks = result;
	res._is_negative = false;
	return res;
}

uint32_t BigInt::leading_zeros(uint32_t value) {
	if (value == 0)
		return 32;

	uint32_t count = 0;
	for (int i = 31; i >= 0; i--)
	{
		if ((value >> i) & 1)
			break;
		count++;
	}
	return count;
}

uint32_t BigInt::estimate_quotient(const BigInt& dividend, const BigInt& divider) {
	if (dividend._chunks.size() < divider._chunks.size())
		return 0;

	uint32_t a = dividend._chunks.back();
	uint32_t b = (dividend._chunks.size() > 1) ? dividend._chunks[dividend._chunks.size() - 2] : 0;
	uint32_t e = divider._chunks.back();

	uint64_t c = (uint64_t)a;
	if (dividend._chunks.size() > 1)
		c = (c << 32) | b;

	c = c / e;

	uint32_t q = (c > UINT32_MAX) ? UINT32_MAX : (uint32_t)c;

	return q + 2;
}

uint32_t BigInt::bit_length() const {
	BigInt zero;

	if (*this == zero)
		return 0;

	uint32_t total_bits = (uint32_t)(_chunks.size() - 1) * 32;
	uint32_t top_chunk = _chunks.back();
	total_bits += 32 - BigInt::leading_zeros(top_chunk);
	return total_bits;
}

std::string BigInt::to_string() const {
	return BigInt::concat_number(_chunks, _is_negative);
}

double BigInt::to_double() const {
	uint64_t sign = _is_negative ? (1ULL << 63) : 0;

	uint64_t mantissa = 0;
	uint32_t shift = BigInt::leading_zeros(_chunks.back());
	BigInt tmp = *this << shift;
	if (_chunks.size() > 1) {
		uint32_t high_bits = tmp._chunks.back() & 0x7FFFFFFF;
		mantissa = (uint64_t)high_bits << 21;
		mantissa = mantissa | (uint64_t)tmp._chunks[tmp._chunks.size() - 2] >> 11;
	}
	else {
		mantissa = ((uint64_t)tmp._chunks.back() & 0x7FFFFFFF) << 21;
	}

	uint64_t exponent = 1023 + 32 * (_chunks.size() - 1) + 31 - shift;
	exponent <<= 52;

	uint64_t result = sign | exponent | mantissa;
	return *reinterpret_cast<const double*>(&result);
}

BigInt BigInt::abs(const BigInt& number) {
	BigInt result = number;
	result._is_negative = false;
	return result;
}

BigInt::BigInt(uint32_t number, bool is_negative) {
	_is_negative = is_negative;
	_chunks.push_back(number);
}

BigInt::BigInt(const std::string& number_str) {
	std::string number = number_str;
	if (number.size() == 0)
		throw new std::invalid_argument("Big number is undefined");

	_is_negative = number[0] == '-';
	if (_is_negative)
		number = number.substr(1);

	if (number.size() == 0)
		throw new std::invalid_argument("Big number is undefined");

	_chunks = parse_number(number, BASE);
}

BigInt::BigInt(const std::vector<uint32_t>& chunks, bool is_negative) {
	_chunks = chunks;
	if (_chunks.size() == 0)
		_chunks.push_back(0);
	_is_negative = is_negative;
}

std::ostream& operator <<(std::ostream& os, const BigInt& number) {
	os << BigInt::concat_number(number._chunks, number._is_negative, BigInt::BASE);
	return os;
}

BigInt BigInt::sum(const BigInt& lhs, const BigInt& rhs) {
	BigInt res;

	if (lhs._is_negative == rhs._is_negative) {
		std::vector<uint32_t> result;
		size_t max_size = std::max(lhs._chunks.size(), rhs._chunks.size());
		result.reserve(max_size + 1);

		uint64_t carry = 0;

		for (size_t i = 0; i < max_size; ++i) {
			uint64_t a = (i < lhs._chunks.size()) ? (uint64_t)lhs._chunks[i] : 0;
			uint64_t b = (i < rhs._chunks.size()) ? (uint64_t)rhs._chunks[i] : 0;

			uint64_t sum = a + b + carry;
			result.push_back((uint32_t)(sum % BASE));
			carry = sum / BASE;
		}

		if (carry > 0) {
			result.push_back((uint32_t)carry);
		}
		res._chunks = result;
		res._is_negative = lhs._is_negative;
	}
	else
	{
		int check = BigInt::abs_cmp(lhs, rhs);

		if (check > 0) {
			res._is_negative = lhs._is_negative;
			res._chunks = BigInt::sub_chunks(lhs, rhs)._chunks;
		}
		else if (check == 0) {
			return res;
		}
		else {
			res._is_negative = rhs._is_negative;
			res._chunks = BigInt::sub_chunks(rhs, lhs)._chunks;
		}
	}

	while (res._chunks.size() > 1 && res._chunks.back() == 0) {
		res._chunks.pop_back();
	}

	return res;
}

BigInt BigInt::sub(const BigInt& lhs, const BigInt& rhs) {
	BigInt tmp;
	tmp._chunks = rhs._chunks;
	tmp._is_negative = !rhs._is_negative;
	return BigInt::sum(lhs, tmp);
}

BigInt BigInt::simple_mul(const BigInt& lhs, const BigInt& rhs) {
	BigInt result;

	size_t result_size = lhs._chunks.size() + rhs._chunks.size();
	std::vector<uint32_t> res_chunks(result_size, 0);

	for (size_t i = 0; i < lhs._chunks.size(); ++i) {
		uint64_t carry = 0;

		for (size_t j = 0; j < rhs._chunks.size(); ++j) {
			uint64_t mul = (uint64_t)(lhs._chunks[i]) * rhs._chunks[j];
			uint64_t sum = (uint64_t)(res_chunks[i + j]) + mul + carry;

			res_chunks[i + j] = (uint32_t)(sum % BASE);
			carry = sum / BASE;
		}

		res_chunks[i + rhs._chunks.size()] += (uint32_t)carry;
	}

	while (res_chunks.size() > 1 && res_chunks.back() == 0)
		res_chunks.pop_back();

	result._chunks = res_chunks;
	result._is_negative = lhs._is_negative ^ rhs._is_negative;

	return result;
}

BigInt BigInt::karatsuba_mul(const BigInt& lhs, const BigInt& rhs) {
	size_t split_length = (lhs._chunks.size() + lhs._chunks.size()) / 4;

	std::vector<uint32_t> lhs_chunks0(lhs._chunks.begin(), (split_length <= lhs._chunks.size()) ? lhs._chunks.begin() + split_length : lhs._chunks.end());
	std::vector<uint32_t> lhs_chunks1;
	if (split_length < lhs._chunks.size())
		lhs_chunks1 = std::vector<uint32_t>(lhs._chunks.begin() + split_length, lhs._chunks.end());
	std::vector<uint32_t> rhs_chunks0(rhs._chunks.begin(), (split_length <= rhs._chunks.size()) ? rhs._chunks.begin() + split_length : rhs._chunks.end());
	std::vector<uint32_t> rhs_chunks1;
	if (split_length < rhs._chunks.size())
		rhs_chunks1 = std::vector<uint32_t>(rhs._chunks.begin() + split_length, rhs._chunks.end());

	BigInt lhs0(lhs_chunks0);
	BigInt lhs1(lhs_chunks1);
	BigInt rhs0(rhs_chunks0);
	BigInt rhs1(rhs_chunks1);
	
	BigInt r2 = (lhs1._chunks.size() == 1 || rhs1._chunks.size() == 1) ? BigInt::simple_mul(lhs1, rhs1) : BigInt::karatsuba_mul(lhs1, rhs1);
	BigInt r0 = (lhs0._chunks.size() == 1 || rhs0._chunks.size() == 1) ? BigInt::simple_mul(lhs0, rhs0) : BigInt::karatsuba_mul(lhs0, rhs0);
	BigInt lhs01 = lhs0 + lhs1;
	BigInt rhs01 = rhs0 + rhs1;
	BigInt tmp = (lhs01._chunks.size() == 1 || rhs01._chunks.size() == 1) ? BigInt::simple_mul(lhs01, rhs01) : BigInt::karatsuba_mul(lhs01, rhs01);
	BigInt r1 = tmp - r2 - r0;

	std::vector<uint32_t> res_chunks2;
	for (size_t i = 0; i < 2 * split_length; i++)
		res_chunks2.push_back(0);
	for (size_t i = 0; i < r2._chunks.size(); i++)
		res_chunks2.push_back(r2._chunks[i]);

	std::vector<uint32_t> res_chunks1;
	for (size_t i = 0; i < split_length; i++)
		res_chunks1.push_back(0);
	for (size_t i = 0; i < r1._chunks.size(); i++)
		res_chunks1.push_back(r1._chunks[i]);

	std::vector<uint32_t> res_chunks0 = r0._chunks;

	BigInt res2(res_chunks2);
	BigInt res1(res_chunks1);
	BigInt res0(res_chunks0);
	BigInt result = res2 + res1 + res0;
	result._is_negative = lhs._is_negative ^ rhs._is_negative;

	while (result._chunks.size() > 1 && result._chunks.back() == 0) {
		result._chunks.pop_back();
	}

	return result;
}

BigInt BigInt::karatsuba_square(const BigInt& number) {
	size_t split_length = number._chunks.size() / 2;

	std::vector<uint32_t> number_chunks0(number._chunks.begin(), (split_length <= number._chunks.size()) ? number._chunks.begin() + split_length : number._chunks.end());
	std::vector<uint32_t> number_chunks1;
	if (split_length < number._chunks.size())
		number_chunks1 = std::vector<uint32_t>(number._chunks.begin() + split_length, number._chunks.end());

	BigInt number0(number_chunks0);
	BigInt number1(number_chunks1);

	BigInt r2 = (number1._chunks.size() == 1) ? BigInt::simple_mul(number1, number1) : BigInt::karatsuba_square(number1);
	BigInt r0 = (number0._chunks.size() == 1) ? BigInt::simple_mul(number0, number0) : BigInt::karatsuba_square(number0);
	BigInt number01 = number0 + number1;
	BigInt tmp = (number01._chunks.size() == 1) ? BigInt::simple_mul(number01, number01) : BigInt::karatsuba_square(number01);
	BigInt r1 = tmp - r2 - r0;

	std::vector<uint32_t> res_chunks2;
	for (size_t i = 0; i < 2 * split_length; i++)
		res_chunks2.push_back(0);
	for (size_t i = 0; i < r2._chunks.size(); i++)
		res_chunks2.push_back(r2._chunks[i]);

	std::vector<uint32_t> res_chunks1;
	for (size_t i = 0; i < split_length; i++)
		res_chunks1.push_back(0);
	for (size_t i = 0; i < r1._chunks.size(); i++)
		res_chunks1.push_back(r1._chunks[i]);

	std::vector<uint32_t> res_chunks0 = r0._chunks;

	BigInt res2(res_chunks2);
	BigInt res1(res_chunks1);
	BigInt res0(res_chunks0);
	BigInt result = res2 + res1 + res0;
	result._is_negative = false;

	while (result._chunks.size() > 1 && result._chunks.back() == 0) {
		result._chunks.pop_back();
	}

	return result;
}

std::pair<BigInt, BigInt> BigInt::div(const BigInt& lhs, const BigInt& rhs) {
	BigInt zero;
	if (rhs == zero)
		throw std::invalid_argument("Division by zero");

	uint32_t shift = leading_zeros(rhs._chunks.back());
	BigInt dividend = BigInt::abs(lhs << shift);
	BigInt divider = BigInt::abs(rhs << shift);

	std::list<uint32_t> quotient_chunks;
	BigInt remainder;
	remainder._chunks.clear();

	for (int i = (int)dividend._chunks.size() - 1; i >= 0; i--) {
		remainder._chunks.insert(remainder._chunks.begin(), dividend._chunks[i]);

		while (!remainder._chunks.empty() && remainder._chunks.back() == 0) {
			remainder._chunks.pop_back();
		}
		if (remainder < divider) {
			quotient_chunks.push_front(0);
			continue;
		}

		uint32_t q = BigInt::estimate_quotient(remainder, divider);

		while (divider * BigInt(q) > remainder) {
			q--;
		}

		quotient_chunks.push_front(q);
		remainder = remainder - BigInt(q) * divider;
	}

	remainder = remainder >> shift;
	while (quotient_chunks.size() > 1 && quotient_chunks.back() == 0) {
		quotient_chunks.pop_back();
	}

	BigInt quotient(std::vector<uint32_t>(quotient_chunks.begin(), quotient_chunks.end()), lhs._is_negative ^ rhs._is_negative);

	if (lhs._is_negative && remainder != zero) {
		remainder = -remainder + BigInt::abs(rhs);
		quotient -= 1;
	}

	while (remainder._chunks.size() > 1 && remainder._chunks.back() == 0) {
		remainder._chunks.pop_back();
	}

	return std::pair<BigInt, BigInt>(quotient, remainder);
}
 
BigInt BigInt::mod(const BigInt& lhs, const BigInt& rhs) {
	BigInt zero;
	if (rhs == zero)
		throw std::invalid_argument("Division by zero");

	uint32_t shift = leading_zeros(rhs._chunks.back());
	BigInt dividend = BigInt::abs(lhs << shift);
	BigInt divider = BigInt::abs(rhs << shift);

	BigInt remainder;
	remainder._chunks.clear();

	for (int i = (int)dividend._chunks.size() - 1; i >= 0; i--) {
		remainder._chunks.insert(remainder._chunks.begin(), dividend._chunks[i]);

		while (!remainder._chunks.empty() && remainder._chunks.back() == 0) {
			remainder._chunks.pop_back();
		}
		if (remainder < divider) {
			continue;
		}

		uint32_t q = BigInt::estimate_quotient(remainder, divider);

		while (divider * BigInt(q) > remainder) {
			q--;
		}

		remainder = remainder - BigInt(q) * divider;
	}

	remainder = remainder >> shift;


	if (lhs._is_negative && remainder != 0)
		remainder = -remainder + BigInt::abs(rhs);

	while (remainder._chunks.size() > 1 && remainder._chunks.back() == 0) {
		remainder._chunks.pop_back();
	}
		
	return remainder;
}

std::tuple<BigInt, BigInt, BigInt> BigInt::extended_gcd(const BigInt& lhs, const BigInt& rhs) {
	BigInt zero;
	BigInt one = 1;
	BigInt a = BigInt::abs(lhs);
	BigInt b = BigInt::abs(rhs);

	BigInt x0 = one, y0 = zero;
	BigInt x1 = zero, y1 = one;

	while (b != zero) {
		auto [q, r] = BigInt::div(a, b);

		BigInt new_x = x0 - q * x1;
		BigInt new_y = y0 - q * y1;

		a = b;
		b = r;

		x0 = x1;
		x1 = new_x;

		y0 = y1;
		y1 = new_y;
	}

	if (lhs._is_negative) x0 = -x0;
	if (rhs._is_negative) y0 = -y0;

	return { a, x0, y0 };
}

BigInt BigInt::gcd(const BigInt& lhs, const BigInt& rhs) {
	BigInt zero;
	if (rhs == zero)
		return lhs;
	return BigInt::gcd(rhs, lhs % rhs);
}

BigInt BigInt::mod_inverse(const BigInt& a, const BigInt& m)
{
	if (a > m) {
		return BigInt::mod_inverse(a % m, m);
	}

	BigInt zero;
	BigInt one = 1;
	auto [g, x, y] = BigInt::extended_gcd(a, m);
	if (g != one)
		throw std::invalid_argument("Modular inverse does not exist");

	if (x < zero)
		x += m;

	return x;
}

BigInt BigInt::left_shift(const BigInt& number, uint32_t shift) {
	if (shift == 0)
		return number;
	uint32_t bit_shift = shift % 32;
	uint32_t chunk_shift = shift / 32;

	std::list<uint32_t> chunks1(number._chunks.begin(), number._chunks.end());

	for (size_t i = 0; i < chunk_shift; i++)
		chunks1.push_front(0);

	std::vector chunks2(chunks1.begin(), chunks1.end());

	if (bit_shift > 0) {
		uint32_t carry = 0;
		for (size_t i = 0; i < chunks2.size(); i++) {
			uint32_t new_carry = chunks2[i] >> (32 - bit_shift);
			chunks2[i] = (chunks2[i] << bit_shift) | carry;
			carry = new_carry;
		}
		if (carry != 0) {
			chunks2.push_back(carry);
		}
	}
	return BigInt(chunks2, number._is_negative);
}

BigInt BigInt::right_shift(const BigInt& number, uint32_t shift) {
	if (shift == 0)
		return number;
	uint32_t bit_shift = shift % 32;
	uint32_t chunk_shift = shift / 32;

	std::list<uint32_t> chunks1(number._chunks.begin(), number._chunks.end());

	for (size_t i = 0; i < chunk_shift; i++) {
		chunks1.pop_front();
		if (chunks1.size() == 0)
			return BigInt();
	}

	std::vector chunks2(chunks1.begin(), chunks1.end());

	if (bit_shift > 0) {
		uint32_t carry = 0;
		for (int i = (int)chunks2.size() - 1; i >= 0; i--) {
			uint32_t new_carry = chunks2[i] << (32 - bit_shift);
			chunks2[i] = (chunks2[i] >> bit_shift) | carry;
			carry = new_carry;
		}
	}
	return BigInt(chunks2, number._is_negative);
}

BigInt BigInt::montgomery(const BigInt& rhs, const BigInt& lhs,const BigInt& module, const BigInt& R, const BigInt& n_prime) {
	BigInt x = rhs * lhs;
	BigInt m = (x * n_prime) % R;
	BigInt t = (x + m * module) / R;

	if (t >= module)
		t -= module;

	return t;
}

BigInt BigInt::montgomery_mul(const BigInt& rhs, const BigInt& lhs, const BigInt& module) { 
	BigInt one = 1;
	uint32_t n = module.bit_length();
	BigInt R = one << n;

	BigInt n_prime = BigInt::mod_inverse(module, R);
	n_prime = R - n_prime;

	BigInt Ra = (lhs * R) % module;
	BigInt Rb = (rhs * R) % module;
	BigInt Rc = BigInt::montgomery(Ra, Rb, module, R, n_prime);

	return BigInt::montgomery(Rc, one, module, R, n_prime);
}

BigInt BigInt::binary_pow(const BigInt& number, const BigInt& degree) {
	if (degree < 0)
		throw std::invalid_argument("Raising to a negative power");

	if (degree == 0)
		return BigInt(1);
	if (degree == 1)
		return number;
	if (number == 0 || number == 1)
		return number;

	std::vector<bool> degree_bits;
	for (auto chunk : degree._chunks) {
		for (size_t i = 0; i < 32; i++)
			degree_bits.push_back((chunk >> i) & 1);
	}

	while (degree_bits.back() == 0) {
		degree_bits.pop_back();
	}

	BigInt acc = number;
	for (int i = (int)degree_bits.size() - 2; i >= 0; i--) {
		acc = BigInt::karatsuba_square(acc);
		if (degree_bits[i] == 1)
			acc *= number;
	}

	return acc;
}

BigInt BigInt::pow(const BigInt& number, const BigInt& degree, uint32_t base) {
	if (degree < 0)
		throw std::invalid_argument("Raising to a negative power");
	if (base > 0 && (base & (base - 1)) != 0 || base == 0)
		throw std::invalid_argument("Base is not a power of 2");
	if (base == 1)
		throw std::invalid_argument("Base cannot be equal to 1");

	if (degree == 0)
		return BigInt(1);
	if (degree == 1)
		return number;
	if (number == 0 || number == 1)
		return number;

	int bit_depth = (size_t)(32 - BigInt::leading_zeros(base)) - 1;

	std::vector<bool> degree_bits;
	for (auto chunk : degree._chunks) {
		for (size_t i = 0; i < 32; i++)
			degree_bits.push_back((chunk >> i) & 1);
	}

	while (degree_bits.back() == 0) {
		degree_bits.pop_back();
	}

	while (degree_bits.size() % bit_depth != 0) {
		degree_bits.push_back(false);
	}

	std::vector<BigInt> factors;

	BigInt acc = 1;
	for (size_t i = 0; i < base; i++) {
		factors.push_back(acc);
		acc *= number;
	}

	acc = 1;
	for (int i = (int)degree_bits.size() - 1; i >= 0; i -= bit_depth) {		
		for (size_t i = 0; i < bit_depth; i++) {
			acc = BigInt::karatsuba_square(acc);
		}

		uint32_t factor_index = 0;
		for (size_t j = 0; j < bit_depth; j++) {
			factor_index |= (uint32_t)degree_bits[i - j] << (bit_depth - j - 1);
		}

		acc *= factors[factor_index];
	}

	return acc;
}

BigInt BigInt::montgomery_pow(const BigInt& number, const BigInt& degree, const BigInt& module, uint32_t base) {
	if (degree < 0)
		throw std::invalid_argument("Raising to a negative power");
	if (base > 0 && (base & (base - 1)) != 0 || base == 0)
		throw std::invalid_argument("Base is not a power of 2");
	if (base == 1)
		throw std::invalid_argument("Base cannot be equal to 1");

	BigInt zero;
	BigInt one = 1;
	uint32_t n = module.bit_length();
	BigInt R = one << n;
	BigInt n_prime = R - BigInt::mod_inverse(module, R);

	BigInt Ra = (number * R) % module;
	BigInt R1 = R % module;

	std::vector<bool> degree_bits;
	for (auto chunk : degree._chunks) {
		for (size_t i = 0; i < 32; ++i) {
			degree_bits.push_back((chunk >> i) & 1);
		}
	}

	while (!degree_bits.empty() && degree_bits.back() == 0) {
		degree_bits.pop_back();
	}

	uint32_t bit_depth = (uint32_t)(32 - BigInt::leading_zeros(base)) - 1;

	while (degree_bits.size() % bit_depth != 0) {
		degree_bits.push_back(false);
	}

	std::vector<BigInt> factors;
	BigInt acc = R1;
	factors.push_back(R1); 
	for (size_t i = 1; i < base; ++i) {
		acc = BigInt::montgomery(acc, Ra, module, R, n_prime);
		factors.push_back(acc);
	}

	acc = R1;

	for (int i = (int)degree_bits.size() - 1; i >= 0; i -= bit_depth) {
		for (uint32_t j = 0; j < bit_depth; ++j) {
			acc = BigInt::montgomery(acc, acc, module, R, n_prime);
		}

		uint32_t factor_index = 0;
		for (uint32_t j = 0; j < bit_depth; ++j) {
			factor_index |= (uint32_t)degree_bits[i - j] << (bit_depth - j - 1);
		}

		if (factor_index != 0) {
			acc = BigInt::montgomery(acc, factors[factor_index], module, R, n_prime);
		}
	}

	acc = BigInt::montgomery(acc, one, module, R, n_prime);
	return acc;
}

BigInt& BigInt::operator =(const BigInt& other) {
	_is_negative = other._is_negative;
	_chunks = other._chunks;
	return *this;
}

BigInt& BigInt::operator =(const std::string& number_str) {
	*this = BigInt(number_str);
	return *this;
}

BigInt& BigInt::operator +=(const BigInt& other) {
	*this = *this + other;
	return *this;
}

BigInt& BigInt::operator -=(const BigInt& other) {
	*this = *this - other;
	return *this;
}

BigInt& BigInt::operator *=(const BigInt& other) {
	*this = *this * other;
	return *this;
}

BigInt& BigInt::operator /=(const BigInt& other) {
	*this = *this + other;
	return *this;
}

BigInt& BigInt::operator %=(const BigInt& other) {
	*this = *this % other;
	return *this;
}

BigInt& BigInt::operator >>=(uint32_t shift) {
	*this = *this >> shift;
	return *this;
}

BigInt& BigInt::operator <<=(uint32_t shift) {
	*this = *this << shift;
	return *this;
}

BigInt BigInt::operator +(const BigInt& other) const  {
	return BigInt::sum(*this, other);
}

BigInt BigInt::operator -(const BigInt& other) const {
	return BigInt::sub(*this, other);
}

BigInt BigInt::operator -() const {
	BigInt result = *this;
	result._is_negative = !result._is_negative;
	return result;
}

BigInt BigInt::operator *(const BigInt& other) const {
	return BigInt::karatsuba_mul(*this, other);
}

BigInt BigInt::operator /(const BigInt& other) const {
	return BigInt::div(*this, other).first;
}

BigInt BigInt::operator %(const BigInt& other) const {
	return BigInt::mod(*this, other);
}

BigInt BigInt::operator >>(uint32_t shift) const {
	return BigInt::right_shift(*this, shift);
}

BigInt BigInt::operator <<(uint32_t shift) const {
	return BigInt::left_shift(*this, shift);
}

bool BigInt::operator ==(const BigInt& other) const {
	BigInt zero;
	if (_chunks == zero._chunks && other._chunks == zero._chunks)
		return true;
	return BigInt::abs_cmp(*this, other) == 0 && _is_negative == other._is_negative;
}

bool BigInt::operator !=(const BigInt& other) const {
	return !(*this == other);
}

bool BigInt::operator >(const BigInt& other) const {
	if (*this == other)
		return false;

	if (_is_negative && !other._is_negative)
		return false;
	if (!_is_negative && other._is_negative)
		return true;

	int check = BigInt::abs_cmp(*this, other) > 0;
	if (check < 0 && _is_negative || check > 0 && !_is_negative)
		return true;
	return false;
}

bool BigInt::operator <(const BigInt& other) const {
	return other > *this;
}

bool BigInt::operator >=(const BigInt& other) const {
	return *this == other || *this > other;
}

bool BigInt::operator <=(const BigInt& other) const {
	return *this == other || *this < other;
}