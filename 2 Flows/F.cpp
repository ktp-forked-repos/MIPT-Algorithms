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

vector<vector<int>> g;
vector<int> pair_reverse;
vector<int> pair_direct;
vector<bool> used;

bool dfs(int u) {
	if (used[u]) {
		return false;
	}
	used[u] = true;
	for (int v : g[u]) {
		if (pair_reverse[v] == -1 || dfs(pair_reverse[v])) {
			pair_reverse[v] = u;
			return true;
		}
	}
	return false;
}

vector<bool> useda;
vector<bool> usedb;

void dfs2(int u) {
	if (useda[u]) {
		return;
	}
	useda[u] = true;
	for (int v : g[u]) {
		usedb[v] = true;
		if (pair_reverse[v] != -1) {
			dfs2(pair_reverse[v]);
		}
	}
}

int main() {
	//freopen("input.txt", "r", stdin);
	int k;
	cin >> k;
	while (k--) {
		int n, m;
		cin >> n >> m;

		set<pair<int, int>> edges;
		for (int i = 0; i < n; ++i) {
			int x;
			while ((cin >> x) && x != 0) {
				edges.insert({i, x - 1});
			}
		}

		g.assign(n, {});
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				if (edges.find({i, j}) == edges.end()) {
					g[i].push_back(j);
				}
			}
		}

		pair_reverse.assign(m, -1);
		used.assign(n, 0);
		for (int i = 0; i < n; ++i) {
			if (dfs(i)) {
				used.assign(n, 0);
			}
		}

		// B- and A+
		pair_direct.assign(n, -1);
		for (int j = 0; j < m; ++j) {
			if (pair_reverse[j] != -1) {
				pair_direct[pair_reverse[j]] = j;
			}
		}
		useda.assign(n, 0);
		usedb.assign(m, 0);
		for (int i = 0; i < n; ++i) {
			if (pair_direct[i] == -1) {
				dfs2(i);
			}
		}

//		dbg(pair_reverse);
//		dbg(pair_direct);

		int oka = accumulate(useda.begin(), useda.end(), 0);
		int okb = accumulate(usedb.begin(), usedb.end(), 0);
		okb = m - okb;
		cout << oka + okb << endl;
		cout << oka << " " << okb << endl;
		for (int i = 0; i < n; ++i) {
			if (useda[i]) {
				cout << i + 1 << " ";
			}
		}
		cout << endl;
		for (int j = 0; j < m; ++j) {
			if (!usedb[j]) {
				cout << j + 1 << " ";
			}
		}
		cout << endl;
		cout << endl;
	}
	return 0;
}