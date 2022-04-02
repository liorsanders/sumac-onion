#pragma once 

#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>

class bigint {
public:
	bigint(std::string str) {
		_data = str;
		_isPositive = str[0] != '-';
		if(str[0] == '-' || str[0] == '+') {
			_data.erase(0, 1);
		}
		while(_data[0] == '0') {
			_data.erase(0, 1);
		}
		if(_data.size() == 0) {
			_data = "0";
		}
	}
	bigint() {
		_data = "0";
		_isPositive = true;
	}
	std::string toString() const {
		if(!_isPositive) {
			return "-" + _data;
		}
		return _data;
	}
	bool isPositive() const {
		return _isPositive;
	}
	// a - b
	// static bigint* multiply(const bigint* number, const bigint* multiplyBy) {
	// 	bigint* mult = new bigint(multiplyBy->toString());
	// 	bigint* res = new bigint(number->toString());
	// 	if(multiplyBy->isPositive()) {
	// 		while(mult->toString()[0] != '0' ) {
	// 			bigint* temp = new bigint(res->toString());
	// 			delete res;
	// 			res = add(temp, number);
	// 			delete temp;
				
	// 		}
		
	// 	}
	// }
	friend std::ostream& operator<<(std::ostream& os, const bigint& number) {
		os << number.toString();
		return os;
	}
	bigint operator+(const bigint& other) {
		bigint res;
		res.add(*this);
		res.add(other);
		return res;
	}
	bigint operator-(const bigint& other) {
		bigint res;
		res.add(*this);
		res.sub(other);
		return res;
	}
	bigint operator*(const bigint& other) {
		bigint res;
		res.add(*this);
		res.mul(other);
		return res;
	}
	void mul(const bigint& other) {
		bigint temp = other;
		bigint initial = *this;
		if(other._data == "0") {
			_data = "0";
			_isPositive = true;
		}
		else if(other.isPositive()) {
			_data = bigint::multiply(_data, other._data);
		}
		else {
			_isPositive = !_isPositive;
			temp._isPositive = true;
			this->mul(temp);
		}
	}
	void div(const bigint& other) {
		__uint128_t num = bigint::bigint_to_uint128(other);
		bigint* a = get_uint128(num);
		std::cout << *a << std::endl;
		delete a;
		_data = bigint::longDivision(_data, num);
	}
	void sub(const bigint& other) {
		bigint* temp = bigint::sub(this, &other);
		this->_data = temp->_data;
		this->_isPositive = temp->_isPositive;
		delete temp;
	}
	void add(const bigint& other) {
		bigint* temp = bigint::add(this, &other);
		this->_data = temp->_data;
		this->_isPositive = temp->_isPositive;
		delete temp;
	}

	static bigint* sub(const bigint* a, const bigint* b) {
		std::string str1 = a->toString();
		std::string str2 = b->toString();
		if(a->isPositive() && b->isPositive()) {
			bigint* diff = new bigint(findDiff(str1, str2));
			if(!isSmaller(str1, str2)) {
				return diff;
			}
			diff->_isPositive = false;
			return diff;
		}
		else if(a->isPositive() && !b->isPositive()) {
			bigint* second = new bigint(b->toString());
			second->_isPositive = true;
			bigint* added = add(a, second);
			delete second;
			return added;
		}
		else if(!a->isPositive() && b->isPositive()) {
			bigint* first = new bigint(a->toString());
			bigint* second = new bigint(b->toString());
			first->_isPositive = true;
			second->_isPositive = true;
			bigint* added = add(first, second);
			delete first;
			delete second;
			added->_isPositive = false;
			return added;
		}
		else {
			return sub(b, a);
		}
	} 
	static bigint* add(const bigint* a, const bigint* b) {
		std::string res = "";
		int carry = 0;
		if(a->isPositive() && b->isPositive()) {
			std::string str1 = a->toString();
			std::string str2 = b->toString();

			bigint::pad(str1, str2);
			
			for(int i=str1.size()-1;i >= 0;i--) {
				int sum = (str1[i] - '0') + (str2[i] - '0') + carry;
				res.insert(0, 1, (char)(sum % 10 + '0'));
				carry = sum / 10;
			}
			res.insert(0, 1, (char)(carry + '0'));
			return new bigint(res);
		}
		else if(a->isPositive() && !b->isPositive()) {
			return sub(a, b);
		}
		else if(!a->isPositive() && b->isPositive()) {
			return sub(b, a);
		}
		else {
			bigint* second = new bigint(b->toString());
			second->_isPositive = true;
			bigint* res = sub(a, second);
			delete second;
			return res;
		}
		
	}
	
private: 
	static __uint128_t bigint_to_uint128(const bigint& number) {
		__uint128_t res = 0;
		std::string num = number._data;
		for(int i=0;i<num.size();i++) {
			res = res * 10 + num[i] - '0';
		}
		return res;
	}
	bigint* get_uint128(__uint128_t n) {
		if (n == 0)  return new bigint();

		char str[40] = {0}; // log10(1 << 128) + ''
		char *s = str + sizeof(str) - 1; // start at the end
		while (n != 0) {
			if (s == str) return new bigint("-1"); // never happens

			*--s = "0123456789"[n % 10]; // save last digit
			n /= 10;                     // drop it
		}
		char* res = new char[40];
		sprintf(res, "%s", s);
		std::string result = res;
		delete[] res;
		return new bigint(result);
	}
	std::string longDivision(std::string number,  __uint128_t divisor)
	{
		// As result can be very large store it in string
		std::string ans;
	
		// Find prefix of number that is larger
		// than divisor.
		int idx = 0;
		int temp = number[idx] - '0';
		while (temp < divisor)
			temp = temp * 10 + (number[++idx] - '0');
		std::cout << "temp: " << temp << std::endl;
		// Repeatedly divide divisor with temp. After
		// every division, update temp to include one
		// more digit.
		while (number.size() > idx) {
			// Store result in answer i.e. temp / divisor
			ans += (temp / divisor) + '0';
			std::cout << ans << std::endl;
			// Take next digit of number
			temp = (temp % divisor) * 10 + number[++idx] - '0';
		}
	
		// If divisor is greater than number
		if (ans.length() == 0)
			return "0";
	
		// else return ans
		return ans;
	}

