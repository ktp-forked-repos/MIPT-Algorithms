#include <bits/stdc++.h>

using namespace std;

string rs(int n) {
	string s(n, '0');
	for (char &c : s)
		if (rand() % 3 == 0)
			c = '?';
		else if (rand() % 2 == 0)
			++c;
	return s;
}

int main() {
	int n = 4;
	int m = 3;
	cout << rs(n) << endl;
	cout << rs(m) << endl;
	fclose(stdout);
	return 0;
}
