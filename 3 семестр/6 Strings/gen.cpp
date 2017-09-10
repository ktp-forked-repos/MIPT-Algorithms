#ifdef LOCAL
#include "/home/dima/C++/debug.h"
#else
void nop();
#define dbg(...) nop
#define dbgl(...) nop
#endif
#include <bits/stdc++.h>
#include <stdlib.h>
using namespace std;

typedef long long ll;
typedef vector<int> VI;
typedef vector<ll> VL;
typedef vector<bool> VB;
typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef pair<ll, ll> PLL;
typedef vector<PLL> VPLL;
typedef vector<VI> VVI;
typedef set<int> SI;
typedef set<ll> SL;
typedef multiset<int> MSI;
typedef multiset<ll> MSL;
typedef vector<string> VS;
const int inf = (int) 1e9;
const ll infl = (ll) 2e18;
const int mod = 1000000007;

int main() {
	freopen("input.txt", "w", stdout);
	int n = 10000;
	string s(n, '_');
	for (char &c : s) {
		c = 'a' + (rand() % 26);
	}
//	cout << s << endl;
	cout << string(n, 'a') << endl;
	return 0;
}
