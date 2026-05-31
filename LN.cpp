#include "LN.h"

#include "utility.h"

#include <limits>

using namespace std;

long long LL_MIN = numeric_limits< long long >::min();
long long LL_MAX = numeric_limits< long long >::max();
long long LL_NAN = numeric_limits< long long >::quiet_NaN();

// Constructors
LN::LN() : LN("0") {}

LN::LN(char* m_val, int m_length, bool m_pos) : m_val(m_val), m_length(m_length), m_pos(m_pos) {}

LN::LN(const char* val) : m_length(0), m_pos(val[0] != '-')
{
	while (val[m_length])
		m_length++;
	m_val = (char*)malloc(sizeof(char) * (m_length + m_pos));
	if (!m_val)
		throw bad_alloc();
	for (int i = !m_pos; i < m_length; i++)
		m_val[m_length - 1 - i] = val[i];
	m_val[m_length + m_pos - 1] = 0;
	m_length -= !m_pos;
}

LN::LN(const long long val) : m_length(val == 0 ? 1 : 0), m_pos(val >= 0)
{
	for (long long temp = val; temp; temp /= 10)
		m_length++;
	m_val = (char*)malloc(sizeof(char) * (m_length + 1));
	if (!m_val)
		throw bad_alloc();
	long long temp = val < 0 ? -val : val;
	for (int i = 0; i < m_length; i++, temp /= 10)
		m_val[i] = (char)(48 + (temp % 10));
	m_val[m_length] = 0;
}

LN::LN(const sv val) : m_length((int)val.size()), m_pos(val[0] != '-')
{
	m_val = (char*)malloc(sizeof(char) * (m_length + m_pos));
	if (!m_val)
		throw bad_alloc();
	for (int i = !m_pos; i < m_length; i++)
		m_val[m_length - 1 - i] = val[i];
	m_val[m_length + m_pos - 1] = 0;
	m_length -= !m_pos;
}

LN::LN(const LN& copyTarget) : m_pos(copyTarget.m_pos), m_length(copyTarget.m_length), m_nan(copyTarget.m_nan)
{
	m_val = (char*)malloc(sizeof(char) * (m_length + 1));
	if (!m_val)
		throw bad_alloc();
	for (int i = 0; i < m_length; i++)
		m_val[i] = copyTarget.m_val[i];
	m_val[m_length] = 0;
}

LN::LN(LN&& moveTarget) noexcept :
	m_pos(moveTarget.m_pos), m_length(moveTarget.m_length), m_val(moveTarget.m_val), m_nan(moveTarget.m_nan)
{
	moveTarget.m_pos = false;
	moveTarget.m_length = 0;
	moveTarget.m_val = nullptr;
	moveTarget.m_nan = false;
}

LN::~LN()
{
	free(m_val);
	m_val = nullptr;
}

// Binary Operators
LN& LN::operator=(const LN& element)
{
	if (m_nan || element.m_nan)
	{
		m_nan = true;
		return *this;
	}
	if (this == &element)
		return *this;
	m_pos = element.m_pos;
	m_length = element.m_length;
	free(m_val);
	m_val = (char*)malloc(sizeof(char) * (m_length + 1));
	if (!m_val)
		throw bad_alloc();
	for (int i = 0; i < m_length; i++)
		m_val[i] = element.m_val[i];
	m_val[m_length] = 0;
	return *this;
}
LN LN::operator+(const LN& element) const
{
	if (m_nan || element.m_nan)
	{
		LN res;
		res.m_nan = true;
		return res;
	}
	bool gr = compare(m_val, m_length, element.m_val, element.m_length) == 1;
	bool pos = (m_pos != element.m_pos ? gr ^ element.m_pos : m_pos);
	char *val, *max_val = (gr ? m_val : element.m_val), *min_val = (gr ? element.m_val : m_val);
	int length, max_length = max(m_length, element.m_length);
	if (m_pos != element.m_pos)
		val = subtract(max_val, min_val, max_length, &length);
	else
		val = add(max_val, min_val, max_length, &length);
	return LN(val, length, pos);
}
LN LN::operator-(const LN& element) const
{
	if (m_nan || element.m_nan)
	{
		LN res;
		res.m_nan = true;
		return res;
	}
	bool gr = compare(m_val, m_length, element.m_val, element.m_length) == 1;
	bool pos = (m_pos == element.m_pos ? gr ^ (!m_pos) : m_pos);
	char *val, *max_val = (gr ? m_val : element.m_val), *min_val = (gr ? element.m_val : m_val);
	int length, max_length = max(m_length, element.m_length);
	if (m_pos == element.m_pos)
		val = subtract(max_val, min_val, max_length, &length);
	else
		val = add(max_val, min_val, max_length, &length);
	return LN(val, length, pos);
}
LN LN::operator*(const LN& element) const
{
	if (m_nan || element.m_nan)
	{
		LN res;
		res.m_nan = true;
		return res;
	}
	bool pos = (m_pos == element.m_pos), gr = compare(m_val, m_length, element.m_val, element.m_length) == 1;
	char *val, *max_val = (gr ? m_val : element.m_val), *min_val = (gr ? element.m_val : m_val);
	int length, max_length = max(m_length, element.m_length);
	val = multiply(max_val, min_val, max_length, &length);
	return LN(val, length, pos);
}
LN LN::operator/(const LN& element) const
{
	LN res;
	if (m_nan || element.m_nan || is_zero(&element))
	{
		res.m_nan = true;
		return res;
	}
	if (compare(m_val, m_length, element.m_val, element.m_length) == -1)
		return 0_ln;
	res.m_val = divide(m_val, m_length, element.m_val, element.m_length, &res.m_length);
	res.m_pos = (m_pos == element.m_pos);
	return res;
}
LN LN::operator%(const LN& element) const
{
	if (m_nan || element.m_nan || is_zero(&element))
	{
		LN res;
		res.m_nan = true;
		return res;
	}
	return *this - element * (*this / element);
}

