#pragma once
#ifndef __MYSTRING_H__
#define __MYSTRING_H__

#include<cassert>
#include<iostream>

using namespace std;
class String
{
public:
	String();
	String(const char *s);
	String(const String &s);
	String(const char *str, bool pascal);
	~String();

	const String& operator=(const String &rhsObject);

	char& operator[](int index);

	char operator[](int index) const;

	int Length() const;

	friend bool operator==(const String &lhsObject, const String &rhsObject);
	friend bool operator<(const String &lhsObject, const String &rhsObject);
	friend bool operator>(const String &lhsObject, const String &rhsObject);
	friend bool operator<=(const String &lhsObject, const String &rhsObject);
	friend bool operator>=(const String &lhsObject, const String &rhsObject);
	friend bool operator!=(const String &lhsObject, const String &rhsObject);
	friend String operator+(const String &lhs, const String &rhs);

	const char* C_StyleString() const;
	
	friend ostream& operator<<(ostream &out, const String &obj);

	public:
		char *rep;
		int len;
};
#endif