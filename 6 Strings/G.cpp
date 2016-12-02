#include <bits/stdc++.h>
#include <stdlib.h>
using namespace std;

#ifdef LOCAL
#include "/home/dima/C++/debug.h"
#else
void nop() {}
#define dbg(...) nop();
#endif

struct Bor {
	vector<array<int, 128>> vertexes;
	vector<int> subtreeSize;
	vector<int> numberEndsIn;

	Bor() {
		newVertex();
	}

	int newVertex() {
		array<int, 128> v0;
		v0.fill(-1);
		int i = vertexes.size();
		vertexes.push_back(v0);
		subtreeSize.push_back(0);
		numberEndsIn.push_back(0);
		return i;
	}

	void insert(string s) {
		int vertex = 0;
		for (int i = 0; i < s.length(); ++i) {
			++subtreeSize[vertex];
			int nextVertex = vertexes[vertex][s[i]];
			if (nextVertex == -1) {
				nextVertex = newVertex();
				vertexes[vertex][s[i]] = nextVertex;
			}
			vertex = vertexes[vertex][s[i]];
		}
		++subtreeSize[vertex];
		++numberEndsIn[vertex];
	}

	string kth(int k) {
		string s;
		for (int vertex = 0; k >= numberEndsIn[vertex];) {
			assert(0 <= k && k < subtreeSize[vertex]);
			k -= numberEndsIn[vertex];
			bool change = false;
			for (char c = 'a'; c <= 'z' && !change; ++c) {
				int nextVertex = vertexes[vertex][c];
				if (nextVertex != -1) {
					int size = subtreeSize[nextVertex];
					if (k >= size) {
						k -= size;
					} else {
						vertex = nextVertex;
						s += c;
						change = true;
					}
				}
			}
			assert(change);
		}
		return s;
	}
};

int main() {
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);
#endif
	int n;
	cin >> n;
//	set<string> strings;
	Bor bor;
	for (int i = 0; i < n; ++i) {
		string line;
		cin >> line;
		if ('0' <= line[0] && line[0] <= '9') {
			int k = atoi(line.c_str()) - 1;
//			auto it = strings.begin();
//			advance(it, k);
//			cout << *it << endl;
			cout << bor.kth(k) << endl;
		} else {
//			strings.insert(line);
			bor.insert(line);
		}
	}
	return 0;
}