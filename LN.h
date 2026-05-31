#ifndef LARGE_NUMBER_LN_H
#define LARGE_NUMBER_LN_H
#include <string_view>

#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

typedef string_view sv;

class LN
{
  public:
	bool m_nan = false;
	int m_length;
	bool m_pos;
	char* m_val;

	// Constructors
	LN();
	explicit LN(char* m_val, int m_length, bool m_pos);
	explicit LN(const char* val);
	explicit LN(long long val);
	explicit LN(sv val);
	LN(const LN& copyTarget);
	LN(LN&& moveTarget) noexcept;
	~LN();

	// Binary Operators
	LN& operator=(const LN& element);
	LN operator+(const LN& element) const;
	LN operator-(const LN& element) const;
	LN operator*(const LN& element) const;
	LN operator/(const LN& element) const;
	LN operator%(const LN& element) const;

	// Unary Operators
	LN operator~() const;
	LN operator-() const;

	// Comparators
	bool operator==(const LN& element) const;
	bool operator>(const LN& element) const;
	bool operator<(const LN& element) const;
	bool operator!=(const LN& element) const;
	bool operator>=(const LN& element) const;
	bool operator<=(const LN& element) const;

	// Shorthand Assignments
	LN& operator+=(const LN& element);
	LN& operator-=(const LN& element);
	LN& operator*=(const LN& element);
	LN& operator/=(const LN& element);
	LN& operator%=(const LN& element);

	// Typecasts
	explicit operator long long() const;
	explicit operator bool() const;
};

// Literal
LN operator""_ln(const char* digits);

#endif
