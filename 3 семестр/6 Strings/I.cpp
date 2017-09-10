#include <bits/stdc++.h>
#include <stdlib.h>
using namespace std;

#ifdef LOCAL
#include "/home/dima/C++/debug.h"
#else
void nop() {}
#define dbg(...) nop();
#define dbgt(...) nop();
#endif

class Bor;

struct Vertex {
	const Bor &bor;
	int index;

	array<int, 128> next;
	bool endsIn = false;
	int isBad = -1;
	int parent;
	char lastChar;
	int suffixLink;

	Vertex(Bor &bor, int index, char lastChar, int parent, int suffixLink = -1) : bor(bor), index(index), parent(parent), lastChar(lastChar), suffixLink(suffixLink) {
		next.fill(-1);
	}
};

string get(const Vertex &vertex);

ostream &operator<<(ostream &out, const Vertex &vertex) {
	out << "{";
	for (char c = '0'; c <= '1'; ++c) {
		if (vertex.next[c] > 0) {
			out << c << "->" << vertex.next[c] << ", ";
		}
	}
	out << "parent=" << vertex.parent << ", ";
	string s = get(vertex);
	out << (s.empty() ? "ε" : s);
	return out << "}";
}

struct Bor {
	vector<Vertex> vertexes = {Vertex(*this, 0, '$', 0, 0)};

	void insert(string s) {
		int vertex = 0;
		for (int i = 0; i < (int) s.length(); ++i) {
			int nextVertex = vertexes[vertex].next[s[i]];
			if (nextVertex == -1) {
				nextVertex = (int) vertexes.size();
				vertexes.push_back(Vertex(*this, nextVertex, s[i], vertex));
				vertexes[vertex].next[s[i]] = nextVertex;
			}
			vertex = nextVertex;
		}
		vertexes[vertex].endsIn = true;
	}

	int getSuffixLink(int vertex) {
		assert(0 <= vertex && vertex <= (int) vertexes.size());
		int &suffixLink = vertexes[vertex].suffixLink;
		if (suffixLink != -1) {
			return suffixLink;
		}
		if (vertex == 0 || vertexes[vertex].parent == 0) {
			return suffixLink = 0;
		}
		return suffixLink = getNextVertex(getSuffixLink(vertexes[vertex].parent), vertexes[vertex].lastChar);
	}

	int getNextVertex(int vertex, char c) {
		int &next = vertexes[vertex].next[c];
		if (next != -1) {
			return next;
		}
		if (vertex == 0) {
			return next = 0;
		}
		return next = getNextVertex(getSuffixLink(vertex), c);
	}

	int isBad(int vertex) {
//		static int i = 0;
//		assert(++i < 100);
		int &is = vertexes[vertex].isBad;
		if (is != -1) {
			return is;
		}
		if (vertexes[vertex].endsIn) {
			return is = 1;
		}
		if (vertex == 0) {
			return is = 0;
		}
		return is = isBad(getSuffixLink(vertex));
	}

	void Aho_Corasick() {
//		for (int vertex = 0; vertex < (int) vertexes.size(); ++vertex) {
//			dbg(vertex, vertexes[vertex]);
//		}
//		return;

		for (int vertex = 0; vertex < (int) vertexes.size(); ++vertex) {
			for (char c = '0'; c <= '1'; ++c) {
				getNextVertex(vertex, c);
			}
			assert(getSuffixLink(vertex) != -1);
			assert(isBad(vertex) != -1);
		}

		for (int vertex = 0; vertex < (int) vertexes.size(); ++vertex) {
			dbg(vertex, vertexes[vertex], isBad(vertex), getSuffixLink(vertex));
		}
	}

	int dfs(int vertex) {
//		dbg(vertex);
		static vector<int> used(vertexes.size(), -1);
		if (used[vertex] != -1) {
			if (used[vertex]) {
				dbg(used);
				dbg(vertex);
			}
			return used[vertex];
		}
		if (vertexes[vertex].isBad) {
			return used[vertex] = 0;
		}
		dbg(vertex);
		used[vertex] = 1;
		used[vertex] =
				dfs(vertexes[vertex].next['0']) ||
				dfs(vertexes[vertex].next['1']);

//		for (char c = '0'; c <= '1'; ++c) {
//			dbgt(vertex, c, vertexes[vertex].next[c]);
//			int ok = dfs(vertexes[vertex].next[c]);
//			dbgtt(vertex, c, vertexes[vertex].next[c], ok);
//			if (ok) {
//				dbg(vertex, c);
//				return 1;
//			} else {
//				used[vertex] = 0;
//			}
//		}
		return used[vertex];
	}
};

string get(const Vertex &vertex) {
	string s;
	const vector<Vertex> &vertexes = vertex.bor.vertexes;
	for (int i = vertex.index; i != 0; i = vertexes[vertexes[i].parent].index) {
		s += vertexes[i].lastChar;
	}
	reverse(s.begin(), s.end());
	return s;
}

int main() {
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);
#endif
	int n;
	cin >> n;
	vector<string> ss(n);
	for (string &s : ss) {
		cin >> s;
	}

	Bor bor;
	for (string &s : ss) {
		bor.insert(s);
	}

	bor.Aho_Corasick();
	bool ok = bor.dfs(0);
	printf("%s\n", ok ? "TAK" : "NIE");
	return 0;
}