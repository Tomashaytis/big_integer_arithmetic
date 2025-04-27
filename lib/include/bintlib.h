#pragma once

#include <list>
#include <tuple>
#include <vector>
#include <string>
#include <iostream>
#include <exception>

class BigInt
{
private:
	bool _is_negative;
	std::vector<uint32_t> _chunks;
public:
	static const uint64_t BASE = (uint64_t)UINT32_MAX + 1;
	BigInt(std::string number);
	BigInt(std::vector<uint32_t>, bool);
	static std::vector<uint32_t> parse_number(std::string number, uint64_t base = (uint64_t)UINT32_MAX + 1);
	static std::string concat_number(std::vector<uint32_t> chunks, bool is_negative = false, uint64_t base = (uint64_t)UINT32_MAX + 1);
	static BigInt sum(const BigInt& lhs, const BigInt& rhs);
	static BigInt sub(const BigInt& lhs, const BigInt& rhs);
	static BigInt simple_mul(const BigInt& lhs, const BigInt& rhs);
	static BigInt karatsuba_mul(const BigInt& lhs, const BigInt& rhs);
	static std::pair<BigInt, BigInt> div(const BigInt& lhs, const BigInt& rhs);
	static BigInt mod(const BigInt& lhs, const BigInt& rhs);
	static BigInt gcd(const BigInt& lhs, const BigInt& rhs);
	static std::tuple<BigInt, BigInt, BigInt> extended_gcd(const BigInt& lhs, const BigInt& rhs);
	static BigInt left_shift(const BigInt& number, int shift);
	static BigInt right_shift(const BigInt& number, int shift);
	static BigInt montgomery_mul(const BigInt& rhs, const BigInt& lhs, const BigInt& module);
	static BigInt pow(const BigInt& number, const BigInt& degree, int base = 2);
	static BigInt montgomery_pow(const BigInt& rhs, const BigInt& lhs, const BigInt& module, int base = 2);
	static size_t compare_abs_bigints(const BigInt& number1, const BigInt& number2);
	static BigInt sub_chunks(const BigInt& lhs, const BigInt& rhs);

	std::string get_number();
	BigInt& operator=(const BigInt& other);
	BigInt operator+(const BigInt& other) const;
	BigInt operator-(const BigInt& other) const;
	BigInt operator-() const;
	BigInt operator*(const BigInt& other) const;
	BigInt operator%(const BigInt& other) const;
	
	friend std::ostream& operator <<(std::ostream& os, const BigInt& number);

};