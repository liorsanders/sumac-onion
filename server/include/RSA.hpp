#pragma once 

#define KEY_SIZE 1024

#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

#define RSA_KEY_DIR "./"

class RSA {
public:
	RSA();
	std::string encrypt(const std::string message);
	std::string decrypt(const std::string cipher);
private:
	void generateKeys();
	void loadKeys();
	CryptoPP::RSA::PrivateKey _privateKey;
	CryptoPP::RSA::PublicKey _publicKey;
	//CryptoPP::AutoSeededRandomPool rng;
};