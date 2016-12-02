#include <bits/stdc++.h>
using namespace std;
#ifdef LOCAL
#include "/home/dima/C++/debug.h"
#endif
typedef long long ll;

const ll mod = 1000000007;
const ll mul = 31;

char s[100001];
//string s;
vector<ll> hashes;
vector<ll> pows;

// [i, j]
ll getHash(int i, int j) {
	return (mod + hashes[j] - hashes[i - 1] * pows[j - i + 1] % mod) % mod;
}

int main() {
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);
#endif
//	cin >> s;
	scanf("%s", s);
//	int n = s.length();
	int n = strlen(s);
	hashes.resize(n + 1);
	pows.resize(n + 1);
	hashes[0] = 0;
	pows[0] = 1;
	for (int i = 1; i <= n; ++i) {
		int c = s[i - 1] - 'a';
		hashes[i] = (hashes[i - 1] * mul + c) % mod;
		pows[i] = pows[i - 1] * mul % mod;
	}

	int m;
	cin >> m;
	for (int i = 0; i < m; ++i) {
		int a, b, c, d;
//		cin >> a >> b >> c >> d;
		scanf("%d%d%d%d", &a, &b, &c, &d);
		ll hash1 = getHash(a, b);
		ll hash2 = getHash(c, d);
		bool equal = hash1 == hash2 && b - a == d - c;
//		cout << (equal ? "Yes" : "No") << endl;
		printf(equal ? "Yes\n" : "No\n");
	}
	return 0;
}