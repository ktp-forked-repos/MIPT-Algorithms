#include <bits/stdc++.h>
#define dbg(x) std::cerr << #x << " = " << (x) << std::endl;

typedef std::vector<size_t> line;
typedef std::vector<line> table;

size_t lcs(std::string s, std::string t)
{
	size_t **dp = new size_t*[s.length() + 1];
	for (size_t i = 0; i <= s.length(); ++i)
		dp[i] = new size_t[t.length() + 1];
	
	dp[0][0] = 0;
	for (size_t i = 1; i <= s.length(); ++i)
		dp[i][0] = 0;
	for (size_t j = 1; j <= t.length(); ++j)
		dp[0][j] = 0;
	
	for (size_t i = 1; i <= s.length(); ++i)
		for (size_t j = 1; j <= t.length(); ++j)
			dp[i][j] = s[i - 1] == t[j - 1] ? (dp[i - 1][j - 1] + 1) : std::max(dp[i - 1][j], dp[i][j - 1]);
	size_t result = dp[s.length()][t.length()];
	
	for (size_t i = 0; i <= s.length(); ++i)
		delete[] dp[i];
	delete[] dp;
	
	return result;
}

// result(i, j) = НОП(s[0, i), t[j, t.length()))
table slow(std::string s, std::string t)
{
	table result(s.length() + 1, line(t.length() + 1));
	for (size_t i = 0; i <= s.length(); ++i)
		for (size_t j = 0; j <= t.length(); ++j)
		{
			std::string sSub = s.substr(0, i);
			std::string tSub = t.substr(j, t.length() - j);
			result[i][j] = lcs(sSub, tSub);
		}
	return result;
}

table fast(std::string s, std::string t)
{
	table h(s.length() + 1, line(t.length() + 1));
	table v(s.length() + 1, line(t.length() + 1));
	//*
	for (size_t i = 0; i <= s.length(); ++i)
		v[i][0] = 0;
	for (size_t j = 0; j <= t.length(); ++j)
		h[0][j] = j + 1;
	//*/
	for (size_t i = 0; i < s.length(); ++i)
		for (size_t j = 0; j < t.length(); ++j)
		{
			bool is = s[i] == t[j];
			int x = h[i][j];
			int y = v[i][j];
			if (x < y)
			{
				h[i + 1][j] = is ? x : y;
				v[i][j + 1] = is ? y : x;
			}
			else
			{
				h[i + 1][j] = x;
				v[i][j + 1] = y;
			}
		}
	
	dbg(h[0][0])
	dbg(v[0][0])
	return h;
	
	for (size_t i = 0; i <= s.length(); ++i)
	{
		for (size_t j = 0; j <= t.length(); ++j)
			printf("%2d", v[i][j]);
		printf("\n");
	}
	return h;
	
	for (size_t i = 0; i <= s.length(); ++i)
		printf("%d ", v[i][t.length()]);
	printf("\n");
}

std::string generateString(size_t n)
{
	std::string s(n, '0');
	for (size_t i = 0; i < n; ++i)
		s[i] = '0' + rand() % 3;
	return s;
}

void generate(size_t n = 7, size_t m = 10)
{
	freopen("input.txt", "w", stdout);
	std::cout << generateString(n) << std::endl;
	std::cout << generateString(m) << std::endl;
	exit(0);
}

void printPoints(table t)
{
	for (size_t i = 0; i < t.size(); ++i)
	{
		for (size_t j = 0; j < t[i].size(); ++j)
			printf("%2d", t[i][j]);
		printf("\n");
		if (i + 1 == t.size())
			break;
		printf("%c ", t[i][0] == t[i + 1][0] ? '0' : ' ');
		for (size_t j = 1; j < t[i].size(); ++j)
		{
			bool c = t[i][j] == t[i + 1][j] && t[i][j] == t[i][j - 1] && t[i][j] + 1 == t[i + 1][j - 1];
			printf("%c ", c ? ('0' + j) : ' ');
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");
}

int main()
{
	//generate();
	freopen("input.txt", "r", stdin);
	std::string s, t;
	std::cin >> s >> t;
	table t1 = slow(s, t);
	//table t2 = fast(s, t);
	//assert(t1 == t2);
	
	//printPoints(t1);
	fast(s, t);
	
	return 0;
}
