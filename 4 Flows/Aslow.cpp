#include <bits/stdc++.h>

#ifdef LOCAL

#include "../headers/debug.h"

#else
void nop() {}
#define dbg(...) nop
#define dbgl(...) nop
#define dbgt(...) nop
#define dbgtt(...) nop
#define dbgttt(...) nop
#define dbgtttt(...) nop
#endif

using namespace std;

const int K = 1e6;
vector<vector<int>> g;
vector<int> pair_reverse;
//vector<bool> used;
bool preused[K];
bool used[K];

bool dfs(int u) {
	if (used[u]) {
		return false;
	}
	used[u] = true;
	for (int v : g[u]) {
		if (pair_reverse[v] == -1 || dfs(pair_reverse[v])) {
			dbg(v, u);
			pair_reverse[v] = u;
			return true;
		}
	}
	return false;
}

vector<string> a;
vector<vector<int>> is;
vector<int> one;
int costs[256];

int add_edge(int d, int i, int j, int di, int dj) {
	int ni = i + di;
	int nj = j + dj;
	if (a[i][j] == '.' || a[ni][nj] == '.') {
		return 0;
	}
	for (int nd = 0; nd < costs[a[ni][nj]]; ++nd) {
		g[is[i][j] + d].push_back(is[ni][nj] + nd);
	}
	return d + 1;
}

int main() {
	freopen("input.txt", "r", stdin);
	int n, m;
	cin >> n >> m;
	dbg(n, m);
	a.resize(n);
	for (string &s : a)
		cin >> s;
	for (string &s : a)
		s = '.' + s + '.';
	a.insert(a.begin(), "");
	a.push_back("");

	dbg(a);

	costs['H'] = 1;
	costs['O'] = 2;
	costs['N'] = 3;
	costs['C'] = 4;

	int k = 0;
	is.assign(n + 2, vector<int>(m + 2));
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			is[i][j] = k;
			k += costs[a[i][j]];
			for (int d = 0; d < costs[a[i][j]]; ++d) {
				one.push_back((i + j) % 2);
			}
		}
	}

	if (k % 2 == 1) {
		printf("Invalid\n");
		return 0;
	}

	g.resize(k);
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			int d = 0;
			d = max(d, add_edge(d, i, j, -1, 0));
			d = max(d, add_edge(d, i, j, 0, -1));
			d = max(d, add_edge(d, i, j, 0, +1));
			d = max(d, add_edge(d, i, j, +1, 0));
		}
	}

	pair_reverse.assign(k, -1);
	memset(preused, 0, k);
	for (int i = 0; i < k; ++i) {
		if (one[i]) {
			for (int v : g[i]) {
				if (pair_reverse[v] == -1) {
					pair_reverse[v] = i;
					preused[i] = true;
					break;
				}
			}
		}
	}
	dbg(pair_reverse);
	memset(used, 0, k);
	for (int i = 0; i < k; ++i) {
		if (!preused[i] && one[i] && dfs(i)) {
			memset(used, 0, k);
		}
	}
	dbg(pair_reverse);

	int sum = 0;
	for (int i = 0; i < k; ++i) {
		sum += pair_reverse[i] != -1;
	}
	dbg(sum);

	printf("%s\n", sum * 2 == k ? "Valid" : "Invalid");
	return 0;
}