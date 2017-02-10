#include <bits/stdc++.h>

using namespace std;

int main() {
	freopen("input.txt", "w", stdout);
	int n = 300;
	cout << n << endl;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			cout << rand() % 1000000 << " ";
		}
		cout << endl;
	}
	return 0;
}