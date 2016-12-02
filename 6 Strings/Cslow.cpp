#include <bits/stdc++.h>
using namespace std;
#ifdef LOCAL
#include "/home/dima/C++/debug.h"
#endif
typedef long long ll;
typedef pair<ll, ll> PLL;

const PLL mod = {1000000007, 1000000009};
const PLL mul = {31, 37};

PLL operator%(PLL a, PLL b) {
	return PLL(a.first % b.first, a.second % b.second);
}

PLL operator*(PLL a, PLL b) {
	return PLL(a.first * b.first, a.second * b.second) % mod;
}

PLL operator+(PLL a, PLL b) {
	return PLL(a.first + b.first, a.second + b.second) % mod;
}

PLL operator-(PLL a, PLL b) {
	return (mod + PLL(a.first - b.first, a.second - b.second)) % mod;
}

string s;
vector<PLL> hashes;
vector<PLL> pows;

// [i, j]
PLL getHash(int i, int j) {
	PLL hash = hashes[j] - hashes[i - 1] * pows[j - i + 1];
	return hash;
}

int main() {
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);
#endif
	cin >> s;
	size_t n = s.length();
	s = " " + s;
	hashes.resize(n + 1);
	pows.resize(n + 1);
	hashes[0] = {0, 0};
	pows[0] = {1, 1};
	for (int i = 1; i <= n; ++i) {
		int c = s[i] - 'a';
		hashes[i] = hashes[i - 1] * mul + PLL(c, c);
		pows[i] = pows[i - 1] * mul;
	}

	int m;
	cin >> m;
	for (int i = 0; i < m; ++i) {
		int a, b, c, d;
		cin >> a >> b >> c >> d;
		PLL hash1 = getHash(a, b);
		PLL hash2 = getHash(c, d);
		bool equal = hash1 == hash2 && b - a == d - c;
		cout << (equal ? "Yes" : "No") << endl;
	}
	return 0;
}