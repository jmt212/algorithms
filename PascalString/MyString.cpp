#include <iostream>
#include <algorithm>
#include <string>
#include "MyString.h"

using namespace std;
// Default constructor
String::String() {
	arr = new char[1];
	len = 0;
	arr[0] = static_cast<char>(len);
}

// Constructor. Converts a C-string to a String object
String::String(const char *s) : String(s, false) {}

String::String(const char *s, bool pascal) {
	if (pascal) {
		const char *tmp = s;
		char num = s[0];				 
		len = static_cast<int>(num);
		arr = new char[len + 1];
		for (int i = 0; i <= len; i++) {
			arr[i] = s[i];
		}
	}
	else {
		len = strlen(s);

		char size = static_cast<char>(len);
		arr = new char[len + 1];
		arr[0] = size;

		for (int i = 1, p = 0; p < len; i++, p++) {
			arr[i] = s[p];
		}
	}
}

// Copy constructor.
String::String(const String &obj) {
	len = obj.len;
	arr = new char[len + 1];
	arr[0] = static_cast<char>(len);
	for (auto i = 1; i <= len; ++i) {
		arr[i] = obj.arr[i];
	}
}

// Move constructor.
String::String(String &&obj) {
	len = obj.len;
	arr = obj.arr;
	obj.arr = nullptr;
}

// Destructor
String::~String() {
	if (arr != nullptr)
		delete[] arr;
}

// Assignment operator
String &String::operator=(const String &rhs) {
	delete[] arr;
	len = rhs.len;
	arr = new char[len + 1];
	arr[0] = static_cast<char>(len);
	for (auto i = 1; i <= len; ++i) {
		arr[i] = rhs.arr[i];
	}
	return *this;
}

// Move assignment operator
String &String::operator=(String &&rhs) {
	delete[] arr;
	len = rhs.len;
	arr = rhs.arr;
	rhs.arr = nullptr;
	return *this;
}


// Mutator operator[]
char &String::operator[](int index) {
	// check whether the index is within bounds
	if (index > len || index < 0)
		throw std::out_of_range("Index out of range");
	index += 1;
	return arr[index];
}

// Accessor operator[]
char String::operator[](int index) const {
	// check whether the index is within bounds
	if (index > len || index < 0)
		throw std::out_of_range("Index out of range");
	index += 1;
	return arr[index];
}

// Get the length (number of characters) of a String object
//method from CS590 lecture 1 notes
int String::length() const {
	return len;
}
//method from CS590 lecture 1 notes, with modifications for pascal string
bool operator==(const String &lhs, const String &rhs) {
	if (lhs.len == 0)
		return rhs.len == 0;
	else {
		int shorter = lhs.length();
		if (rhs.length() < shorter)
			shorter = rhs.length();
		for (int i = 1; i < shorter; i++) {
			if (lhs.arr[i] == rhs.arr[i])
				continue;
			else
				return false;
		}
		if (lhs.length() == rhs.length()) {
			return true;
		}
		else
			return false;
	}
}
//method from CS590 lecuture notes, with modifications for pascal string
bool operator<(const String &lhs, const String &rhs) {
	if (lhs.len == 0)
		return rhs.len != 0;
	else {
		int shorter = lhs.length();
		if (rhs.length() < shorter) {
			shorter = rhs.length();
		}
		for (int i = 1; i < shorter; i++) {
			if (lhs.arr[i] == rhs.arr[i])
				continue;
			else if (lhs.arr[i] < rhs.arr[i])
				return true;
			else if (lhs.arr[i] > rhs.arr[i])
				return false;
		}
		if (lhs.length() == rhs.length())
			return false;
		else if (lhs.length() > rhs.length())
			return true;
		else
			return false;
	}
}

// Friend functions for > comparison
//method from CS590 lecuture notes
bool operator>(const String &lhs, const String &rhs) {
	return rhs < lhs;
}

// Friend functions for <= comparison
//method from CS590 lecuture notes
bool operator<=(const String &lhs, const String &rhs) {
	return !(rhs < lhs);
}

// Friend functions for >= comparison
//method from CS590 lecuture notes, with modifications for pascal string
bool operator>=(const String &lhs, const String &rhs) {
	return !(lhs  < rhs);
}

// Friend functions for != comparison
//method from CS590 lecuture notes, with modifications for pascal string
bool operator!=(const String &lhs, const String &rhs) {
	return !(lhs == rhs);
}

//	Friend function for string concatination
String operator+(const String &lhs, const String &rhs) {
	int strLength = lhs.len + rhs.len;
	char strLenChar = static_cast<char>(strLength);
	char *tmpStr = new char[strLength + 1];
	tmpStr[0] = strLenChar;
	for (auto i = 1; i <= lhs.len; ++i) {
		tmpStr[i] = lhs.arr[i];
	}
	for (auto i = 1, p = 1; i <= rhs.len; ++i, ++p)
		tmpStr[lhs.len + i] = rhs.arr[p];
	String retStr(tmpStr, true);
	delete[] tmpStr;
	return retStr;
}

// Return C style character string
const char* String::c_str() const {
	string lengthStr = std::to_string(len);
	int lengthInt = strlen(lengthStr.c_str());
	int cStrLen = len;
	char *tmp = new char[cStrLen];

	for (int i = 0, p = 0; p < cStrLen; i++, p++) {
		tmp[i] = arr[p];
	}
	if (len == 0) {
		tmp[0] = '0';
	}
	tmp[cStrLen] = '\0';
	return tmp;
}

//	Friend function for output
//From code provided on CS590 canvas for assignment 1
std::ostream& operator<<(std::ostream &out, const String &obj) {
	return out << obj.c_str();
}

int main() {
	String b("!dddddddddddddddddddddddddddddddd");
	cout << b << endl;
}
