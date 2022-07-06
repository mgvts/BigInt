#include "LN.h"

#include <climits>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#define ULL unsigned long long
using namespace std;

void zeroes(vector< int > &digits)
{
	while (digits.size() > 1 && digits.back() == 0)
	{
		digits.pop_back();
	}
}

LN::LN(long long num)
{
	string s = to_string(num);
	unsigned long long n;
	if (s[0] == '-')
	{
		LN::negate = true;
		n = s.size() - 1;
		s = s.substr(1);
	}
	else
	{
		LN::negate = false;
		n = s.size();
	}
	LN::digits = vector< int >(n);
	for (size_t i = n - 1; i > -1; i--)
	{
		LN::digits.at(n - i - 1) = s[i] - '0';
	}
	LN::NaN = false;
}

LN::LN(const char *str)
{
	if (str[0] == 'N' && str[1] == 'a' && str[2] == 'N')
	{
		LN::NaN = true;
		return;
	}
	ULL n;
	if (str[0] == '-')
	{
		LN::negate = true;
		n = strlen(str) - 1;
	}
	else
	{
		LN::negate = false;
		n = strlen(str);
	}
	LN::digits = vector< int >(n);
	for (ULL i = 0; i < n; i++)
	{
		if (LN::negate)
		{
			LN::digits.at(i) = str[n - i] - '0';
		}
		else
		{
			LN::digits.at(i) = str[n - i - 1] - '0';
		}
	}
	LN::NaN = false;
}

LN::LN(const LN &obj)
{
	LN::digits = obj.digits;
	LN::negate = obj.negate;
	LN::NaN = obj.NaN;
}

LN::LN(const vector< int > &arr)
{
	LN::negate = false;
	LN::digits = arr;

	LN::NaN = false;
}

LN::LN(LN &&x) noexcept : digits(), negate(false), NaN(false)
{
	digits = x.digits;
	NaN = x.NaN;
	negate = x.negate;
}

LN &LN::operator=(const LN &x) = default;

LN &LN::operator=(const LN &&x) noexcept
{
	digits = x.digits;
	NaN = x.NaN;
	negate = x.negate;
	return *this;
}

LN operator""_ln(const char *str)
{
	return LN(&(str[0]));
}

LN operator-(const LN &x)
{
	if (x.NaN)
	{
		string res = "NaN";
		return LN(&(res[0]));
	}
	string res;
	if (x.negate)
	{
		res = toString(x).substr(1);
	}
	else
	{
		res = "-" + toString(x);
	}
	return LN(&(res[0]));
}