	static std::string multiply(std::string num1, std::string num2)
	{
		int len1 = num1.size();
		int len2 = num2.size();
		if (len1 == 0 || len2 == 0)
		return "0";
	
		// will keep the result number in vector
		// in reverse order
		std::vector<int> result(len1 + len2, 0);
	
		// Below two indexes are used to find positions
		// in result.
		int i_n1 = 0;
		int i_n2 = 0;
		
		// Go from right to left in num1
		for (int i=len1-1; i>=0; i--)
		{
			int carry = 0;
			int n1 = num1[i] - '0';
	
			// To shift position to left after every
			// multiplication of a digit in num2
			i_n2 = 0;
			
			// Go from right to left in num2            
			for (int j=len2-1; j>=0; j--)
			{
				// Take current digit of second number
				int n2 = num2[j] - '0';
	
				// Multiply with current digit of first number
				// and add result to previously stored result
				// at current position.
				int sum = n1*n2 + result[i_n1 + i_n2] + carry;
	
				// Carry for next iteration
				carry = sum/10;
	
				// Store result
				result[i_n1 + i_n2] = sum % 10;
	
				i_n2++;
			}
	
			// store carry in next cell
			if (carry > 0)
				result[i_n1 + i_n2] += carry;
	
			// To shift position to left after every
			// multiplication of a digit in num1.
			i_n1++;
		}
	
		// ignore '0's from the right
		int i = result.size() - 1;
		while (i>=0 && result[i] == 0)
		i--;
	
		// If all were '0's - means either both or
		// one of num1 or num2 were '0'
		if (i == -1)
		return "0";
	
		// generate the result string
		std::string s = "";
		
		while (i >= 0)
			s += std::to_string(result[i--]);
	
		return s;
	}
	static void pad(std::string& a, std::string& b) {
		if(a.size() < b.size()) {
			a.insert(0, b.size() - a.size(), '0');
		}
		else if(a.size() > b.size()) {
			b.insert(0, a.size() - b.size(), '0');
		}
	}
	static bool isSmaller(std::string str1, std::string str2)
	{
		// Calculate lengths of both string
		int n1 = str1.length(), n2 = str2.length();
	
		if (n1 < n2)
			return true;
		if (n2 < n1)
			return false;
	
		for (int i = 0; i < n1; i++)
			if (str1[i] < str2[i])
				return true;
			else if (str1[i] > str2[i])
				return false;
	
		return false;
	}
	
// Function for find difference of larger numbers
	static std::string findDiff(std::string str1, std::string str2)
	{
		// Before proceeding further, make sure str1
		// is not smaller
		if (isSmaller(str1, str2))
			std::swap(str1, str2);
	
		// Take an empty string for storing result
		std::string str = "";
	
		// Calculate length of both string
		int n1 = str1.length(), n2 = str2.length();
	
		// Reverse both of strings
		std::reverse(str1.begin(), str1.end());
		std::reverse(str2.begin(), str2.end());
	
		int carry = 0;
	
		// Run loop till small string length
		// and subtract digit of str1 to str2
		for (int i = 0; i < n2; i++) {
			// Do school mathematics, compute difference of
			// current digits
	
			int sub
				= ((str1[i] - '0') - (str2[i] - '0') - carry);
	
			// If subtraction is less then zero
			// we add then we add 10 into sub and
			// take carry as 1 for calculating next step
			if (sub < 0) {
				sub = sub + 10;
				carry = 1;
			}
			else
				carry = 0;
	
			str.push_back(sub + '0');
		}
	
		// subtract remaining digits of larger number
		for (int i = n2; i < n1; i++) {
			int sub = ((str1[i] - '0') - carry);
	
			// if the sub value is -ve, then make it positive
			if (sub < 0) {
				sub = sub + 10;
				carry = 1;
			}
			else
				carry = 0;
	
			str.push_back(sub + '0');
		}
	
		// reverse resultant string
		std::reverse(str.begin(), str.end());
		return str;
	}
	
	std::string _data;
	bool _isPositive;
};