#pragma once 

#include <string>
#include <algorithm>

class bigint {
public:
	bigint(std::string str) {
		_data = str;
		_isPositive = str[0] != '-';
		if(str[0] == '-' || str[0] == '+') {
			_data.erase(0, 1);
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