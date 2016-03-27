#include <bits/stdc++.h>
using namespace std;
#define dbg(x) cerr << #x << " = " << (x) << endl;
typedef unsigned long long ull;
typedef vector<int> VI;
typedef vector<VI> VVI;

bool allisok;
int colors[100];
void dfs(const VVI &g, int v, int color)
{
	if (colors[v] == color || !allisok)
		return;
	if (colors[v] == 3 - color)
	{
		allisok = 0;
		return;
	}
	colors[v] = color;
	for (int nv : g[v])
		dfs(g, nv, 3 - color);
}

bool check(const VVI &g)
{
	int n = g.size();
	allisok = 1;
	for (int i = 0; i < n; ++i)
		colors[i] = 0;
	for (int i = 0; i < n; ++i)
		if (colors[i] == 0)
			dfs(g, i, 1);
	return allisok;
}

bool check(int n, ull mask)
{
	VVI g(n);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < i; ++j)
		{
			if (mask % 2)
			{
				g[i].push_back(j);
				g[j].push_back(i);
			}
			mask /= 2;
		}
	if (check(g) && 0)
	{
		static int i = 0;
		printf("\t%d\n", ++i);
		for (int i = 0; i < n; ++i)
			for (int v : g[i])
				if (i < v)
					printf("%d %d\n", i + 1, v + 1);
	}
	return check(g);
}

ull slow(int n)
{
	ull ans = 0;
	int m = n * (n - 1) / 2;
	for (ull mask = 0; mask < (1LL << m); ++mask)
		ans += check(n, mask);
	return ans;
}

const int CN = 100;
ull cs[CN][CN];
void initC()
{
	for (int i = 0; i < CN; ++i)
		cs[i][0] = cs[i][i] = 1;
	for (int i = 1; i < CN; ++i)
		for (int j = 1; j < i; ++j)
			cs[i][j] = cs[i - 1][j] + cs[i - 1][j - 1];
}

ull C(int n, int k)
{
	return cs[n][k];
}

ull all(int n)
{
	return 1LL << (n * (n - 1) / 2);
}

bool used[100];
void dfs(int v, VVI &g)
{
	if (used[v])
		return;
	used[v] = 1;
	for (int nv : g[v])
		dfs(nv, g);
}

bool checkConnected(VVI &g)
{
	int n = g.size();
	if (n == 0)
		return 1;
	for (int i = 0; i < n; ++i)
		used[i] = 0;
	dfs(0, g);
	for (int i = 0; i < n; ++i)
		if (!used[i])
			return 0;
	return 1;
}

bool checkConnected(int n, int m, ull mask)
{
	VVI g(n + m);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
		{
			if (mask % 2)
			{
				g[i].push_back(n + j);
				g[n + j].push_back(i);
			}
			mask /= 2;
		}
	if (check(g) && checkConnected(g) && 1)
	{
		static int i = 0;
		printf("\t%d\n", ++i);
		for (int i = 0; i < n; ++i)
			for (int v : g[i])
				if (i < v)
					printf("%d %d\n", i + 1, v + 1);
	}
	return check(g) && checkConnected(g);
}

ull connectedSlow(int n, int m)
{
	ull ans = 0;
	for (ull mask = 0; mask < (1LL << (n * m)); ++mask)
		ans += checkConnected(n, m, mask);
	return ans;
}

ull connected(int n, int m)
{
	//*
	if (n > m)
		swap(n, m);
	if (n == 0)
		return m <= 1;
	//*/
	ull ans = 1LL << (n * m);
	--ans; // без рёбер
	for (int k1 = 0; k1 < n; ++k1)
		for (int k2 = 0; k2 < m; ++k2)
			if (k1 > 0 || k2 > 0)
				ans -= connected(n - k1, m - k2) * C(n, k1) * C(m, k2);
	assert(ans >= 0);
	return ans;
}

ull connected(int n)
{
	if (n == 0)
		return 1;
	ull ans = 0;
	for (int k = 1; k <= n; ++k)
		ans += connected(k, n - k) * C(n - 1, k - 1);
	return ans;
}

ull fast(int n)
{
	if (n == 0)
		return 1;
	ull ans = 0;
	for (int n0 = 1; n0 <= n; ++n0)
		ans += C(n - 1, n0 - 1) * connected(n0) * fast(n - n0);
	return ans;
}

void test()
{
	//*
	dbg(connected(0, 0))
	dbg(connected(0, 1))
	dbg(connected(0, 2))
	dbg(connected(1, 1))
	dbg(connected(1, 2))
	dbg(connected(1, 3))
	dbg(connected(1, 4))
	dbg(connected(1, 5))
	dbg(connected(2, 2))
	exit(0);
	//*/
	
	for (int i = 0; i <= 3; ++i)
	{
		for (int j = 0; j <= 3; ++j)
			printf("%3d ", connected(i, j));
		printf("\n");
	}
	exit(0);
}

const int N = 6;
int main()
{
	initC();
	dbg(connected(2, 2))
	dbg(connectedSlow(2, 2))
	return 0;
	dbg(connected(2, 3))
	dbg(connectedSlow(2, 3))
	//test();
	for (int i = 0; i <= N; ++i)
	{
		ull ans1 = slow(i);
		ull ans2 = fast(i);
		printf("%d %d %10lld %10lld    %.2f\n", ans1 == ans2, i, ans1, ans2, ans1 / (float) all(i));
	}
	return 0;
}
