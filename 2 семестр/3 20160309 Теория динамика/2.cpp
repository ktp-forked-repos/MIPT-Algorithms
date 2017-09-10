#include <bits/stdc++.h>
using namespace std;
#define dbg(x) cerr << #x << " = " << (x) << endl;
typedef unsigned long long ull;
typedef vector<int> VI;
typedef vector<VI> VVI;

bool used[100];
void dfs(int v, VVI &g)
{
	if (used[v])
		return;
	used[v] = 1;
	for (int nv : g[v])
		dfs(nv, g);
}

bool check(VVI &g)
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





// число неориентированных графов на n вершинах
ull all(int n)
{
	return 1LL << (n * (n - 1) / 2);
}

ull good(int n);

// число несвязных неориентированных графов на n вершинах
ull bad(int n)
{
	ull ans = 0;
	for (int size = 1; size < n; size++)
		ans += C(n - 1, size - 1) * good(size) * all(n - size);
	return ans;
}

// число связных неориентированных графов на n вершинах
ull good(int n)
{
	return all(n) - bad(n);
}

const int N = 6;
int main()
{
	initC();
	for (int i = 0; i <= N; ++i)
	{
		ull ans1 = slow(i);
		ull ans2 = good(i);
		printf("%d %d %10lld %10lld %.2f\n", ans1 == ans2, i, ans1, ans2, ans1 / (float) all(i));
	}
	return 0;
}
