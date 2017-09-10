#include <bits/stdc++.h>
using namespace std;
#define dbg(x) cerr << #x << " = " << (x) << endl;
//typedef unsigned long long ull;
typedef long long ull;
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

ull all(int n)
{
	return 1LL << (n * (n - 1) / 2);
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
	if (check(g) && checkConnected(g) && 0)
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

bool checkBipartate(VVI &g, int k)
{
	int n = g.size();
	if (n == 0)
		return 1;
	for (int i = 0; i < n; ++i)
		used[i] = 0;
	for (int i = 0; i < n; ++i)
		if (!used[i])
		{
			k--;
			dfs(i, g);
		}
	return k == 0;
}

bool checkBipartate(int n, int m, int k, ull mask)
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
	for (int i = 0; i < n + m; ++i)
		if (g[i].empty())
			return 0;
	if (checkBipartate(g, k) && check(g) && 0)
	{
		static int i = 0;
		printf("\t%d\n", ++i);
		for (int i = 0; i < n; ++i)
			for (int v : g[i])
				if (i < v)
					printf("%d %d\n", i + 1, v + 1);
	}
	return checkBipartate(g, k) && check(g);
}

ull bipartiteSlow(int n, int m, int k)
{
	ull ans = 0;
	for (ull mask = 0; mask < (1LL << (n * m)); ++mask)
		ans += checkBipartate(n, m, k, mask);
	return ans;
}





const int N = 100;
// Число несвязных двудольных графов с долями размера n и m
ull dp_bipartite2more[N][N];
// Число связных двудольных графов на n + m вершинах, причём первые n вершин относятся к доле в которой лежит первая вершина
ull dp_connected[N][N];

// Число двудольных графов на n вершинах с k связными компонентами
ull dp_bipartite[N][N];

void bipartite2more(int n, int m)
{
	// k1 --- число вершин первой доли, лежащих в компоненте связности первой вершины
	// k2 --- ... во второй
	for (int k1 = 1; k1 <= n; ++k1)
		for (int k2 = 0; k2 <= m; ++k2)
			if (k1 < n || k2 < m)
				dp_bipartite2more[n][m] += C(n - 1, k1 - 1) * C(m, k2) * dp_connected[k1][k2] * (1LL << ((n - k1) * (m - k2)));
}

void connected(int n, int m)
{
	if (n == 0)
	{
		dp_connected[n][m] = m <= 1;
		return;
	}
	dp_connected[n][m] = (1LL << (n * m)) - dp_bipartite2more[n][m];
}

void bipartite(int n, int k)
{
	if (n == k)
	{
		dp_bipartite[n][k] = 1;
		return;
	}
	if (n < k)
		return;
	if (k == 0)
		return;
	
	if (k == 1)
	{
		// одна компонента связности => однозначно разбиваетя на доли
		// q --- число вершин в доле первой вершины
		for (int q = 1; q <= n; ++q)
			dp_bipartite[n][k] += C(n - 1, q - 1) * dp_connected[q][n - q];
	}
	else
	{
		// n0 --- число вершин в связной компоненте первой вершины
		for (int n0 = 1; n0 < n; ++n0)
			dp_bipartite[n][k] += C(n - 1, n0 - 1) * dp_bipartite[n0][1] * dp_bipartite[n - n0][k - 1];
	}
}

void dp()
{
	for (int n = 0; n < N; ++n)
		for (int m = 0; m < N; ++m)
		{
			bipartite2more(n, m);
			connected(n, m);
		}
	
	for (int n = 0; n < N; ++n)
		for (int k = 0; k <= n; ++k)
			bipartite(n, k);
}

ull fast(int n)
{
	ull ans = 0;
	// k --- число связных компонент
	for (int k = 0; k <= n; k++)
		ans += dp_bipartite[n][k];
	return ans;
}

const int NUM = 6;
int main()
{
	//freopen("output.txt", "w", stdout);
	initC();
	dp();
	for (int i = 0; i <= NUM; ++i)
	{
		ull ans1 = slow(i);
		ull ans2 = fast(i);
		printf("%d %d %10lld %10lld    %.2f\n", ans1 == ans2, i, ans1, ans2, ans1 / (float) all(i));
	}
	printf("OK\n");
	return 0;
}
