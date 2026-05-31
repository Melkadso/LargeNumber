#include "LN.h"
#include "return_codes.h"
#include "utility.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		perror("Wrong number of arguments, expected 3");
		return ERROR_INVALID_PARAMETER;
	}

	ifstream in(argv[1]);
	if (!in.is_open())
	{
		cerr << "Could not open input file";
		return ERROR_FILE_NOT_FOUND;
	}
	FILE *out = fopen(argv[2], "w");
	if (!out)
	{
		in.close();
		cerr << "Could not open output file";
		return ERROR_FILE_NOT_FOUND;
	}

	string line;
	vector<LN> nums;

	try
	{
		while (getline(in, line))
		{
			istringstream str(line);
			if (unary(&line))
			{
				if (nums.empty())
				{
					in.close();
					fclose(out);
					cerr << "Invalid data";
					return ERROR_INVALID_DATA;
				}
				LN res;
				LN x = nums.back();

				if (line == "_")
					res = -x;
				else if (line == "~")
					res = ~x;

				nums.pop_back();
				nums.push_back(res);
			}
			else if (binary(&line))
			{
				if (nums.size() < 2)
				{
					in.close();
					fclose(out);
					cerr << "Invalid data";
					return ERROR_INVALID_DATA;
				}
				LN res;
				LN x = nums[nums.size() - 2];
				LN y = nums[nums.size() - 1];

				if (line == "+")
					res = x + y;
				else if (line == "-")
					res = x - y;
				else if (line == "*")
					res = x * y;
				else if (line == "/")
					res = x / y;
				else if (line == "%")
					res = x % y;

				nums.pop_back();
				nums.pop_back();
				nums.push_back(res);
			}
			else if (comparator(&line))
			{
				if (nums.size() < 2)
				{
					in.close();
					fclose(out);
					cerr << "Invalid data";
					return ERROR_INVALID_DATA;
				}
				bool comp = false;
				LN res;
				LN x = nums[nums.size() - 2];
				LN y = nums[nums.size() - 1];

				if (line == "==")
					comp = x == y;
				else if (line == "!=")
					comp = x != y;
				else if (line == "<")
					comp = x < y;
				else if (line == "<=")
					comp = x <= y;
				else if (line == ">")
					comp = x > y;
				else if (line == ">=")
					comp = x >= y;

				if (comp)
					res = LN("1");
				else
					res = LN("0");

				nums.pop_back();
				nums.pop_back();
				nums.push_back(res);
			}
			else
				nums.push_back(LN(line.c_str()));
		}
	} 
	catch (bad_alloc &e)
	{
		cerr << "Allocation error" << endl;
		in.close();
		fclose(out);
		return ERROR_MEMORY;
	}

	for (int i = (int)nums.size() - 1; i >= 0; i--)
	{
		if (nums[i].m_nan)
		{
			fprintf(out, "NaN\n");
			continue;
		}
		if (!nums[i].m_pos && !is_zero(&nums[i]))
			fprintf(out, "%c", '-');
		for (int j = nums[i].m_length - 1; j >= 0; j--)
			fprintf(out, "%c", nums[i].m_val[j]);
		fprintf(out, "\n");
	}
	fclose(out);
	in.close();
}
