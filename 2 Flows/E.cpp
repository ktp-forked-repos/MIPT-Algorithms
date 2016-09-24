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

void try_add(int u, int v) {
	if (u != -1 && v != -1) {
		g[u].push_back(v);
	}
}

int main() {
	freopen("input.txt", "r", stdin);
	int n, m, a, b;
	cin >> n >> m >> a >> b;

	vector<vector<int>> is(n + 2, vector<int>(m + 2, -1));
	vector<int> ktoi, ktoj;
	int k = 0;
	for (int i = 1; i <= n; ++i) {
		string s;
		cin >> s;
		for (int j = 1; j <= m; ++j) {
			if (s[j - 1] == '*') {
				ktoi.push_back(i);
				ktoj.push_back(j);
				is[i][j] = k++;
			}
		}
	}
	dbg(is);

	if (b * 2 <= a) {
		cout << k * b << endl;
		return 0;
	}

	g.resize(k);
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			try_add(is[i][j], is[i - 1][j]);
			try_add(is[i][j], is[i][j - 1]);
			try_add(is[i][j], is[i][j + 1]);
			try_add(is[i][j], is[i + 1][j]);
		}
	}
	dbg(g);

	pair_reverse.assign(k, -1);
	memset(preused, 0, k);
	for (int i = 0; i < k; ++i) {
		if ((ktoi[i] + ktoj[i]) % 2 == 0) {
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
		if (!preused[i] && (ktoi[i] + ktoj[i]) % 2 == 0 && dfs(i)) {
			memset(used, 0, k);
		}
	}
	dbg(pair_reverse);

	int sum = 0;
	for (int i = 0; i < k; ++i) {
		sum += pair_reverse[i] != -1;
	}
	dbg(sum);
	cout << sum * a + (k - sum * 2) * b << endl;
	//cout << (clock() * 1.0 / CLOCKS_PER_SEC) << endl;
	return 0;
}