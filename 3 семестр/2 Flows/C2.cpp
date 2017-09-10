#include <bits/stdc++.h>

using namespace std;

class Edge {
	friend class Network;

public:
	const int index;
	const int from;
	const int to;
	const int capacity;

	Edge(const int index, const int from, const int to, const int capacity, int flow) : index(index), from(from), to(to), capacity(capacity), flow(flow) {}

	int get_residual_capacity() const {
		return capacity - flow;
	}

private:
	int flow;
};

class Network {
public:
	Network(int number_vertexes = 0) : number_vertexes(number_vertexes), first_edge_for_vertex(number_vertexes, -1) {}

	int number_vertexes;
	int number_edges = 0;
	int flow = 0;
	vector<Edge> edges;
	vector<int> next_edge;
	vector<int> first_edge_for_vertex;

	int get_number_vertexes() const {
		return number_vertexes;
	}

	int get_number_edges() const {
		return number_edges;
	}

	int get_flow() const {
		return flow;
	}

	int add_vertex() {
		first_edge_for_vertex.push_back(-1);
		++number_vertexes;
		return number_vertexes - 1;
	}

	void add_edge(int from, int to, int capacity = 1) {
		add_edge_internal(from, to, capacity);
		add_edge_internal(to, from, capacity);
		++number_edges;
	}

	void add_edge_internal(int from, int to, int capacity) {
		edges.push_back({(int) edges.size(), from, to, capacity, 0});
		next_edge.push_back(first_edge_for_vertex[from]);
		first_edge_for_vertex[from] = (int) next_edge.size() - 1;
	}

	int first_edge(int u) {
		int i = first_edge_for_vertex[u];
		return i;
	}

	int next_edge1(int i) {
		assert(i != -1);
		i = next_edge[i];
		return i;
	}

	void push_flow(int index_edge, int flow) {
		edges[index_edge ^ 0].flow += flow;
		edges[index_edge ^ 1].flow -= flow;
	}

	class EdgesFromVertex {
	private:
		const Network *network;
		int vertex;

	public:
		EdgesFromVertex(const Network *network, int u) : network(network), vertex(u) {}

		class iterator {
			const Network *network;
			int i;

		public:
			iterator(const Network *network, int i) : network(network), i(i) {}

			void operator++() {
				i = network->next_edge[i];
			}

			bool operator!=(const iterator &other) const {
				return i != other.i;
			}

			const Edge &operator*() const {
				return network->edges[i];
			}

			const Edge *operator->() const {
				return &network->edges[i];
			}
		};

		iterator begin() const {
			return iterator(network, network->first_edge_for_vertex[vertex]);
		}

		iterator end() const {
			return iterator(nullptr, -1);
		}
	};

	EdgesFromVertex operator[](int u) const {
		return {this, u};
	}

	typedef EdgesFromVertex::iterator iterator;
};

vector<int> add_vertexes(Network &g, string s) {
	vector<int> is(s.length(), -1);
	for (int i = 0; i < s.length(); ++i) {
		if (s[i] == '?') {
			is[i] = g.add_vertex();
		}
	}
	return is;
}

vector<bool> used;
vector<vector<int>> g2;

void dfs(int u) {
	if (used[u]) {
		return;
	}
	used[u] = true;
	for (int v : g2[u]) {
		dfs(v);
	}
}

class Dinic {
public:
	void run_on(Network &g) {
		while (1) {
			present.assign(g.get_number_edges(), true);
			if (!getLevelGraph(g, 0, 1)) {
				break;
			}
			createBlockingFlow(g, 0, 1);
		}
	}

private:
	vector<bool> present;

	bool getLevelGraph(Network &g, int s, int t) {
		int n = g.number_vertexes;
		vector<int> level(n, -1);
		vector<int> curr = {s}, next;
		for (int i = 0; !curr.empty(); ++i) {
			for (int u : curr) {
				if (level[u] == -1) {
					level[u] = i;
					for (int j = g.first_edge_for_vertex[u]; j != -1; j = g.next_edge1(j)) {
//					for (Edge e : g[u]) {
						Edge e = g.edges[j];
						int v = e.to;
						if (e.get_residual_capacity() > 0 && level[v] == -1) {
							next.push_back(v);
						}
					}
				}
			}
			curr.clear();
			swap(next, curr);
		}

		for (int u = 0; u < n; ++u) {
//			for (Edge e : g[u]) {
			for (int j = g.first_edge_for_vertex[u]; j != -1; j = g.next_edge1(j)) {
				Edge e = g.edges[j];
				int v = e.to;
				present[j] = level[u] + 1 == level[v];
//				present[e.index] = level[u] + 1 == level[v];
			}
		}

		return level[t] != -1;
	}

	int dfs(Network &g, vector<Network::iterator> &curr, int t, int u, int max_flow) {
//	int dfs(Network &g, vector<int> &curr, int t, int u, int max_flow) {
		if (max_flow == 0) {
			return 0;
		}
		if (u == t) {
			return max_flow;
		}
//		for (Edge e : g[u]) {
//		for (; curr[u] != -1; curr[u] = g.next_edge(curr[u])) {
		for (; curr[u] != g[u].end(); ++curr[u]) {
			if (present[curr[u]->index]) {
//			if (present[curr[u]]) {
//				Edge &e = g.edges[curr[u]];
				const Edge &e = *curr[u];
				int flow = dfs(g, curr, t, e.to, min(max_flow, e.get_residual_capacity()));
				if (flow > 0) {
					g.push_flow(e.index, flow);
//					const_cast<Edge &>(e).f += flow;
//					g.edges[e.i ^ 1].f -= flow;
					return flow;
				}
			}
		}

		return 0;
	}

	void createBlockingFlow(Network &g, int s, int t) {
		int n = g.number_vertexes;
//		vector<int> curr;
		vector<Network::iterator> curr;
		for (int u = 0; u < n; ++u) {
//			curr.push_back(g.first_edge(u));
			curr.push_back(g[u].begin());
		}
		while (int flow = dfs(g, curr, t, s, INT_MAX)) {
			g.flow += flow;
		}
	}
};

int main() {
	freopen("input.txt", "r", stdin);

	string s, t;
	cin >> s >> t;
	int ns = s.length();
	int nt = t.length();

	Network g(2);
	vector<int> si = add_vertexes(g, s);
	vector<int> ti = add_vertexes(g, t);

	int ans = 0;
	for (int i = 0; i + nt <= ns; ++i) {
		for (int j = 0; j < nt; ++j) {
			char ct = t[j];
			char cs = s[i + j];
			g.add_edge(ct == '?' ? ti[j] : (ct - '0'), cs == '?' ? si[i + j] : (cs - '0'));
		}
	}

	Dinic d;
	d.run_on(g);
	ans += g.flow;

	g2.assign(g.number_vertexes, {});
	for (Edge e : g.edges) {
		if (e.get_residual_capacity() > 0) {
			g2[e.from].push_back(e.to);
		}
	}

	used.assign(g.number_vertexes, 0);
	dfs(0);

	for (int i = 0; i < ns; ++i) {
		if (si[i] != -1) {
			s[i] = used[si[i]] ? '0' : '1';
		}
	}
	for (int i = 0; i < nt; ++i) {
		if (ti[i] != -1) {
			t[i] = used[ti[i]] ? '0' : '1';
		}
	}

	cout << ans << endl;
	cout << s << endl;
	cout << t << endl;
	return 0;
}
