#include <bits/stdc++.h>

using namespace std;

struct Edge {
	int u;
	int v;
	int c;
	int f;
};

void createBlockingFlow(Graph vector);

typedef vector<int> VI;
typedef vector<vector<Edge>> Graph;

Graph getResidualGraph(Graph g) {
	int n = g.size();
	Graph r(n);
	for (int u = 0; u < n; ++u) {
		for (Edge e : g[u]) {
			int v = e.v;
			int c = e.c;
			int f = e.f;
			r[u].push_back({v, c - f, 0});
			r[v].push_back({u, f, 0});
		}
	}
	return r;
}


pair<Graph, bool> getLevelGraph(Graph g, int s, int t) {
	int n = g.size();
	VI level(n, -1);
	VI curr = {s}, next;
	for (int i = 0; !curr.empty(); ++i) {
		for (int u : curr) {
			if (level[u] == -1) {
				level[u] = i;
				for (Edge e : g[u]) {
					int v = e.v;
					if (level[v] == -1) {
						next.push_back(v);
					}
				}
			}
		}
		curr.clear();
		swap(next, curr);
	}

	Graph r(n);
	for (int u = 0; u < n; ++u) {
		for (Edge e : g[u]) {
			int v = e.v;
			int c = e.c;
			int f = e.f;
			if (level[u] + 1 == level[v]) {
				r[u].push_back({v, c, f});
			}
		}
	}
	return {r, level[t] != -1};
}

int dfs(Graph &g, VI &curr, int t, int u, int max_flow) {
	if (u == t) {
		return max_flow;
	}
	if (curr[u] == g[u].size()) {
		return 0;
	}
	Edge &e = g[u][curr[u]];
	int flow = dfs(g, curr, t, e.v, min(max_flow, e.c - e.f));
	if (flow == 0) {
		++curr[u];
	} else {
		e.f += flow;
	}
	return flow;
}

void createBlockingFlow(Graph &g, int s, int t) {
	int n = g.size();
	VI curr(n, 0);
	dfs(g, curr, t, s, INT_MAX);
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
		vector<Edge> g(m * 2);
		for (int i = 0; i < m * 2; i += 2) {
			int u, v, c;
			cin >> u >> v >> c;
			g[i * 2 + 0].u = u;
			g[i * 2 + 0].v = v;
			g[i * 2 + 0].c = c;
			g[i * 2 + 0].f = f;
			g[i * 2 + 1].u = v;
			g[i * 2 + 1].v = u;
			g[i * 2 + 1].c = 0;
			g[i * 2 + 1].f = f;
		}

		while (1) {
			Graph r = getResidualGraph(g);

			auto p = getLevelGraph(r, s, t);
			if (!p.second) {
				break;
			}
			Graph l = p.first;

			createBlockingFlow(l, s, t);

			g = l;
		}
	}

	return 0;
}
