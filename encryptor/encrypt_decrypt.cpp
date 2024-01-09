#include <iostream>
#include <set>
#include <vector>
#include <numeric>
#include <fstream>

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
int64_t encrypt(double message) {
	int e = public_key;
	int64_t encrpyted_text = 1;
	while (e--) {
		encrpyted_text *= message;
		encrpyted_text %= n;
	}
	return encrpyted_text;
}
// to decrypt the given number
int64_t decrypt(int encrpyted_text) {
	int d = private_key;
	int64_t decrypted = 1;
	while (d--) {
		decrypted *= encrpyted_text;
		decrypted %= n;
	}
	return decrypted;
}
// first converting each character to its ASCII value and then encoding it
// then decoding the number to get the ASCII and converting it to character
std::vector<int> encoder(std::string message) {
	std::vector<int> form;
	// calling the encrypting function in encoding function
	for (auto& letter : message) {
		form.push_back(encrypt((int)letter));
	}
	return form;
}
std::string decoder(std::vector<int> encoded) {
	std::string s;
	// calling the decrypting function decoding function
	for (auto& num : encoded) {
		s += decrypt(num);
	}
	return s;
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
		std::vector<int> coded = encoder(line);
		for (auto& p: coded) {
			encrypted << p;
			decrypted << (char)decrypt(p);
		}
		encrypted << '\n';
		decrypted << '\n';
	}
	return 0;
}

