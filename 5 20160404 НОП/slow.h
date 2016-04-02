#include "base.h"

// простой lcs
size_t lcs(const std::string &a, const std::string &b)
{
	size_t **dp = new size_t*[a.length() + 1];
	for (size_t i = 0; i <= a.length(); ++i)
		dp[i] = new size_t[b.length() + 1];
	
	dp[0][0] = 0;
	for (size_t i = 1; i <= a.length(); ++i)
		dp[i][0] = 0;
	for (size_t j = 1; j <= b.length(); ++j)
		dp[0][j] = 0;
	
	for (size_t i = 1; i <= a.length(); ++i)
		for (size_t j = 1; j <= b.length(); ++j)
			dp[i][j] = a[i - 1] == b[j - 1] ? (dp[i - 1][j - 1] + 1) : std::max(dp[i - 1][j], dp[i][j - 1]);
	size_t result = dp[a.length()][b.length()];
	
	for (size_t i = 0; i <= a.length(); ++i)
		delete[] dp[i];
	delete[] dp;
	
	return result;
}

// lcs префикса a и суффикса b, O(n^4)
// result(i, j) = НОП(a[0, i), b[j, b.length()))
table lcsSlow(const std::string &a, const std::string &b)
{
	table result(a.length() + 1, line(b.length() + 1));
	for (size_t i = 0; i <= a.length(); ++i)
		for (size_t j = 0; j <= b.length(); ++j)
		{
			std::string aSub = a.substr(0, i);
			std::string bSub = b.substr(j, b.length() - j);
			result[i][j] = lcs(aSub, bSub);
		}
	return result;
}
