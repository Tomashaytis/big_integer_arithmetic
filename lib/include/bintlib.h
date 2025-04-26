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
	std::list<uint32_t> _chunks;
	uint64_t _base;
	void _parse_number(std::string number);
	std::string _concat_number() const;
public:
	BigInt(std::string number);
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

	friend std::ostream& operator <<(std::ostream& os, const BigInt& number);
};