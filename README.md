## bintlib library 
The library for working with large numbers.

Based on C++.

**Features:**
- [x] Storage in base 2^32 notation
- [x] Conversion to string, double
- [x] Summation and substruction
- [x] Simple multiplication
- [x] Karatsuba multiplication
- [x] Karatsuba squaring
- [x] Integer division
- [x] Remainder of division 
- [x] Left and right shifts
- [x] GCD by Euclidian algorithm
- [x] GCD by extended Euclidian algorithm
- [x] Modular inverse
- [x] Montgomery multiplication by module
- [x] Binary and q-ary raising to a power
- [x] Montgomery raising to a power by module

**Statistics:**
| Operation       | Algorithm       | Time, s   | status |
|-----------------|-----------------|-----------|--------|
| summation       | basic           | 0,0000182 | valid  |
| substruction    | basic           | 0,0000057 | valid  |
| multiplication  | karatsuba       | 0,0040018 | valid  |
| squaring        | karatsuba       | 0,0030905 | valid  |
| division        | fast            | 0,0012942 | valid  |
| remainder       | fast            | 0,0012712 | valid  |
| left shift      | basic           | 0,0000074 | valid  |
| right shift     | basic           | 0,0000151 | valid  |
| gcd             | euclidean       | 0,0626498 | valid  |
| modular inverse | euclidean (ex)  | 0,002094  | valid  |
| power           | binary          | 0,0045132 | valid  |
| power           | q-ary (4)       | 0,0042703 | valid  |
| power           | q-ary (8)       | 0,0046538 | valid  |
| power           | q-ary (16)      | 0,0046578 | valid  |
| multiplication  | montgomery      | 0,0615283 | valid  |
| power           | montgomery (2)  | 0,1332807 | valid  |
| power           | montgomery (4)  | 0,2058758 | valid  |
| power           | montgomery (8)  | 0,2022511 | valid  |
| power           | montgomery (16) | 0,3111579 | valid  |
