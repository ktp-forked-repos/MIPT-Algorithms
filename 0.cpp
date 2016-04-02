#include <bits/stdc++.h>
using namespace std;
#define dbg(x) cerr << #x << " = " << (x) << endl;

template<typename T>
class TW;

template<typename T>
void f(T t)
{
	TW<T> tw;
}

typedef vector<int> VI;
int main()
{
	VI a = {1, 2, 3};
	VI& ra = a;
	VI&& rra = move(a);
	VI b(move(rra));
	printf("%d %d %d %d\n", a.size(), ra.size(), rra.size(), b.size());
	return 0;
}
