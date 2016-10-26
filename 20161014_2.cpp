#include <bits/stdc++.h>
using namespace std;

int main() {
	try {
		try {
			throw runtime_error("1");
		} catch (...) {
			throw_with_nested(runtime_error("2"));
		}
	} catch (exception &e) {
		cout << e.what() << endl;
		try {
			rethrow_if_nested(e);
		} catch (exception &e2) {
			cout << e2.what() << endl;
		}
	}
    return 0;
}
