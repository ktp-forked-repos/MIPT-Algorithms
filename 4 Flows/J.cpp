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

int a[400][400];
const int inf = INT_MAX;

struct Task {
	int i;
	int start_time;
	int duration_time;
	int profit;

	int end_time() const {
		return start_time + duration_time;
	}

	bool operator<(const Task &other) const {
		return start_time < other.start_time;
	}
};

int main() {
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
#endif
	int number_tasks, number_machines;
	cin >> number_tasks >> number_machines;
	vector<Task> tasks(number_tasks);
	for (Task &task : tasks) {
		cin >> task.start_time >> task.duration_time >> task.profit;
		task.profit *= -1;
	}
	for (int i = 0; i < number_tasks; ++i) {
		tasks[i].i = i;
	}
	sort(tasks.begin(), tasks.end());

	int number_vertexes = number_tasks * 2 + 1;
	int source = number_vertexes - 1;
	int target = number_vertexes - 2;
	Graph graph(number_vertexes);
	graph.add_edge(source, 0, number_machines, 0);
	for (int i = 0; i < number_tasks; ++i) {
		graph.add_edge(i, number_tasks + i, 1, tasks[i].profit);
		if (i < number_tasks - 1) {
			graph.add_edge(i, i + 1, inf, 0);
			graph.add_edge(number_tasks + i, target, 1, 0);
			for (int j = 0; j < number_tasks; ++j) {
				if (tasks[i].end_time() <= tasks[j].start_time) {
					graph.add_edge(number_tasks + i, j, 1, 0);
					break;
				}
			}
		}
	}

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

	while (graph.flow < number_machines) {
		vector<int> ds(number_vertexes, inf);
		vector<int> ps(number_vertexes, -1);
		ds[source] = 0;

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
			Edge &e = graph.edges[ps[u]];
			dbg(e.u, e.v, e.f);
		}
		graph.flow++;
	}

	dbg(graph.flow);
	for (Edge &e :  graph.edges) {
		if (e.f == 1) {
			dbg(e.u, e.v);
		}
	}

	vector<bool> should_complete(number_tasks);
	for (Edge &e : graph.edges) {
		if (e.u + number_tasks == e.v && e.f == 1) {
			should_complete[tasks[e.u].i] = true;
		}
	}
	for (bool b : should_complete) {
		cout << b << " ";
	}
	cout << endl;
	dbg(clock() / (double) CLOCKS_PER_SEC);
	return 0;
}