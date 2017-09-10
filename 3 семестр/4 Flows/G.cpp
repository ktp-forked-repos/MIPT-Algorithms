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

typedef long long ll;
typedef pair<int, int> PII;

void dfs(Graph &g, int v, vector<int> &path) {
	for (int i = g.first[v]; i != -1; i = g.next[i]) {
		Edge &e = g.edges[i];
		if (e.f > 0) {
			e.f--;
			path.push_back(i);
			dfs(g, e.v, path);
			return;
		}
	}
}

int main() {
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
#endif
	int n, m, k;
	cin >> n >> m >> k;

	Graph g(n);
	for (int i = 0; i < m; ++i) {
		int u, v, t;
		cin >> u >> v >> t;
		--u, --v;
		g.add_edge(u, v, 1, t);
		g.add_edge(v, u, 1, t);
	}

	ll ans = 0;
	int s = 0;
	int t = n - 1;
	while (g.flow < k) {
		vector<ll> ds(n, INT_MAX);
		vector<int> ps(n, -1);
		ds[s] = 0;
		bool change;
		do {
			change = 0;
			for (Edge &e : g.edges) {
				if (e.r() > 0 && ds[e.u] != INT_MAX && ds[e.u] + e.t < ds[e.v]) {
					ds[e.v] = ds[e.u] + e.t;
					ps[e.v] = e.i;
					change = 1;
				}
			}
		} while (change);

		dbg(ds);
		dbg(ps);
		if (ds[t] == INT_MAX) {
			cout << -1 << endl;
			return 0;
		}

		for (int u = t; ps[u] != -1; u = g.edges[ps[u]].u) {
			g.push_flow(ps[u], 1);
			ans += g.edges[ps[u]].t;
		}
		g.flow++;
	}

	vector<vector<int>> paths(k);
	for (int i = 0; i < k; ++i) {
		dfs(g, s, paths[i]);
	}

	printf("%.5f\n", ans / (double) k);
	for (auto path : paths) {
		cout << path.size() << "  ";
		for (int i : path) {
			cout << i / 4 + 1 << " ";
//			printf("(%d->%d %d %d) ", g.edges[i].u + 1, g.edges[i].v + 1, g.edges[i].t, g.edges[i].c);
		}
		cout << endl;
	}
	return 0;
}