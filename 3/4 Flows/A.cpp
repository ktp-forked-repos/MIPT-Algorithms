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
	Graph(int n) : n(n), m(0), first(n, -1) {}

	int n;
	int m;
	int flow = 0;
	vector<Edge> edges;
	vector<int> next;
	vector<int> first;
	vector<bool> present;

	void add_edge(int u, int v, int c) {
		add_edge_internal(u, v, c);
		add_edge_internal(v, u, 0);
	}

	void add_edge_internal(int u, int v, int c) {
		edges.push_back({u, v, c, 0});
		next.push_back(first[u]);
		first[u] = (int) next.size() - 1;
		present.push_back(true);
		++m;
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

vector<vector<int>> is;
int cost_of_letter[256];

int main() {
//	freopen("input.txt", "r", stdin);
	int n, m;
	cin >> n >> m;
	dbg(n, m);
	vector<string> a(n);
	for (string &s : a)
		cin >> s;
	for (string &s : a)
		s = '.' + s + '.';
	a.insert(a.begin(), "");
	a.push_back("");

	cost_of_letter['H'] = 1;
	cost_of_letter['O'] = 2;
	cost_of_letter['N'] = 3;
	cost_of_letter['C'] = 4;

	int k = 2;
	int sumcost = 0;
	is.assign(n + 2, vector<int>(m + 2, 0));
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			if (a[i][j] != -1) {
				sumcost += cost_of_letter[a[i][j]];
				is[i][j] = k++;
			}
		}
	}

	const int di[4] = {-1, 0, 0, +1};
	const int dj[4] = {0, -1, +1, 0};

	Graph g(k);
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			if (is[i][j]) {
				int f = (i + j) % 2;
				int cost = cost_of_letter[a[i][j]];
				f ? g.add_edge(0, is[i][j], cost) : g.add_edge(is[i][j], 1, cost);
				if (f) {
					for (int k = 0; k < 4; ++k) {
						int ni = i + di[k];
						int nj = j + dj[k];
						if (is[ni][nj]) {
							g.add_edge(is[i][j], is[ni][nj], 1);
						}
					}
				}
			}
		}
	}

	while (1) {
		dbg(g.flow);
		g.present.assign(g.m, true);
		if (!getLevelGraph(g, 0, 1)) {
			break;
		}
		createBlockingFlow(g, 0, 1);
	}

//	cout << g.flow << endl;
	printf("%s\n", g.flow * 2 == sumcost && sumcost > 0 ? "Valid" : "Invalid");

	return 0;
}