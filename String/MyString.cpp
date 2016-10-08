#include "MyString.h"
#include <String.h>
#include <string>


String::String()
{
	rep = new char[1];
	rep[0] = '\0';
	len = 0;
}

String::String(const char *s, bool pascal) {
	if (pascal) {
		len = 0;
		const char *tmp = s;
		char num = s[0];
		len = static_cast<int>(num) - 48;
		rep = new char[len + 1];
		for (int i = 0, p = 1; i < len; i++, p++) {
			rep[i] = s[p];
		}
		rep[len] = '\0';
	}
	else {
		len = 0;
		const char *tmp = s;
		while (*tmp) {
			++len;
			++tmp;
		}
		rep = new char[len + 1];
		for (int i = 0; i <= len; ++i)
			rep[i] = s[i];
	}
}


String::String(const char *s) {
	len = 0;
	const char *tmp = s;
	while (*tmp) {
		++len;
		++tmp;
	}
	rep = new char[len + 1];
	for (int i = 0; i <= len; ++i)
		rep[i] = s[i];
}

String::String(const String &obj) {
	len = 0;
	char *tmp = obj.rep;
	while (*tmp) {
		++len;
		++tmp;
	}
	rep = new char[len + 1];
	for (int i = 0; i <= len; i++)
		rep[i] = obj.rep[i];
}

String::~String()
{
	delete[] rep;
	len = 0;
}

const String& String::operator=(const String &rhs) {
	if (this != &rhs) {
		len = 0;
		char *tmp = rhs.rep;
		while (*tmp) {
			++len;
			++tmp;
		}
		delete[] rep;
		rep = new char[len + 1];
		for (int i = 0; i <= len; ++i)
			rep[i] = rhs.rep[i];
	}
	return *this;
}

char& String::operator[](int index) {
	assert(index < len && index >= 0);
	return rep[index];
}

char String::operator[](int index) const {
	assert(index < len && index >= 0);
	return rep[index];
}

int String::Length() const {
	return len;
}
/*
bool operator==(const String &lhs, const String &rhs) {
	if (lhs.Length == 0) {
		if (rhs.Length == 0)
			return true;
		else
			return false;
	}
	else {
		int shorter = lhs.Length();
		if (rhs.Length < shorter)
			shorter = rhs.Length;
		for (int i = 0; i < shorter; i++) {
			if (lhs.rep[i] == rhs.rep[i])
				continue;
			else
				return false;
		}
		if (lhs.Length() == rhs.Length())
			return true;
		else
			return false;
	}
}
*/
bool operator==(const String &lhs, const String &rhs) {
	return (strcmp(lhs.rep, rhs.rep) == 0);
}

bool operator<(const String &lhs, const String &rhs) {
	return (strcmp(lhs.rep, rhs.rep) < 0);
}

bool operator>(const String &lhs, const String &rhs) {
	return (rhs < lhs);
}

bool operator<=(const String &lhs, const String &rhs) {
	return !(rhs < lhs);
}

bool operator>=(const String &lhs, const String &rhs) {
	return !(lhs < rhs);
}

bool operator!=(const String &lhs, const String &rhs) {
	return !(lhs == rhs);
}

String operator+(const String &lhs, const String &rhs) {
	int strLength = lhs.len + rhs.len + 1;
	char *tmpStr = new char[strLength];
	for (int i = 0; i <= rhs.len; ++i)
		tmpStr[lhs.len + i] = rhs.rep[i];
	String retStr(tmpStr);
	delete[] tmpStr;
	return retStr;
}

const char* String::C_StyleString() const {
	return rep;
}

ostream& operator<<(ostream &out, const String &obj) {
	return out << obj.C_StyleString();
}



/*
bool operator<(const String &lhs, const String &rhs) {
	if (lhs.Length() == 0) {
		if (rhs.Length() == 0)
			return false;
		else return true;
	}
	else {
		int shorter = lhs.Length();
		if (rhs.Length() < shorter)
			shorter = rhs.Length();
		for (int i = 0; i < shorter; i++) {
			if (lhs.rep[i] == rhs.rep[i])
				return true;
			else
				return false;
		}
		if (lhs.Length() == rhs.Length())
			return false;
		else if (lhs.Length() < rhs.Length())
			return true;
		else
			return false;
}
*/

