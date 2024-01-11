#include <iostream>
#include <set>
#include <vector>
#include <numeric>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <sstream>

#define BLOCK_SIZE 256

namespace Numbers {

const int prime_count = 9592; 

int64_t pick_random_prime() {
	std::ifstream primes("primes.txt");
	if (!primes.is_open()) {
		std::cerr << "Error opening file: primes.txt" << std::endl;
		return -1;
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
		exp = exp >> 1;
		base = (base * base) % mod;
	}
	return result;
}

} // namespace Numbers

class RSA {
	int64_t _n;
	int64_t _public_key;
	int64_t _private_key;
	
	void set_keys();
	
	void encode_block(const std::string& message, int start_index, std::string& encoded);
	void decode_block(const std::string& encoded, int start_index, std::string& decoded);
public:
	RSA() {
		set_keys();
	}
	std::string encode(const std::string& message);
	std::string decode(const std::string& encoded);
};

void RSA::set_keys() {
	int e = (1 << 16) + 1;
	_public_key = e;
	
	int64_t prime1 = Numbers::pick_random_prime();
	int64_t prime2 = Numbers::pick_random_prime();
	while (prime2 == prime1) {
		prime2 = Numbers::pick_random_prime();
	}
	if (prime1 == -1 || prime2 == -1) {
		throw "Error generating primes";
	}
	_n = prime1 * prime2;
	int64_t totient = std::lcm(prime1 - 1, prime2 - 1); // Charmichael's totient function
	int64_t d = 2;
	d = Numbers::mod_inverse(e, totient);
	if (d == -1) {
		throw "Error generating mod inverse: mod inverse does not exist";
	}
	_private_key = d;
}

void RSA::encode_block(const std::string& message, int start_index, std::string& encoded) {
	int end_index = start_index + BLOCK_SIZE;
	if (end_index > message.length()) {
		end_index = message.length();
	}
	for (int i = start_index; i < end_index; ++i) {
		char symbol = message[i];
		int64_t e = _public_key;
		int64_t encrypted_text = 1;
		encrypted_text = Numbers::mod_pow(symbol, _public_key, _n);
		encoded += std::to_string(encrypted_text) + ' ';
	}
}

void RSA::decode_block(const std::string& encoded, int start_index, std::string& decoded) {
	int end_index = start_index + BLOCK_SIZE;
	if (end_index > encoded.length()) {
		end_index = encoded.length();
	}
	for (int i = start_index; i < end_index; ++i) {
		int64_t coded = 0;
		while (encoded[i] >= '0' && encoded[i] <= '9') {
			coded *= 10;
			coded += encoded[i++] - '0';
		}
		if (coded == 0) {
			continue;
		}
		int64_t d = _private_key;
		int64_t decrypted = 1;
		decrypted = Numbers::mod_pow(coded, _private_key, _n);
		decoded += (char)decrypted;
	}
}

std::string RSA::encode(const std::string& message) {
	std::string encoded;
	for (int i = 0; i < message.length(); i += BLOCK_SIZE) {
		encode_block(message, i, encoded);
	}
	return encoded;
}

std::string RSA::decode(const std::string& encoded) {
	std::string decoded;
	for (int i = 0; i < encoded.size(); i += BLOCK_SIZE) {
		decode_block(encoded, i, decoded);
	}
	return decoded;
}

int main() {
	RSA rsa;
	std::ifstream in("document.docx");
	std::fstream encrypted("encrypted.docx", std::fstream::in | std::fstream::out);
	std::fstream decrypted("decrypted.docx", std::fstream::in | std::fstream::out);
	if (!in.is_open()) {
		std::cerr << "Error opening document.docx\n";
		std::abort();
	}
	if (!encrypted.is_open()) {
		std::cerr << "Error opening encrypted.docx\n";
		std::abort();
	}
	std::string line;
	while (std::getline(in, line)) {
		encrypted << rsa.encode(line) << '\n';
	}

	if (!decrypted.is_open()) {
		std::cerr << "Error opening decrypted.docx\n";
		std::abort();
	}
	encrypted.seekg(0, std::ios::beg);
	while (std::getline(encrypted, line)) {
		decrypted << rsa.decode(line) << '\n';
	}
	return 0;
}

