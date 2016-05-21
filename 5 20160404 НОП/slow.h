#ifndef SLOW_H
#define SLOW_H

#include "base.h"

// dp[i][j] = lcs(a[0..i], b[0..j])
table getLcsTable(const std::string &a, const std::string &b)
{
	table dp(a.length() + 1, line(b.length() + 1));
	dp[0][0] = 0;
	for (size_t i = 1; i <= a.length(); ++i)
		dp[i][0] = 0;
	for (size_t j = 1; j <= b.length(); ++j)
		dp[0][j] = 0;
	for (size_t i = 1; i <= a.length(); ++i)
		for (size_t j = 1; j <= b.length(); ++j)
			dp[i][j] = a[i - 1] == b[j - 1] ? (dp[i - 1][j - 1] + 1) : std::max(dp[i - 1][j], dp[i][j - 1]);
	return dp;
}

// простой lcs
size_t lcs(const std::string &a, const std::string &b)
{
	return getLcsTable(a, b)[a.length()][b.length()];
}

// lcs префикса a и суффикса b, O(n^4)
// result[i][j] = НОП(a[i, a.length()), b[0, j))
table lcsVerySlow(const std::string &a, const std::string &b)
{
	size_t na = a.length();
	size_t nb = b.length();
	table result(na, line(nb));
	for (size_t i = 0; i < na; ++i)
		for (size_t j = 0; j < nb; ++j)
		{
			std::string aSub = a.substr(i, na - i);
			std::string bSub = b.substr(0, j + 1);
			result[i][j] = lcs(aSub, bSub);
		}
	return result;
}

// lcs префикса a и суффикса b, O(n^3)
// result[i][j] = НОП(a[i, a.length()), b[0, j))
table lcsSlow(const std::string &a, const std::string &b)
{
	size_t na = a.length();
	size_t nb = b.length();
	table result(na, line(nb));
	// перебираем префикс первой строки
	// и ищем lcs этого префикса с каждой из подстрок второй строки
	for (size_t i = 0; i < na; ++i)
	{
		std::string aSub = a.substr(i, na - i);
		size_t nASub = na - i;
		table dp = getLcsTable(b, aSub);
		for (size_t j = 0; j < nb; ++j)
			result[i][j] = dp[j + 1][nASub];
	}
	return result;
}

#endif
