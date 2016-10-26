#include <bits/stdc++.h>
using namespace std;

template<typename F>
void universal_executor(const F& f) {
	f();
}

void universal_executor_best(function<void()> f) {
	f();
}

void f() {
	cout << "1" << endl;
}

int m(int a, int b) {
	return a - b;
}

int main() {
	universal_executor(f);
    universal_executor([]{ cout << "2" << endl; });
	
	// лучше
    universal_executor_best(f);
    universal_executor_best([]{ cout << "2" << endl; });
    
    // bind
    using namespace placeholders;
    function<int(int)> mf = bind(m, _1, 7);
    cout << mf(5);
    return 0;
}