// Unary Operators
LN LN::operator~() const
{
	if (!m_pos)
	{
		if (is_zero(this))
			return 0_ln;
		LN res;
		res.m_nan = true;
		return res;
	}
	LN sqrt = *this;
	while (sqrt * sqrt > *this)
		sqrt = (sqrt + (*this / sqrt)) / 2_ln;
	return sqrt;
}
LN LN::operator-() const
{
	LN res = *this;
	res.m_pos = !res.m_pos;
	return res;
}

// Comparators
bool LN::operator==(const LN& element) const
{
	if (m_nan || element.m_nan)
		return false;
	if (is_zero(this) && is_zero(&element))
		return true;
	if (m_pos != element.m_pos)
		return false;
	if (m_length != element.m_length)
		return false;
	return !compare(m_val, m_length, element.m_val, m_length);
}
bool LN::operator>(const LN& element) const
{
	if (m_nan || element.m_nan)
		return false;
	if (is_zero(this) && is_zero(&element))
		return false;
	if (m_pos != element.m_pos)
		return m_pos;
	if (m_length != element.m_length)
		return (m_length > element.m_length) == m_pos;
	if (m_pos)
		return compare(m_val, m_length, element.m_val, element.m_length) == 1;
	else
		return compare(element.m_val, element.m_length, m_val, m_length) == 1;
}
bool LN::operator<(const LN& element) const
{
	if (m_nan || element.m_nan)
		return false;
	if (is_zero(this) && is_zero(&element))
		return false;
	if (m_pos != element.m_pos)
		return !m_pos;
	if (m_length != element.m_length)
		return (m_length < element.m_length) == m_pos;
	if (m_pos)
		return compare(element.m_val, element.m_length, m_val, m_length) == 1;
	else
		return compare(m_val, m_length, element.m_val, element.m_length) == 1;
}
bool LN::operator!=(const LN& element) const
{
	if (m_nan || element.m_nan)
		return false;
	return !(*this == element);
}
bool LN::operator>=(const LN& element) const
{
	return *this == element || *this > element;
}
bool LN::operator<=(const LN& element) const
{
	return *this == element || *this < element;
}

// Shorthand Assignments
LN& LN::operator+=(const LN& element)
{
	if (!m_nan)
		*this = *this + element;
	return *this;
}
LN& LN::operator-=(const LN& element)
{
	if (!m_nan)
		*this = *this - element;
	return *this;
}
LN& LN::operator*=(const LN& element)
{
	if (!m_nan)
		*this = *this * element;
	return *this;
}
LN& LN::operator/=(const LN& element)
{
	if (!m_nan)
		*this = *this / element;
	return *this;
}
LN& LN::operator%=(const LN& element)
{
	if (!m_nan)
		*this = *this % element;
	return *this;
}

// Typecasts
LN::operator long long() const
{
	if (m_nan)
		return LL_NAN;
	long long res = 0;
	long long temp = (m_pos ? 1 : -1);
	for (int i = 0; i < m_length; i++)
	{
		if (m_pos && (LL_MAX - res < (m_val[i] - '0') * temp))
			throw overflow_error("Casting overflow");
		if (!m_pos && (LL_MIN - res > (m_val[i] - '0') * temp))
			throw overflow_error("Casting overflow");
		res += (m_val[i] - '0') * temp;
		temp *= 10;
	}
	return res;
}
LN::operator bool() const
{
	if (m_nan)
		return false;
	return !is_zero(this);
}

// Literal
LN operator""_ln(const char* digits)
{
	int length = 0;
	while (digits[length])
		length++;
	char* m_val = (char*)malloc(sizeof(char) * (length + 1));
	if (!m_val)
		throw bad_alloc();
	for (int i = 0; i < length; i++)
		m_val[i] = digits[length - 1 - i];
	m_val[length] = 0;
	return LN(m_val, length, true);
}
