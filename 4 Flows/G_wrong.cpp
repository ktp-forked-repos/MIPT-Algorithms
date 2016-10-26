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
		add_edge_internal(v, u, c, t);
	}

	void add_edge_internal(int u, int v, int c, int t) {
		edges.push_back({u, v, c, t, 0});
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
	}

	ll ans = 0;
	vector<vector<int>> paths;

	int s = 0;
	int t = n - 1;
	while (g.flow < k) {
		vector<ll> ds(n, INT_MAX);
		vector<int> ps(n, -1);
		ds[s] = 0;
		set<PII> q = {{0, s}};
		while (!q.empty()) {
			auto p = *q.begin();
			q.erase(q.begin());
			ll d = p.first;
			int u = p.second;
			assert(ds[u] == d);
			ds[u] = d;
			for (int i = g.first[u]; i != -1; i = g.next[i]) {
				Edge e = g.edges[i];
				if (e.r() > 0 && ds[u] + e.t < ds[e.v]) {
					q.erase({ds[e.v], e.v});
					ds[e.v] = ds[u] + e.t;
					q.insert({ds[e.v], e.v});
					ps[e.v] = i;
				}
			}
		}

		dbg(ds);
		dbg(ps);
		if (ds[t] == INT_MAX) {
			cout << -1 << endl;
			return 0;
		}

		vector<int> cpath;
		for (int u = t; ps[u] != -1; u = g.edges[ps[u]].u) {
			cpath.push_back(ps[u]);
			g.push_flow(ps[u], 1);
			ans += g.edges[ps[u]].t;
			Edge &e = g.edges[ps[u]];
			dbg(e.u, e.v, e.t);
		}
		reverse(cpath.begin(), cpath.end());
		paths.push_back(cpath);
		g.flow++;
	}

	printf("%.5f\n", ans / (double) k);
	for (auto path : paths) {
		cout << path.size() << "  ";
		for (int i : path) {
			cout << i / 2 + 1 << " ";
//			printf("(%d->%d %d %d) ", g.edges[i].u + 1, g.edges[i].v + 1, g.edges[i].t, g.edges[i].c);
		}
		cout << endl;
	}
	return 0;
}