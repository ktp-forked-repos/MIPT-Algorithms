#include "base.h"

// Медленная реализация получения таблицы критических моментов для вертикальных рёбер
table getRightV(const std::string &a, const std::string &b)
{
	table v(a.length(), line(b.length() + 1, 0));
	for (size_t i = 0; i < a.length(); ++i)
		for (size_t j = 0; j <= b.length(); ++j)
		{
			std::string partA1 = a.substr(0, i);
			std::string partA2 = a.substr(0, i + 1);
			for (size_t k = 0; k <= j; ++k)
			{
				std::string partB = b.substr(k, j - k);
				size_t lcs1 = lcs(partB, partA1);
				size_t lcs2 = lcs(partB, partA2);
				if (lcs1 == lcs2)
				{
					v[i][j] = k;
					break;
				}
			}
		}
	return v;
}

// ... горизонтальных
table getRightH(const std::string &a, const std::string &b)
{
	table h(a.length() + 1, line(b.length(), 0));
	for (size_t i = 0; i <= a.length(); ++i)
		for (size_t j = 0; j < b.length(); ++j)
		{
			std::string partA = a.substr(0, i);
			h[i][j] = j + 1;
			for (size_t k = 0; k <= j; ++k)
			{
				std::string partB1 = b.substr(k, j - k);
				std::string partB2 = b.substr(k, j - k + 1);
				size_t lcs1 = lcs(partA, partB1);
				size_t lcs2 = lcs(partA, partB2);
				if (lcs1 < lcs2)
				{
					h[i][j] = k;
					break;
				}
			}
		}
	return h;
}
