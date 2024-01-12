#pragma once

#include <string>
#include <cstdint>

#define BLOCK_SIZE 256

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
