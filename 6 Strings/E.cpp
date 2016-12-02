#include <bits/stdc++.h>
using namespace std;

#ifdef LOCAL
#include "/home/dima/C++/debug.h"
#else
void nop() {}
#define dbg(...) nop();
#endif

int main() {
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);
#endif
	int n;
	scanf("%d", &n);
	vector<int> p(n);
	vector<int> z(n, 0);
	for (int i = 0; i < n; ++i) {
		scanf("%d", &p[i]);
	}
	dbg(p);

	vector<int> maxjs(n, -1);
	for (int j = n - 1, mini = n; j >= 0; --j) {
		for (int i = j - p[j] + 1; i < min(mini, j + 1); ++i) {
			dbg(i, j);
			maxjs[i] = j;
		}
		mini = min(mini, j - p[j] + 1);
	}

	for (int i = 0; i < n; ++i) {
//		int maxj = -1;
//		for (int j = i; j < n; ++j) {
//			[j - p[j] + 1, j]
//			if (j - p[j] + 1 <= i && (maxj == -1 || j > maxj)) {
//				maxj = j;
//			}
//		}
//		dbg(i, maxj, maxjs[i]);
		int maxj = maxjs[i];
		if (maxj != -1) {
			if (maxj - p[maxj] + 1 == i) {
				z[i] = p[maxj];
			} else {
				z[i] = min(z[i - (maxj - p[maxj] + 1)], maxj - i + 1);
			}
		}
	}

	z[0] = n;
	for (int i = 0; i < n; ++i) {
		printf("%d ", z[i]);
	}
	printf("\n");
}