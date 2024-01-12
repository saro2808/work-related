#include <numeric>

#include "rsa.hpp"
#include "numbers.hpp"

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