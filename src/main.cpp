#include "bigint.hpp"

#include <iostream> 


int main() {
	std::string input;
	bigint* first = nullptr;
	bigint* second = nullptr;
	bigint* res = nullptr;
	while(true) {
		std::cout << "enter number 1: ";
		std::getline(std::cin, input);
		if(input[input.size()-1] == '\n') {
			input.erase(input.size()-1);
		}
		first = new bigint(input);
		std::cout << "enter number 2: ";
		std::getline(std::cin, input);
		if(input[input.size()-1] == '\n') {
			input.erase(input.size()-1);
		}
		second = new bigint(input);
		std::cout << "enter 1 to add, 2 to subtract: ";
		std::getline(std::cin, input);
		if(input[0] == '1') {
			res = bigint::add(first, second);
		}
		else if(input[0] == '2') {
			res = bigint::sub(first, second);
		}
		std::cout << "res: " << res->toString() << std::endl;
		if(res) {
			delete res;
		}
		if(first) {
			delete first;
		}
		if(second) {
			delete second;
		}
	}
	return 0;
}