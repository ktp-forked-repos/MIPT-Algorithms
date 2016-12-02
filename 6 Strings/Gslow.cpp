#include <bits/stdc++.h>
using namespace std;

//#include "/home/dima/C++/debug.h"
int main() {
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);
#endif
	int n;
	cin >> n;
	set<string> strings;
	for (int i = 0; i < n; ++i) {
		string line;
		cin >> line;
		if ('0' <= line[0] && line[0] <= '9') {
			int k = atoi(line.c_str()) - 1;
			auto it = strings.begin();
			advance(it, k);
			cout << *it << endl;
		} else {
			strings.insert(line);
		}
	}
	return 0;
}