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
	int f;

	int r() {
		return c - f;
	}
};

struct Graph {
	Graph(int n, int m) : n(n), m(m), first(n, -1), present(m * 2, true) {}

	int n;
	int m;
	int flow = 0;
	vector<Edge> edges;
	vector<int> next;
	vector<int> first;
	vector<bool> present;

	void add_edge(int u, int v, int c) {
		add_edge_internal(u, v, c);
		add_edge_internal(v, u, c);
	}

	void add_edge_internal(int u, int v, int c) {
		edges.push_back({u, v, c, 0});
		next.push_back(first[u]);
		first[u] = (int) next.size() - 1;
	}

	int first_edge(int u) {
		int i = first[u];
		for (; i != -1 && !present[i]; i = next[i]);
		return i;
	}

	int next_edge(int i) {
		assert(i != -1);
		i = next[i];
		for (; i != -1 && !present[i]; i = next[i]);
		return i;
	}
};

bool getLevelGraph(Graph &g, int s, int t) {
	int n = g.n;
	vector<int> level(n, -1);
	vector<int> curr = {s}, next;
	for (int i = 0; !curr.empty(); ++i) {
		for (int u : curr) {
			if (level[u] == -1) {
				level[u] = i;
				for (int j = g.first[u]; j != -1; j = g.next_edge(j)) {
					Edge e = g.edges[j];
					int v = e.v;
					if (e.r() > 0 && level[v] == -1) {
						next.push_back(v);
					}
				}
			}
		}
		curr.clear();
		swap(next, curr);
	}

	for (int u = 0; u < n; ++u) {
		for (int j = g.first[u]; j != -1; j = g.next_edge(j)) {
			Edge e = g.edges[j];
			int v = e.v;
			g.present[j] = level[u] + 1 == level[v];
		}
	}
	return level[t] != -1;
}

int dfs(Graph &g, vector<int> &curr, int t, int u, int max_flow) {
	dbgt(u, max_flow);
	if (max_flow == 0) {
		return 0;
	}
	if (u == t) {
		return max_flow;
	}
	for (; curr[u] != -1; curr[u] = g.next_edge(curr[u])) {
		Edge &e = g.edges[curr[u]];
		dbgtt(curr[u], e.r(), e.u, e.v);
		int flow = dfs(g, curr, t, e.v, min(max_flow, e.r()));
		if (flow > 0) {
			e.f += flow;
			g.edges[curr[u] ^ 1].f -= flow;
			return flow;
		}
	}
	return 0;
}

void createBlockingFlow(Graph &g, int s, int t) {
	int n = g.n;
	vector<int> curr;
	for (int u = 0; u < n; ++u) {
		curr.push_back(g.first_edge(u));
	}
	while (int flow = dfs(g, curr, t, s, INT_MAX)) {
		dbg(flow);
		g.flow += flow;
	}
}

int main() {
	freopen("input.txt", "r", stdin);
	while (1) {
		int n, s, t, m;
		cin >> n;
		if (n == 0) {
			break;
		}
		cin >> s >> t >> m;
		--s, --t;
		Graph g(n, m);
		for (int i = 0; i < m * 2; i += 2) {
			int u, v, c;
			cin >> u >> v >> c;
			g.add_edge(u - 1, v - 1, c);
		}

		while (1) {
			dbg(g.flow);
			g.present.assign(m * 2, true);
			if (!getLevelGraph(g, s, t)) {
				break;
			}
			createBlockingFlow(g, s, t);
		}
		cout << g.flow << endl;
	}
	return 0;
}