bool operator<(const LN &x, const LN &y)
{
	if (x.NaN || y.NaN)
	{
		return false;
	}
	if (x.negate && y.negate)
	{
		return (-y) < (-x);
	}
	else if (x.negate && !y.negate)
	{
		return true;
	}
	else if (!x.negate && y.negate)
	{
		return false;
	}
	if (x.digits.size() < y.digits.size())
	{
		return true;
	}
	else if (x.digits.size() > y.digits.size())
	{
		return false;
	}
	ULL L = x.digits.size();

	for (ULL i = L - 1; i > 0; i--)
	{
		if (x.digits[i] < y.digits[i])
		{
			return true;
		}
		else if (x.digits[i] > y.digits[i])
		{
			return false;
		}
	}
	if (x.digits[0] < y.digits[0])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool operator>(const LN &x, const LN &y)
{
	if (x.NaN || y.NaN)
	{
		return false;
	}
	return (y <= x) && (x != y);
}

bool operator==(const LN &x, const LN &y)
{
	if (x.NaN || y.NaN)
	{
		return false;
	}
	return toString(x) == toString(y);
}

LN operator+(const LN &x, const LN &y)
{
	if (x.NaN || y.NaN)
	{
		string res = "NaN";
		return LN(&(res[0]));
	}
	if (x.negate && y.negate)
	{
		return -((-x) + (-y));
	}
	else if (x.negate && !y.negate)
	{
		return y - (-x);
	}
	else if (!x.negate && y.negate)
	{
		return x - (-y);
	}
	ULL L = max(x.digits.size(), y.digits.size()) + 1;
	vector< int > z(L);
	int over = 0;
	for (ULL i = 0; i < L; i++)
	{
		if (i < x.digits.size() && i < y.digits.size())
		{
			z[i] = x.digits[i] + y.digits[i] + over;
		}
		else if (i < x.digits.size() && i >= y.digits.size())
		{
			z[i] = x.digits[i] + over;
		}
		else if (i >= x.digits.size() && i < y.digits.size())
		{
			z[i] = y.digits[i] + over;
		}
		else if (i >= x.digits.size() && i >= y.digits.size())
		{
			z[i] = over;
		}
		over = 0;
		if (z[i] > 9)
		{
			z[i] = z[i] % 10;
			over = 1;
		}
	}

	zeroes(z);

	return LN(z);
}

LN operator-(const LN &x, const LN &y)
{
	if (x.NaN || y.NaN)
	{
		string res = "NaN";
		return LN(&(res[0]));
	}
	if (x.negate && y.negate)
	{
		return (-y) - (-x);
	}
	else if (x.negate && !y.negate)
	{
		return -(y + (-x));
	}
	else if (!x.negate && y.negate)
	{
		return x + (-y);
	}

	if (x < y)
	{
		return -(y - x);
	}
	ULL L = x.digits.size();

	int borrowed = 0;
	vector< int > res;
	for (ULL i = 0; i < L; i++)
	{
		int tmp = x.digits[i] - borrowed;
		if (i < y.digits.size())
		{
			tmp -= y.digits[i];
		}
		if (tmp < 0)
		{
			borrowed = abs(tmp - 10 + 1) / 10;
		}
		else
		{
			borrowed = 0;
		}
		res.push_back((tmp + 10) % 10);
	}
	zeroes(res);

	return LN(res);
}

// simple N^2 mul
LN operator*(const LN &x, const LN &y)
{
	if (x.NaN || y.NaN)
	{
		string res = "NaN";
		return LN(&(res[0]));
	}
	if (toString(x) == "0" || toString(y) == "0")
	{
		return 0_ln;
	}
	if (x.negate ^ y.negate)
	{
		return -((-x) * y);
	}
	ULL L = x.digits.size() + y.digits.size();
	vector< int > res(L);
	for (ULL i = 0; i < x.digits.size(); i++)
		for (ULL j = 0; j < y.digits.size(); j++)
			res[i + j] += x.digits[i] * y.digits[j];

	for (int i = 0; i < L; i++)
	{
		res[i + 1] += res[i] / 10;
		res[i] %= 10;
	}

	zeroes(res);
	return LN(res);
}

LN operator%(const LN &x, const LN &y)
{
	if (x.NaN || y.NaN)
	{
		string res = "NaN";
		return LN(&(res[0]));
	}
	return x - ((x / y) * y);
}

// divide with bin search
LN operator/(const LN &x, const LN &y)
{
	if (x.NaN || y.NaN || y == 0_ln)
	{
		string res = "NaN";
		return LN(&(res[0]));
	}
	if (x.negate && !y.negate)
	{
		return -((-x) / y);
	}
	else if (!x.negate && y.negate)
	{
		return -(x / (-y));
	}
	else if (x.negate && y.negate)
	{
		return (-x) / (-y);
	}
	if (y == 1_ln)
	{
		return x;
	}

	LN r = x;
	LN l = 0_ln;
	LN m;
	while (l < r - 1_ln)
	{
		m = right_shift((l + r) * 5_ln);

		if ((m * y) <= x)
		{
			l = m;
		}
		else
		{
			r = m;
		}
	}
	return l;
}

// sqrt with bin search
LN sqrt(const LN &x)
{
	if (x == 0_ln)
	{
		return 0_ln;
	}
	if (x < 0_ln || x.NaN)
	{
		string res = "NaN";
		return LN(&(res[0]));
	}

	if (x == 1_ln)
	{
		return 1_ln;
	}
	LN r = LN(x);
	LN l = 0_ln;
	LN m;
	while (l < r - 1_ln)
	{
		m = right_shift((l + r) * 5_ln);
		if (m * m <= x)
		{
			l = m;
		}
		else
		{
			r = m;
		}
	}
	return l;
}

LN right_shift(const LN &x)
{
	vector< int > res = {};
	res.insert(res.cbegin(), x.digits.begin() + 1, x.digits.end());
	if (res.empty())
	{
		return 0_ln;
	}
	return LN(res);
}

bool operator>=(const LN &x, const LN &y)
{
	if (x.NaN || y.NaN)
	{
		return false;
	}
	return (x > y) || (x == y);
}

bool operator<=(const LN &x, const LN &y)
{
	if (x.NaN || y.NaN)
	{
		return false;
	}
	return (x < y) || (x == y);
}

bool operator!=(const LN &x, const LN &y)
{
	if (x.NaN || y.NaN)
	{
		return false;
	}
	return !(x == y);
}

string toString(const LN &x)
{
	string s;
	if (x.NaN)
	{
		return "NaN";
	}
	for (ULL i = x.digits.size() - 1; i > 0; i--)
	{
		s += to_string(x.digits[i]);
	}
	s += to_string(x.digits[0]);
	if (s == "0")
	{
		return "0";
	}
	if (x.negate)
	{
		return "-" + s;
	}

	return s;
}

LN::operator long long() const
{
	if (*this > LN(LLONG_MAX) || *this < LN(LLONG_MIN))
	{
		throw invalid_argument("the number must fit into the long long type in order to be converted");
	}
	else if (this->NaN)
	{
		throw invalid_argument("sorry NaN couldn't convert into long long");
	}
	{
		return stoll(toString(*this));
	}
}

LN::operator bool() const
{
	return *this != 0_ln;
}

LN LN::operator+=(const LN &x)
{
	*this = *this + x;
	return *this;
}
LN LN::operator-=(const LN &x)
{
	*this = *this - x;
	return *this;
}
LN LN::operator*=(const LN &x)
{
	*this = *this * x;
	return *this;
}
LN LN::operator/=(const LN &x)
{
	*this = *this / x;
	return *this;
}
LN LN::operator%=(const LN &x)
{
	*this = *this % x;
	return *this;
}
