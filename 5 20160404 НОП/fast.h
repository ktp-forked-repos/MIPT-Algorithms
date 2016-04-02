#include "base.h"

// lcs префикса a и суффикса b, O(n^2)
// result(i, j) = НОП(a[0, i), b[j, b.length()))
table lcsFast(const std::string &a, const std::string &b)
{
	table h(a.length() + 1, line(b.length(), 0));
	table v(a.length(), line(b.length() + 1, 0));
	for (size_t i = 0; i < a.length(); ++i)
		v[i][0] = 0;
	for (size_t j = 0; j < b.length(); ++j)
		h[0][j] = j + 1;
	for (size_t i = 0; i < a.length(); ++i)
		for (size_t j = 0; j < b.length(); ++j)
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
	
	table ans(a.length() + 1, line(b.length() + 1, 0));
	for (size_t i = 0; i < a.length(); ++i)
	{
		size_t critical = v[i][b.length()];
		for (size_t j = 0; j < b.length(); ++j)
			ans[i + 1][j] = ans[i][j] + (j < critical);
	}
	return ans;
}
