#include "bigint.hpp"

#include <iostream> 
#include <numeric>
#include <boost/multiprecision/cpp_int.hpp>
#include <bits/stdc++.h>

int main() {
	boost::multiprecision::cpp_int num("34534857928475927498572984759827948572398475");
	std::cout << num % boost::multiprecision::cpp_int("2342342342342342") << std::endl;
	return 0;
}