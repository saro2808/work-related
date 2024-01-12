#pragma once

#include <string>
#include <iostream>
#include <cstdint>
#include <fstream>

namespace Numbers {

const int prime_count = 9592; 

int64_t pick_random_prime() {
	std::ifstream primes("data/primes.txt");
	if (!primes.is_open()) {
		std::cerr << "Error opening file: primes.txt" << std::endl;
		std::abort();
	}
	int prime_to_pick = rand() % prime_count;
	std::string line;
	int current_line = 0;
	while (std::getline(primes, line)) {
		++current_line;
		if (current_line == prime_to_pick) {
			return std::stoll(line);
		}
	}
	return -1; // should not reach here
}

int64_t gcd_extended(int64_t a, int64_t b, int64_t& x, int64_t& y) {
	if (a == 0) {
		x = 0;
		y = 1;
		return b;
	}
	int64_t x1, y1;
	int64_t gcd = gcd_extended(b % a, a, x1, y1);

	x = y1 - (b / a) * x1;
	y = x1;
	return gcd;
}

int64_t mod_inverse(int64_t e, int64_t t) {
	int64_t x, y;
	int64_t gcd = gcd_extended(e, t, x, y);
	if (gcd != 1) {
		return -1; // no inverse exists
	}
	return (x % t + t) % t;
}

int64_t mod_pow(int64_t base, int64_t exp, int64_t mod) {
	int64_t result = 1;
	base %= mod;
	while (exp > 0) {
		if (exp % 2 == 1) {
			result = (result * base) % mod;
		}
		exp >>= 1;
		base = (base * base) % mod;
	}
	return result;
}

} // namespace Numbers
