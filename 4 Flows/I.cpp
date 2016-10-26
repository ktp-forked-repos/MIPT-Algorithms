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

struct Edge {
	int i;
	int u;
	int v;
	int c;
	int t;
	int f;

	int r() {
		return c - f;
	}
};

struct Graph {
	Graph(int n) : n(n), m(0), first(n, -1) {}

	int n;
	int m;
	int flow = 0;
	vector<Edge> edges;
	vector<int> next;
	vector<int> first;

	void add_edge(int u, int v, int c, int t) {
		add_edge_internal(u, v, c, t);
		add_edge_internal(v, u, 0, -t);
	}

	void add_edge_internal(int u, int v, int c, int t) {
		edges.push_back({(int) edges.size(), u, v, c, t, 0});
		next.push_back(first[u]);
		first[u] = (int) next.size() - 1;
		++m;
	}

	void push_flow(int i, int f) {
		edges[i ^ 0].f += f;
		edges[i ^ 1].f -= f;
	}
};

vector<bool> used;
vector<int> pair_reverse;

bool dfs(vector<set<int>> &g, int u) {
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

const int inf = INT_MAX;

int main() {
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
#endif
	int n, k;
	cin >> n >> k;
	vector<vector<int>> w(n, vector<int>(n));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			cin >> w[i][j];
		}
	}

	int number_vertexes = n * 2 + 2;
	int source = number_vertexes - 1;
	int target = number_vertexes - 2;
	Graph graph(number_vertexes);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			graph.add_edge(i, n + j, 1, w[i][j]);
		}
	}
	for (int i = 0; i < n; ++i) {
		graph.add_edge(source, i, k, 0);
		graph.add_edge(i + n, target, k, 0);
	}

	/*
	int ans = 0;
	vector<int> potentials(number_vertexes, inf);
	potentials[source] = 0;
	bool change;
	do {
		change = 0;
		for (Edge &e : graph.edges) {
			if (potentials[e.u] != inf && potentials[e.u] + e.t < potentials[e.v]) {
				potentials[e.v] = potentials[e.u] + e.t;
				change = 0;
			}
		}
	} while (change);
	*/

	vector<int> potentials(number_vertexes, 0);
	long long ans = 0;
	while (graph.flow < n * k) {
		vector<int> ds(number_vertexes, inf);
		vector<int> ps(number_vertexes, -1);
		ds[source] = 0;

		/*
		set<pair<int, int>> q = {{0, source}};
		while (!q.empty()) {
			auto p = *q.begin();
			q.erase(q.begin());
			int u = p.second;
			//assert(ds[u] == cost);
			for (int i = graph.first[u]; i != -1; i = graph.next[i]) {
				Edge &e = graph.edges[i];
				if (e.r() > 0) {
					int actual_cost = e.t - potentials[e.v] + potentials[e.u];
					if (ds[u] + actual_cost < ds[e.v]) {
						q.erase({ds[e.v], e.v});
						ds[e.v] = ds[u] + actual_cost;
						ps[e.v] = e.i;
						q.insert({ds[e.v], e.v});
					}
				}
			}
		}
		*/

		vector<bool> used(number_vertexes, false);
		while (true) {
			pair<int, int> minp = {inf, -1};
			for (int i = 0; i < number_vertexes; ++i) {
				if (!used[i]) {
					minp = min(minp, {ds[i], i});
				}
			}
			int u = minp.second;
			if (u == -1) {
				break;
			}
			used[u] = true;
			for (int i = graph.first[u]; i != -1; i = graph.next[i]) {
				Edge &e = graph.edges[i];
				if (e.r() > 0) {
					int actual_cost = e.t - potentials[e.v] + potentials[e.u];
					if (ds[u] + actual_cost < ds[e.v]) {
						ds[e.v] = ds[u] + actual_cost;
						ps[e.v] = e.i;
					}
				}
			}
		}

		for (int i = 0; i < number_vertexes; ++i) {
			potentials[i] += ds[i];
		}

//		dbg(ds);
//		dbg(ps);
		if (ds[target] == inf) {
			break;
		}

		for (int u = target; ps[u] != -1; u = graph.edges[ps[u]].u) {
			graph.push_flow(ps[u], 1);
			ans += graph.edges[ps[u]].t;
//			Edge &e = graph.edges[ps[u]];
//			dbg(e.u, e.v, e.f);
		}
		graph.flow++;
	}
	assert(graph.flow == n * k);

	vector<set<int>> g2(n);
	for (Edge &e : graph.edges) {
		if (e.f > 0 && e.u < n) {
			assert(e.i % 2 == 0);
			g2[e.u].insert(e.v);
		}
	}

	cout << ans << endl;
	while (k--) {
		dbg(g2);
		used.assign(n, false);
		pair_reverse.assign(n * 2, -1);
		for (int i = 0; i < n; ++i) {
			if (dfs(g2, i)) {
				used.assign(n, 0);
			}
		}

		vector<int> pair_direct(n);
		for (int i = 0; i < n; ++i) {
			pair_direct[pair_reverse[n + i]] = i;
			g2[pair_reverse[n + i]].erase(n + i);
		}
		for (int pair : pair_direct) {
			cout << pair + 1 << " ";
		}
		cout << endl;
	}
	dbg(clock() / (double) CLOCKS_PER_SEC);
	return 0;
}