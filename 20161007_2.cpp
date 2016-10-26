#include <bits/stdc++.h>
using namespace std;

int f() {
	// ...
	throw 1;
	return 42;
}

int main() {
	future<int> result = async(f);
	cout << result.get() << endl;
	// или
	
	promise<int> prom;
	
	try {
		prom.set_value(f());
	} catch (...) {
		prom.set_exception(current_exception());
	}
	
	future<int> promise_future = prom.get_future();
	cout << promise_future.get() << endl;
	return 0;
}
