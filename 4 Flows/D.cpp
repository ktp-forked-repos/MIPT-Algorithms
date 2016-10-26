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

int dist(int a, int b, int c, int d) {
	return abs(a - c) + abs(b - d);
}

struct Entry {
	int h, m, a, b, c, d;

	int start_time() {
		return h * 60 + m;
	}

	int end_time() {
		return h * 60 + m + dist(a, b, c, d);
	}

	bool to(Entry &e) {
		return end_time() + dist(c, d, e.a, e.b) < e.start_time();
	}
};

int main() {
//	freopen("input.txt", "r", stdin);
	int m;
	cin >> m;
	vector<Entry> es(m);
	for (Entry &e : es) {
		scanf("%d:%d %d %d %d %d", &e.h, &e.m, &e.a, &e.b, &e.c, &e.d);
	}

	int k = m * 2;
	g.resize(k);
	for (int i = 0; i < m; ++i) {
		for (int j = i + 1; j < m; ++j) {
			if (es[i].to(es[j])) {
				g[i].push_back(j * 2);
			}
		}
	}
	dbg(g);

	pair_reverse.assign(k, -1);
	memset(preused, 0, k);
	for (int i = 0; i < m; ++i) {
		for (int v : g[i]) {
			if (pair_reverse[v] == -1) {
				pair_reverse[v] = i;
				preused[i] = true;
				break;
			}
		}
	}
	dbg(pair_reverse);
	memset(used, 0, k);
	for (int i = 0; i < m; ++i) {
		if (!preused[i] && dfs(i)) {
			memset(used, 0, k);
		}
	}
	dbg(pair_reverse);

	int sum = 0;
	for (int i = 0; i < k; ++i) {
		sum += pair_reverse[i] != -1;
	}
	dbg(sum);
	cout << m - sum << endl;
	return 0;
}