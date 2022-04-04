#include "RSA.hpp"
#include <iostream>


int main() {
	RSA rsa;
	std::string message = "Ben Zonna";
	std::cout << "encrypting: " << message << std::endl;
	std::string res = rsa.encrypt(message);
	std::cout << "encrypted: " << res << std::endl;
	std::string recovered = rsa.decrypt(res);
	std::cout << "recovered: " << recovered << std::endl;
	return 0;
}