#include <iostream>
#include <set>
#include <vector>
#include <numeric>
#include <fstream>

#define BLOCK_SIZE 256

std::set<int> prime; // this set will be the collection of prime numbers, where we can select random primes p and q
int public_key;
int private_key;
int n;
// we will run the function only once to fill the set of prime numbers
void primefiller() {
	// method used to fill the primes set is seive of eratosthenes (a method to collect prime numbers)
	std::vector<bool> seive(250, true);
	seive[0] = false;
	seive[1] = false;
	for (int i = 2; i < 250; ++i) {
		for (int j = i * 2; j < 250; j += i) {
			seive[j] = false;
		}
	}
	for (int i = 0; i < seive.size(); ++i) {
		if (seive[i]) {
			prime.insert(i);
		}
	}
}
// picking a random prime number and erasing that prime number from list because p!=q
int pickrandomprime() {
	int k = rand() % prime.size();
	auto it = prime.begin();
	while (k--) {
		++it;
	}
	int ret = *it;
	prime.erase(it);
	return ret;
}
void setkeys() {
	int prime1 = pickrandomprime(); // first prime number
	int prime2 = pickrandomprime(); // second prime number
	n = prime1 * prime2;
	int fi = (prime1 - 1) * (prime2 - 1);
	int e = 2;
	while (1) {
		if (std::gcd(e, fi) == 1) {
			break;
		}
		++e;
	} // d = (k*Φ(n) + 1) / e for some integer k
	public_key = e;
	int d = 2;
	while (1) {
		if ((d * e) % fi == 1) {
			break;
		}
		++d;
	}
	private_key = d;
}
// to encrypt the given number
void encrypt(const std::string& message, int start_index, std::string& encoded) {
	const int end_index = start_index + BLOCK_SIZE;
	for (int i = start_index; i < end_index; ++i) {
		char symbol = message[i];
		int e = public_key;
		int64_t encrypted_text = 1;
		while (e--) {
			encrypted_text *= symbol;
			encrypted_text %= n;
		}
		encoded += encrypted_text + ' ';
	}
}
// to decrypt the given number
void decrypt(const std::vector<int>& encoded, int start_index, std::string& decoded) {
	const int end_index = start_index + BLOCK_SIZE;
	for (int i = 0; i < BLOCK_SIZE; ++i) {
		int d = private_key;
		int64_t decrypted = 1;
		while (d--) {
			decrypted *= encoded[i];
			decrypted %= n;
		}
		decoded += (char)decrypted;
	}
}
// first converting each character to its ASCII value and then encoding it
// then decoding the number to get the ASCII and converting it to character
std::string encoder(const std::string& message) {
	std::string encoded;
	for (int i = 0; i < message.length(); i += BLOCK_SIZE) {
		encrypt(message, i, encoded);
	}
	return encoded;
}
std::string decoder(const std::vector<int>& encoded) {
	std::string decoded;
	for (int i = 0; i < encoded.size(); i += BLOCK_SIZE) {
		decrypt(encoded, i, decoded);
	}
	return decoded;
}
int main() {
	primefiller();
	setkeys();
	std::ifstream in("document.docx");
	std::fstream encrypted("encrypted.docx", std::fstream::in | std::fstream::out);
	std::fstream decrypted("decrypted.docx", std::fstream::in | std::fstream::out);
	if (!in.is_open()) {
		std::cout << "Error opening document.docx\n";
	}
	if (!encrypted.is_open()) {
		std::cout << "Error opening encrypted.docx\n";
	}
	if (!decrypted.is_open()) {
		std::cout << "Error opening decrypted.docx\n";
	}
	std::string line;
	while (std::getline(in, line)) {
		std::string coded = encoder(line);
		encrypted << coded;
		encrypted << '\n';
	}
	std::vector<int> encoded;
	while (std::getline(encrypted, line)) {
		std::string delimiter = " ";
		size_t pos = 0;
		std::string token;
		while ((pos = line.find(delimiter)) != std::string::npos) {
			token = line.substr(0, pos);
			encoded.push_back(stoi(token));
			line.erase(0, pos + delimiter.length());
		}
		encoded.push_back(std::stoi(line));
		decrypted << decoder(encoded);
		decrypted << '\n';
	}
	return 0;
}

