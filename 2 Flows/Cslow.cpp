#include <bits/stdc++.h>

using namespace std;

int getk(string s) {
	int k = 1;
	for (char c : s) {
		if (c == '?') {
			k *= 2;
		}
	}
	return k;
}

string gets(string s, int mask) {
	for (char &c : s) {
		if (c == '?') {
			c = '0' + mask % 2;
			mask /= 2;
		}
	}
	return s;
}

pair<int, pair<string, string>> get_ans(string s, string t) {
	int ans = 0;
	for (int j = 0; j < (int) t.length(); ++j) {
		for (int i = 0; i + t.length() <= s.length(); ++i) {
			if (t[j] != s[i + j]) {
				++ans;
			}
		}
	}
	return {ans, {s, t}};
}

int main() {
	freopen("input.txt", "r", stdin);
	string s, t;
	cin >> s >> t;
	int ks = getk(s);
	int kt = getk(t);
	pair<int, pair<string, string>> ans = {INT_MAX, {s, t}};
	for (int smask = 0; smask < ks; ++smask) {
		for (int tmask = 0; tmask < kt; ++tmask) {
			ans = min(ans, get_ans(gets(s, smask), gets(t, tmask)));
		}
	}
	cout << ans.first << endl;
	cout << ans.second.first << endl;
	cout << ans.second.second << endl;
	return 0;
}
