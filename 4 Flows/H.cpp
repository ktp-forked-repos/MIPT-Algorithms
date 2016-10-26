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

/*
const int MEMORY_SIZE = 200 * 1024 * 1024;
char memory[MEMORY_SIZE];
int memory_pointer;

void *operator new(size_t size) {
	void *ret = memory + memory_pointer;
	memory_pointer += size;
	assert(memory_pointer <= MEMORY_SIZE);
	return ret;
}

void operator delete(void *pointer) {}
*/

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

const int inf = INT_MAX;

int a[400][400];
const int K = 1000;
int ds[K];
int ps[K];
bool used[K];

int main() {
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
#endif
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			scanf("%d", &a[i][j]);
		}
	}

	int s = n * 2;
	int t = n * 2 + 1;
	int k = n * 2 + 2;
	Graph g(k);
	for (int i = 0; i < n; ++i) {
		g.add_edge(s, i, 1, 0);
		g.add_edge(n + i, t, 1, 0);
	}
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			g.add_edge(i, n + j, 1, a[i][j]);
		}
	}

	int ans = 0;
	vector<int> potentials(k, 0);
	while (g.flow < n) {
//		vector<int> ds(k, inf);
//		vector<int> ps(k, -1);
		for (int i = 0; i < k; ++i) {
			ds[i] = inf;
			ps[i] = -1;
			used[i] = false;
		}
		ds[s] = 0;

		/*
//		set<pair<int, int>> q = {{0, s}};
		set<ll> q = {s};
		while (!q.empty()) {
//			auto p = *q.begin();
//			q.erase(q.begin());
//			int u = p.second;
			ll x = *q.begin();
			int u = x % 1000;
			q.erase(q.begin());
			//assert(ds[u] == cost);
			for (int i = g.first[u]; i != -1; i = g.next[i]) {
				Edge &e = g.edges[i];
				if (e.r() > 0) {
					int actual_cost = e.t - potentials[e.v] + potentials[e.u];
					if (ds[u] + actual_cost < ds[e.v]) {
//						q.erase({ds[e.v], e.v});
						q.erase(ds[e.v] * 1000 + e.v);
						ds[e.v] = ds[u] + actual_cost;
						ps[e.v] = e.i;
//						q.insert({ds[e.v], e.v});
						q.insert(ds[e.v] * 1000 + e.v);
					}
				}
			}
		}
		*/

		while (true) {
			pair<int, int> minp = {inf, -1};
			for (int i = 0; i < k; ++i) {
				if (!used[i]) {
					minp = min(minp, {ds[i], i});
				}
			}
			int u = minp.second;
			if (u == -1) {
				break;
			}
			used[u] = true;
			for (int i = g.first[u]; i != -1; i = g.next[i]) {
				Edge &e = g.edges[i];
				if (e.r() > 0) {
					int actual_cost = e.t - potentials[e.v] + potentials[e.u];
					if (ds[u] + actual_cost < ds[e.v]) {
						ds[e.v] = ds[u] + actual_cost;
						ps[e.v] = e.i;
					}
				}
			}
		}

		for (int i = 0; i < k; ++i) {
			potentials[i] += ds[i];
		}

//		dbg(ds);
//		dbg(ps);
		if (ds[t] == inf) {
			cout << -1 << endl;
			return 0;
		}

		for (int u = t; ps[u] != -1; u = g.edges[ps[u]].u) {
			g.push_flow(ps[u], 1);
			ans += g.edges[ps[u]].t;
//			Edge &e = g.edges[ps[u]];
//			dbg(e.u, e.v, e.f);
		}
		g.flow++;
	}

//	dbg(g.flow);
	cout << ans << endl;
	for (Edge &e : g.edges) {
		if (e.u < n && e.v < n * 2 && e.f == 1) {
			printf("%d %d\n", e.u + 1, e.v - n + 1);
		}
	}
	dbg(clock() / (double) CLOCKS_PER_SEC);
	return 0;
}