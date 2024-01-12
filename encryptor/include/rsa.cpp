#include <numeric>

#include "rsa.hpp"
#include "numbers.hpp"

void RSA::set_keys() {
	_public_key = (1 << 16) + 1;
	
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
	
	_private_key = Numbers::mod_inverse(_public_key, totient);
	if (_private_key == -1) {
		throw "Error generating mod inverse: mod inverse does not exist";
	}
}

void RSA::encode_block(const std::string& message, int start_index, std::string& encoded) {
	int end_index = std::min<int64_t>(start_index + BLOCK_SIZE, message.length());
	for (int i = start_index; i < end_index; ++i) {
		char symbol = message[i];
		int64_t encrypted_text = 1;
		encrypted_text = Numbers::mod_pow(symbol, _public_key, _n);
		encoded += std::to_string(encrypted_text) + ' ';
	}
}

void RSA::decode_block(const std::string& encoded, int start_index, std::string& decoded) {
	int end_index = std::min<int64_t>(start_index + BLOCK_SIZE, encoded.length());
	for (int i = start_index; i < end_index; ++i) {
		int64_t coded = 0;
		while (encoded[i] >= '0' && encoded[i] <= '9') {
			coded *= 10;
			coded += encoded[i++] - '0';
		}
		if (coded == 0) {
			continue;
		}
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
