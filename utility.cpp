#pragma once
#include "LN.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

int max(int a, int b)
{
	return a > b ? a : b;
}
int min(int a, int b)
{
	return a < b ? a : b;
}

bool binary(const string *line)
{
	return *line == "+" || *line == "-" || *line == "*" || *line == "/" || *line == "%";
}
bool unary(const string *line)
{
	return *line == "_" || *line == "~";
}
bool comparator(const string *line)
{
	return *line == "==" || *line == "!=" || *line == "<" || *line == "<=" || *line == ">" || *line == ">=";
}

bool is_zero(const LN *a)
{
	return a->m_length == 1 && a->m_val[0] == '0';
}
char *clear(char *array, int length)
{
	for (int i = 0; i < length; i++)
		array[i] = '0';
	return array;
}
int compare(const char *a, int len_a, const char *b, int len_b)
{
	if (len_a != len_b)
		return len_a > len_b ? 1 : -1;
	int i = len_a - 1;
	while (i >= 0)
	{
		if (a[i] > b[i])
			return 1;
		if (a[i] < b[i])
			return -1;
		i--;
	}
	return 0;
}

char *add(const char *a, const char *b, int len_a, int *res_length)
{
	bool carry = false, past_b = false;
	char cur_a, cur_b;

	auto res = (char *)malloc(sizeof(char) * (len_a + 2));
	if (!res)
		throw bad_alloc();

	for (int i = 0; i < len_a; i++)
	{
		if (b[i] == 0)
			past_b = true;
		cur_a = (char)(a[i] + carry);
		cur_b = (past_b ? '0' : b[i]);
		carry = (cur_a + cur_b - 96) >= 10;
		res[i] = (char)((cur_a + cur_b - 96) % 10 + '0');
	}

	*res_length = len_a + carry;
	res[len_a] = (char)('0' + carry);
	res[*res_length] = 0;
	return res;
}
char *subtract(const char *a, const char *b, int len_a, int *res_length)
{
	int trailing = 0;
	bool carry = false, past_b = false;
	char cur_a, cur_b;

	auto res = (char *)malloc(sizeof(char) * (len_a + 1));
	if (!res)
		throw bad_alloc();

	for (int i = 0; i < len_a; i++)
	{
		if (b[i] == 0)
			past_b = true;
		cur_a = (char)(a[i] - carry);
		cur_b = (past_b ? '0' : b[i]);
		carry = (cur_a < cur_b);
		res[i] = (char)(cur_a - cur_b + '0' + 10 * carry);
	}

	for (int i = len_a - 1; i >= 0; i--)
	{
		if (res[i] == '0')
			trailing++;
		else
			break;
	}

	*res_length = max(len_a - trailing, 1);
	res[*res_length] = 0;
	return res;
}
char *multiply(const char *a, const char *b, int len_a, int *res_length)
{
	int cur, length, trailing = 0;
	auto res = (char *)malloc(sizeof(char) * (len_a * 2 + 1)), temp_res = (char *)malloc(sizeof(char) * (len_a * 2 + 1));
	if (!res || !temp_res)
	{
		free(res);
		free(temp_res);
		throw bad_alloc();
	}
	clear(res, len_a * 2 + 1);

	for (int i = 0; b[i] != 0; i++)
	{
		cur = 0;
		clear(temp_res, len_a * 2 + 1);
		for (int j = 0; j < len_a; j++)
		{
			cur += (b[i] - '0') * (a[j] - '0');
			temp_res[i + j] += cur % 10;
			cur /= 10;
		}
		temp_res[i + len_a] += cur;
		res = add(temp_res, res, len_a * 2, &length);
	}

	for (int i = len_a * 2 - 1; i >= 0; i--)
	{
		if (res[i] == '0')
			trailing++;
		else
			break;
	}

	free(temp_res);
	*res_length = max(len_a * 2 - trailing, 1);
	res[*res_length] = 0;
	return res;
}
char *divide(const char *a, int len_a, const char *b, int len_b, int *res_length)
{
	*res_length = len_a - len_b + 1;
	auto res = (char *)malloc(sizeof(char) * (*res_length + 1));
	if (!res)
		throw bad_alloc();
	res[*res_length] = 0;
	clear(res, *res_length);
	res[*res_length - 1] = '1';
	int comp;
	const char *maximum = (compare(res, *res_length, b, len_b) == 1) ? res : b;
	const char *minimum = (maximum == res ? b : res);

	int temp_mult_len = 0;
	char *temp_mult = multiply(maximum, minimum, max(*res_length, len_b), &temp_mult_len);
	if (compare(temp_mult, temp_mult_len, a, len_a) == 1)
	{
		*res_length = *res_length - 1;
		res[*res_length] = 0;
		res[*res_length - 1] = '1';
	}

	for (int i = *res_length; i >= 0; i--)
	{
		while (res[i] <= '9')
		{
			res[i] += 1;
			maximum = (compare(res, *res_length, b, len_b) == 1) ? res : b;
			minimum = (maximum == res ? b : res);
			temp_mult = multiply(maximum, minimum, max(*res_length, len_b), &temp_mult_len);
			comp = compare(temp_mult, temp_mult_len, a, len_a);
			if (comp == 1)
				break;
		}
		res[i] -= 1;
	}

	return res;
}
