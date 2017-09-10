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
private:
	int number_vertexes;
	vector<Edge> edges;
	vector<int> next_edges;
	vector<int> first_edge_for_vertex;

	void add_edge_internal(int from, int to, int capacity) {
		edges.push_back({(int) edges.size(), from, to, capacity, 0});
		next_edges.push_back(first_edge_for_vertex[from]);
		first_edge_for_vertex[from] = (int) next_edges.size() - 1;
	}

public:
	int flow = 0;

	Network(int number_vertexes = 0) : number_vertexes(number_vertexes), first_edge_for_vertex(number_vertexes, -1) {}

	int get_number_vertexes() {
		return number_vertexes;
	}

	int get_number_edges() {
		return edges.size();
	}

	int add_vertex() {
		first_edge_for_vertex.push_back(-1);
		++number_vertexes;
		return number_vertexes - 1;
	}

	void add_edge(int from, int to, int capacity = 1) {
		add_edge_internal(from, to, capacity);
		add_edge_internal(to, from, capacity);
	}

	void push_flow(int vertex_index, int flow) {
		edges[vertex_index ^ 0].flow += flow;
		edges[vertex_index ^ 1].flow -= flow;
	}

	class EdgesFromVertex {
	private:
		const Network *network;
		int vertex;

	public:
		EdgesFromVertex(const Network *network, int vertex) : network(network), vertex(vertex) {}

		class iterator {
			const Network *network;
			int edge_index;

		public:
			iterator(const Network *network, int edge_index) : network(network), edge_index(edge_index) {}

			void operator++() {
				edge_index = network->next_edges[edge_index];
			}

			bool operator!=(const iterator &other) const {
				return edge_index != other.edge_index;
			}

			const Edge &operator*() const {
				return network->edges[edge_index];
			}

			const Edge *operator->() const {
				return &network->edges[edge_index];
			}
		};

		iterator begin() const {
			return iterator(network, network->first_edge_for_vertex[vertex]);
		}

		iterator end() const {
			return iterator(nullptr, -1);
		}
	};

	EdgesFromVertex operator[](int vertex) const {
		return {this, vertex};
	}

	typedef EdgesFromVertex::iterator iterator;

	vector<Edge>::iterator begin() {
		return edges.begin();
	}

	vector<Edge>::iterator end() {
		return edges.end();
	}
};

class Dinic {
public:
	Dinic(Network &network, int source, int target) : network(network), source(source), target(target) {}

	void run() {
		while (1) {
			present.assign(network.get_number_edges(), true);
			if (!getLevelGraph()) {
				break;
			}
			createBlockingFlow();
		}
	}

private:
	Network &network;
	int source;
	int target;
	vector<bool> present;

	bool getLevelGraph() {
		vector<int> levels(network.get_number_vertexes(), -1);
		vector<int> vertexes_of_current_level = {source}, vertexes_of_next_level;
		for (int level = 0; !vertexes_of_current_level.empty(); ++level) {
			for (int vertex : vertexes_of_current_level) {
				if (levels[vertex] == -1) {
					levels[vertex] = level;
					for (Edge edge : network[vertex]) {
						int to = edge.to;
						if (edge.get_residual_capacity() > 0 && levels[to] == -1) {
							vertexes_of_next_level.push_back(to);
						}
					}
				}
			}
			vertexes_of_current_level.clear();
			swap(vertexes_of_next_level, vertexes_of_current_level);
		}

		for (int vertex = 0; vertex < network.get_number_vertexes(); ++vertex) {
			for (Edge edge : network[vertex]) {
				int to = edge.to;
				present[edge.index] = levels[vertex] + 1 == levels[to];
			}
		}

		return levels[target] != -1;
	}

	int dfs(vector<Network::iterator> &last_edge_for_vertex, int vertex, int max_flow) {
		if (max_flow == 0) {
			return 0;
		}
		if (vertex == target) {
			return max_flow;
		}
		for (; last_edge_for_vertex[vertex] != network[vertex].end(); ++last_edge_for_vertex[vertex]) {
			if (present[last_edge_for_vertex[vertex]->index]) {
				const Edge &edge = *last_edge_for_vertex[vertex];
				int flow = dfs(last_edge_for_vertex, edge.to, min(max_flow, edge.get_residual_capacity()));
				if (flow > 0) {
					network.push_flow(edge.index, flow);
					return flow;
				}
			}
		}
		return 0;
	}

	void createBlockingFlow() {
		vector<Network::iterator> last_edge_for_vertex;
		for (int vertex = 0; vertex < network.get_number_vertexes(); ++vertex) {
			last_edge_for_vertex.push_back(network[vertex].begin());
		}
		while (int flow = dfs(last_edge_for_vertex, source, INT_MAX)) {
			network.flow += flow;
		}
	}
};

vector<int> add_vertexes(Network &network, string s) {
	vector<int> indexes(s.length(), -1);
	for (int i = 0; i < s.length(); ++i) {
		if (s[i] == '?') {
			indexes[i] = network.add_vertex();
		}
	}
	return indexes;
}

class Task {
public:
	Task(const string &s, const string &p) : s(s), p(p) {}

	pair<int, pair<string, string>> solve() {
		Network network(2);
		vector<int> s_indexes = add_vertexes(network, s);
		vector<int> p_indexes = add_vertexes(network, p);

		int answer = 0;
		for (int i = 0; i + p.length() <= s.length(); ++i) {
			for (int j = 0; j < p.length(); ++j) {
				char char_of_s = s[i + j];
				char char_of_p = p[j];
				network.add_edge(char_of_p == '?' ? p_indexes[j] : (char_of_p - '0'), char_of_s == '?' ? s_indexes[i + j] : (char_of_s - '0'));
			}
		}

		Dinic dinic(network, 0, 1);
		dinic.run();
		answer += network.flow;

		graph.assign(network.get_number_vertexes(), {});
		for (Edge edge : network) {
			if (edge.get_residual_capacity() > 0) {
				graph[edge.from].push_back(edge.to);
			}
		}

		used.assign(network.get_number_vertexes(), 0);
		dfs(0);

		for (int i = 0; i < s.length(); ++i) {
			if (s_indexes[i] != -1) {
				s[i] = used[s_indexes[i]] ? '0' : '1';
			}
		}
		for (int i = 0; i < p.length(); ++i) {
			if (p_indexes[i] != -1) {
				p[i] = used[p_indexes[i]] ? '0' : '1';
			}
		}

		return {answer, {s, p}};
	}

private:
	string s;
	string p;

	vector<bool> used;
	vector<vector<int>> graph;

	void dfs(int vertex) {
		if (used[vertex]) {
			return;
		}
		used[vertex] = true;
		for (int next_vertex : graph[vertex]) {
			dfs(next_vertex);
		}
	}
};

int main() {
	freopen("input.txt", "r", stdin);

	string s, p;
	cin >> s >> p;
	Task task(s, p);
	auto answer = task.solve();
	cout << answer.first << endl;
	cout << answer.second.first << endl;
	cout << answer.second.second << endl;
	return 0;
}
