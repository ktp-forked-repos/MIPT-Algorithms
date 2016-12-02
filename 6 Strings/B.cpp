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

const int N = 1000010;
char *s = new char[N];
char ans[N];
int z[N];

void zfunc(int n) {
	z[0] = n;

	int maxi = -1;
	for (int i = 1; i < n; ++i) {
		int max_i_plus_zi = maxi + z[maxi];
		z[i] = (maxi != -1 && i < max_i_plus_zi) ? min(z[i - maxi], max_i_plus_zi - i) : 0;
		while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
			++z[i];
		}
		if (maxi == -1 || maxi + z[maxi] < i + z[i]) {
			maxi = i;
		}
	}
}

int main() {
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);
#endif
	scanf("%s", s);
//	string s(cs);
//	string s;
//	cin >> s;
	if (s[0] == '1') {
		printf("1.(0)\n");
		return 0;
	}
//	s = s.substr(2);
	s += 2;
//	reverse(s.begin(), s.end());
//	int n = s.length();
	int n = strlen(s);
	reverse(s, s + n);
//	vector<int> z(n);
	zfunc(n);
//	dbg(s);
//	dbg(z);
//	pair<int, int> minsum = {n, n};
	int minsumFirst = n;
	int minsumSecond = n;
	for (int period = 1; period < n; ++period) {
		int totalPeriodLength = period + z[period];
//		pair<int, int> currsum = {period + n - totalPeriodLength, period};
//		dbg(period, z[period], totalPeriodLength, currsum);
		if (period + n - totalPeriodLength < minsumFirst) {
			minsumFirst = period + n - totalPeriodLength;
			minsumSecond = period;
		}
//		minsum = min(minsum, currsum);
	}

//	dbg(minsum);
	int period = minsumSecond;
	int totalPeriodLength = period + z[period];
//	string ans = ")" + s.substr(totalPeriodLength - period, period) + "(" + s.substr(totalPeriodLength) + ".0";
//	reverse(ans.begin(), ans.end());
//	cout << ans << endl;
//	printf("%s\n", ans.c_str());

	int ansn = 0;
	ans[ansn++] = '0';
	ans[ansn++] = '.';
//	printf("0.");
	for (int i = n - 1; i >= totalPeriodLength; --i) {
		ans[ansn++] = s[i];
//		printf("%c", s[i]);
	}
	ans[ansn++] = '(';
//	printf("(");
	for (int i = totalPeriodLength - 1; i >= totalPeriodLength - period; --i) {
		ans[ansn++] = s[i];
//		printf("%c", s[i]);
	}
	ans[ansn++] = ')';
	ans[ansn++] = 0;
//	printf(")\n");

	printf("%s\n", ans);
	return 0;
}