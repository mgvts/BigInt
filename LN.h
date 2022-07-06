#ifndef CPP_2_MGVTS_LN_H
#define CPP_2_MGVTS_LN_H

//#include <string_view>

#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class LN
{
  private:
	vector< int > digits;
	bool negate;
	bool NaN;

  public:
	explicit LN(long long num = 0ll);
	explicit LN(const char *);
	LN(const LN &);
	LN(LN &&other) noexcept;
	explicit LN(const vector< int > &);
	friend string toString(const LN &x);

	LN &operator=(const LN &x);
	LN &operator=(const LN &&x) noexcept;

	~LN() { digits.clear(); }

	explicit operator long long() const;
	explicit operator bool() const;

	friend LN operator+(const LN &, const LN &);
	friend LN operator-(const LN &, const LN &);
	friend LN operator*(const LN &, const LN &);
	friend LN operator/(const LN &, const LN &);
	friend LN operator%(const LN &, const LN &);

	friend bool operator<(const LN &, const LN &);
	friend bool operator<=(const LN &, const LN &);
	friend bool operator>=(const LN &, const LN &);
	friend bool operator>(const LN &, const LN &);
	friend bool operator==(const LN &, const LN &);
	friend bool operator!=(const LN &, const LN &);

	friend LN operator-(const LN &);
	LN operator+=(const LN &);
	LN operator-=(const LN &);
	LN operator*=(const LN &);
	LN operator/=(const LN &);
	LN operator%=(const LN &);

	friend LN sqrt(const LN &);
	friend LN right_shift(const LN &);
};
LN operator"" _ln(const char *str);

#endif