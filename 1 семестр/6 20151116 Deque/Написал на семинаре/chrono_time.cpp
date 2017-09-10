#include <bits/stdc++.h>
using namespace std;

#define dbg(x) std::cerr << #x << " = " << (x) << std::endl;

void simple_declaration()
{
	auto time = chrono::system_clock::now();
	//auto = std::chrono::time_point<chrono::system_clock>
	chrono::steady_clock::now();			// не зависят от текущего времени системы
	chrono::high_resolution_clock::now();
	cout << sizeof(time) << endl;
}

void measure_interval()
{
	auto s = chrono::high_resolution_clock::now();
	
	vector<int> a;
	a.resize(20000000);
	for_each(a.begin(), a.end(), [](int& x) { x++; });
	
	auto e = chrono::high_resolution_clock::now();
	chrono::high_resolution_clock::duration d = e - s;
	//auto d = e - s;
	cout << chrono::duration_cast<chrono::microseconds>(d).count() << endl;
	//auto = std::chrono::time_duration<chrono::system_clock>
}

int main()
{
	measure_interval();
	return 0;
}