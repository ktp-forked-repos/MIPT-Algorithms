#include <bits/stdc++.h>
#include <stdlib.h>
using namespace std;

#ifdef LOCAL
#include "/home/dima/C++/debug.h"
#else
void nop() {}
#define msg(...) nop();
#define print_clock(...) nop();
#define dbg(...) nop();
#define dbgt(...) nop();
#endif

/*vector<int> zfunc(string s) {
	int n = s.length();
	vector<int> z(n);
	z[0] = n;

	int maxi = -1;
	for (int i = 1; i < n; ++i) {
		z[i] = (maxi != -1 && i < maxi + z[maxi]) ? min(z[i - maxi], maxi + z[maxi] - i) : 0;
		while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
			++z[i];
		}
		if (maxi == -1 || maxi + z[maxi] < i + z[i]) {
			maxi = i;
		}
	}
	return z;
}*/

string replaceAll(string source, string find, string replace) {
	for (string::size_type i = 0; (i = source.find(find, i)) != string::npos;) {
		source.replace(i, find.length(), replace);
		i += replace.length();
	}
	return source;
}

const int N = 10010;
int previ[N];
int nexti[N];
int previous[N * N / 2];
int z[N];
//int16_t previous[N * N / 2];

int find(int left, int right, int len) {
	for (int i = right; i >= left; --i) {
		if (z[i] >= len) {
			return i;
		}
	}
	return -1;
}

void zfunc(const char *s0, int i0, int n) {
	const char *s = s0 + i0;
	z[0] = n;

	int maxi = -1;
	for (int i = 1; i < n; ++i) {
		z[i] = (maxi != -1 && i < maxi + z[maxi]) ? min(z[i - maxi], maxi + z[maxi] - i) : 0;
		while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
			++z[i];
		}
		if (maxi == -1 || maxi + z[maxi] < i + z[i]) {
			maxi = i;
		}
	}
}

void zfunc2(const char *s0, int i0, int n) {
	const char *s = s0 + i0;
	z[0] = n;
	for (int i = 1, l = 0, r = 0; i < n; ++i) {
		int zi = i <= r ? min(r - i + 1, z[i - l]) : 0;
		while (i + zi < n && s[zi] == s[i + zi])
			++zi;
		if (i + zi - 1 > r)
			l = i, r = i + zi - 1;
		z[i] = zi;
	}
}

int main() {
#ifdef LOCAL
	freopen("input.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);
#endif
	string s;
	cin >> s;
//	s = string(10000, 'a');
	int n = s.length();

	string ss0 = s + "#" + s;
	const char *ss = ss0.c_str();
	// previous[i][len]
//	int k = 0;
//	vector<vector<int>> previous(n);
	print_clock();
	for (int i = 0; i < n; ++i) {
//		previous[i].resize(n - i + 1);
//		assert(k == ((n + 1) + (n - i + 2)) * i / 2);
//		k += n - i + 1;
		int k = ((n + 1) + (n - i + 2)) * i / 2;
//		vector<int> z = zfunc(s.substr(i) + "#" + s);
		zfunc2(ss, i, n * 2 + 1 - i);
		int i0 = (n - i) + 1;
		int last = find(i0, i0 + i - 1, 1);
//		for (int len = 1; len <= n - i; ++len) {
		for (int len = 1; len <= n - i; ++len) {
//			int last2 = -1;
//			for (int j = 0; j <= i - len; ++j) {
//				if (z[i0 + j] >= len) {
//					last2 = j;
//				}
//			}
//			assert(last2 == (last == -1 ? -1 : last - i0));
//			previous[i][len] = last == -1 ? -1 : last - i0;
			previous[k + len] = last == -1 ? -1 : last - i0;
//			previous[i].push_back(last2);
			last = last == -1 ? -1 : find(i0, min(last, i0 + i - len - 1), len + 1);
		}
	}
//	dbg(previous);
	print_clock();

	// {minsum, i, len}
//	vector<int> ans = {INT_MAX};
	int minsum = INT_MAX;
	pair<int, int> ans = {-1, -1};
	for (int len = 1; len <= n; ++len) {
		int numberSubstrings = n - len + 1;
//		vector<int> prev(numberSubstrings);
//		vector<int> next(numberSubstrings, -1);
		for (int i = 0; i < numberSubstrings; ++i) {
			nexti[i] = -1;
		}
		for (int i = 0; i < numberSubstrings; ++i) {
//			previ[i] = previous[i][len];
			previ[i] = previous[((n + 1) + (n - i + 2)) * i / 2 + len];
			if (previ[i] != -1) {
				nexti[previ[i]] = i;
			}
		}
//		msg("");
//		dbg(len);
//		dbg(previ);
//		dbg(nexti);

		for (int i = 0; i < numberSubstrings; ++i) {
			if (previ[i] == -1) {
				int k = 0;
				for (int j = i; j != -1; j = nexti[j]) {
					++k;
				}
//				dbg(i, len, k, len + (n - (len - 1) * k));
//				ans = min(ans, {len + (n - (len - 1) * k), i, len});
				int csum = len + (n - (len - 1) * k);
				if (csum < minsum) {
					minsum = csum;
					ans = {i, len};
				}
			}
		}
	}
	print_clock();

	int i = ans.first;
	int len = ans.second;
	string t = s.substr(i, len);
	if (t.length() == 1) {
		cout << endl;
		cout << s << endl;
	} else {
		assert(!t.empty());
		cout << t << endl;
		cout << replaceAll(s, t, "A") << endl;
	}
	print_clock();
	return 0;
}