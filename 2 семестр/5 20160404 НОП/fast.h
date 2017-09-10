#ifndef FAST_H
#define FAST_H

#include "base.h"
#include "slowCheck.h"

// lcs префикса a и суффикса b, O(n^2)
// result[i][j] = НОП(a[i, a.length()), b[0, j))
table calculateSuffixPrefixLCS(const std::string &b, const std::string &a)
{
	size_t na = a.length();
	size_t nb = b.length();
	table h(na + 1, line(nb, 0));
	table v(na, line(nb + 1, 0));
	for (size_t i = 0; i < na; ++i)
		v[i][0] = 0;
	for (size_t j = 0; j < nb; ++j)
		h[0][j] = j + 1;
	for (size_t i = 0; i < na; ++i)
		for (size_t j = 0; j < nb; ++j)
		{
			bool isDiagonal = a[i] == b[j];
			int x = v[i][j];
			int y = h[i][j];
			if (x < y)
			{
				h[i + 1][j] = isDiagonal ? x : y;
				v[i][j + 1] = isDiagonal ? y : x;
			}
			else
			{
				h[i + 1][j] = x;
				v[i][j + 1] = y;
			}
		}
	
	//assert(v == getRightV(a, b));
	//assert(h == getRightH(a, b));
	
	table ans(na + 1, line(nb + 1, 0));
	for (size_t i = 0; i < na; ++i)
	{
		size_t critical = v[i][nb];
		for (size_t j = 0; j < nb; ++j)
			ans[i + 1][j] = ans[i][j] + (j < critical);
	}
	
	table transpose(nb, line(na));
	for (size_t i = 0; i < nb; ++i)
		for (size_t j = 0; j < na; ++j)
			transpose[i][j] = ans[j + 1][i];
	return transpose;
}

#endif
