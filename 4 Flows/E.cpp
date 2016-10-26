#include <bits/stdc++.h>

using namespace std;

vector<bool> used;
vector<int> pair_reverse;

bool dfs(vector<vector<int>> &g, int u) {
	if (used[u]) {
		return false;
	}
	used[u] = 1;
	for (int v : g[u]) {
		if (pair_reverse[v] == -1 || dfs(g, pair_reverse[v])) {
			pair_reverse[v] = u;
			return true;
		}
	}
	return false;
}

int get_matching(vector<vector<int>> g, int n, int k) {
	used.assign(n, false);
	pair_reverse.assign(k, -1);
	for (int i = 0; i < n; ++i) {
		if (dfs(g, i)) {
			used.assign(n, 0);
		}
	}
	int matching = 0;
	for (int i : pair_reverse) {
		if (i != -1) {
			++matching;
		}
	}
	return matching;
}

int main() {
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
#endif
	int n, k;
	cin >> n >> k;
	vector<vector<int>> loves(n);
	vector<vector<int>> loved(k);
	for (int boy = 0; boy < n; ++boy) {
		int m;
		cin >> m;
		loves[boy].resize(m);
		for (int &girl : loves[boy]) {
			cin >> girl;
			--girl;
			loved[girl].push_back(boy);
		}
	}

	int q;
	cin >> q;
	while (q--) {
		int t;
		cin >> t;
		vector<int> boys(t);
		for (int &boy : boys) {
			cin >> boy;
			--boy;
		}

		vector<vector<int>> g(n);
		vector<bool> used(k, 0);
		int need_matching = 0;
		for (int boy : boys) {
			for (int girl : loves[boy]) {
				if (!used[girl]) {
					used[girl] = 1;
					++need_matching;
					for (int boy0 : loved[girl]) {
						if (boy0 < boy) {
							g[boy0].push_back(girl);
						}
					}
				}
			}
		}

		int matching = get_matching(g, n, k);
		printf("%s\n", matching == need_matching ? "Yes" : "No");
	}
	return 0;
}