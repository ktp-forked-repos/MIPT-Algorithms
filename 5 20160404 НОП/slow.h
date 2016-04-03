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
// result(i, j) = НОП(a[0, i), b[j, b.length()))
table lcsVerySlow(const std::string &a, const std::string &b)
{
	size_t na = a.length();
	size_t nb = b.length();
	table result(na + 1, line(nb + 1));
	for (size_t i = 0; i <= na; ++i)
		for (size_t j = 0; j <= nb; ++j)
		{
			std::string aSub = a.substr(0, i);
			std::string bSub = b.substr(j, nb - j);
			result[i][j] = lcs(aSub, bSub);
		}
	return result;
}

// lcs префикса a и суффикса b, O(n^3)
// result(i, j) = НОП(a[0, i), b[j, b.length()))
table lcsSlow(const std::string &a, const std::string &b)
{
	size_t na = a.length();
	size_t nb = b.length();
	table result(na + 1, line(nb + 1));
	// перебираем префикс второй строки
	// и ищем lcs этого префикса с каждой из подстрок первой строки
	for (size_t j = 0; j <= nb; ++j)
	{
		std::string bSub = b.substr(j, nb - j);
		size_t nBSub = nb - j;
		table dp = getLcsTable(a, bSub);
		for (size_t i = 0; i <= na; ++i)
			result[i][j] = dp[i][nBSub];
	}
	return result;
}
