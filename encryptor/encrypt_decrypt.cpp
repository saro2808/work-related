#include <iostream>
#include <fstream>

#include "include/rsa.hpp"

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

