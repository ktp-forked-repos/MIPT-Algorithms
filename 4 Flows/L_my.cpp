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

bool a[200][200];

int main() {
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
#endif
	int n;
	cin >> n;

//	int m = 0;
	int k = n * 2;
	g.resize(k);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j)
			cin >> a[i][j];
	}
	for (int k = 0; k < n; ++k) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				if (a[i][k] && a[k][j]) {
					a[i][j] = 1;
				}
			}
		}
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (a[i][j]) {
				g[i].push_back(n + j);
			}
		}
	}

	pair_reverse.assign(k, -1);
	memset(preused, 0, k);
	/*
	for (int i = 0; i < n; ++i) {
		for (int v : g[i]) {
			if (pair_reverse[v] == -1) {
				pair_reverse[v] = i;
				preused[i] = true;
				break;
			}
		}
	}
	dbg(pair_reverse);
	// */
	memset(used, 0, k);
	for (int i = 0; i < n; ++i) {
		if (!preused[i] && dfs(i)) {
			memset(used, 0, k);
		}
	}
	dbg(pair_reverse);

	vector<int> ans;
	for (int i = 0; i < n; ++i) {
		if (pair_reverse[n + i] == -1) {
			ans.push_back(i);
		}
	}

	cout << ans.size() << endl;
	for (int x : ans)
		cout << x + 1 << " ";
	cout << endl;
	return 0;
}