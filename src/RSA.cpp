#include "RSA.hpp"

#include <cryptopp/osrng.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/rsa.h>
#include <cryptopp/files.h>
#include <cryptopp/integer.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

//using CryptoPP::RSA::PrivateKey;


RSA::RSA() {
	if(std::ifstream(std::string(RSA_KEY_DIR) + "rsaparams.dat").good()) {
		loadKeys();
	} else {
		std::cout << "keys file not found" << std::endl;
		generateKeys();
	}
}

std::string RSA::decrypt(const std::string cipher) {
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::Integer c((std::string("0x") + cipher).c_str());
	std::string recovered;
	CryptoPP::Integer r = this->_privateKey.CalculateInverse(rng, c);
	recovered.resize(r.MinEncodedSize());
	r.Encode((CryptoPP::byte *)recovered.data(), recovered.size());
	return recovered;
}

std::string RSA::encrypt(const std::string message) {
	CryptoPP::Integer m((const CryptoPP::byte *)message.data(), message.size());
	CryptoPP::Integer c = this->_publicKey.ApplyFunction(m);

	std::stringstream ss;
	ss << std::hex << c;
	return ss.str();
}

void RSA::loadKeys() {
	//load keys
	CryptoPP::InvertibleRSAFunction params;
	CryptoPP::FileSource input((std::string(RSA_KEY_DIR) + "rsaparams.dat").c_str(), true);
	params.BERDecode(input);

	//store in object 
	this->_privateKey = CryptoPP::RSA::PrivateKey(params);
	this->_publicKey = CryptoPP::RSA::PublicKey(params);
	std::cout << "Loaded keys" << std::endl;
}

void RSA::generateKeys() {
	//generate random keys
	CryptoPP::AutoSeededRandomPool rng;
	CryptoPP::InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize(rng, KEY_SIZE);

	//save keys 
	CryptoPP::FileSink output((std::string(RSA_KEY_DIR) + "rsaparams.dat").c_str());
	params.DEREncode(output);
	
	//initialize keys
	this->_privateKey = CryptoPP::RSA::PrivateKey(params);
	this->_publicKey = CryptoPP::RSA::PublicKey(params);
	std::cout << "Generated keys" << std::endl;
}