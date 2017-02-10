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

int main() {
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);
#endif
	int n;
	string s;
	cin >> n >> s;
	vector<int> indexToSuffix(n);
	for (int &suffix : indexToSuffix) {
		cin >> suffix;
		--suffix;
	}

	vector<int> suffixToIndex(n);
	for (int index = 0; index < n; ++index) {
		suffixToIndex[indexToSuffix[index]] = index;
	}

	// lcp[index] == LCP(
	//      s[indexToSuffix[index - 1], |s|),
	//      s[indexToSuffix[index], |s|)
	// )
	vector<int> lcp(n);
	lcp[0] = -1;
	for (int suffix = 0; suffix < n; ++suffix) {
		int index = suffixToIndex[suffix];
		if (index != 0) {
			lcp[index] = suffix == 0 ? 0 : max(0, lcp[suffixToIndex[suffix - 1]] - 1);
			dbg(suffix, index, s.substr(suffix), lcp[index]);
			while (suffix + lcp[index] < n
			       && indexToSuffix[index - 1] + lcp[index] < n
			       && s[suffix + lcp[index]] == s[indexToSuffix[index - 1] + lcp[index]]) {
				dbgt(s[suffix + lcp[index]], s[indexToSuffix[index - 1] + lcp[index]]);
				++lcp[index];
			}
			dbg(suffix, index, s.substr(suffix), lcp[index]);
		}
	}

	for (int i = 1; i < n; ++i) {
		cout << lcp[i] << " ";
	}
	cout << endl;
	return 0;
}