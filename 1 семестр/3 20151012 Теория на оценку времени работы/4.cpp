#define INPUT 1
#define OUTPUT 0
#define DEBUG 1
#define FILE_NAME ""

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<int, int> PII;
typedef pair<ll, ll> PLL;
typedef vector<int> VI;
typedef vector<ll> VL;
#define dbg(x) if (debug) cerr << #x << " = " << (x) << endl;

const int MOD = 1e9 + 7;
const int INF = 2e9 + 7;
const ll INFL = 8e18L + 7;
const double INFD = 1e300;

bool debug = 0;

void run();

int main()
{
	#ifdef LOCAL
		if (INPUT)
			freopen("input.txt", "r", stdin);
		if (OUTPUT)
			freopen("output.txt", "w", stdout);
		debug = DEBUG;
	#else
		if (strlen(FILE_NAME) > 0)
		{
			freopen(FILE_NAME".in", "r", stdin);
			freopen(FILE_NAME".out", "w", stdout);
		}
	#endif
	
	run();
	
	#ifdef LOCAL
		fprintf(stderr, "%.3f seconds \n", clock() / (double) CLOCKS_PER_SEC);
	#endif
	return 0;
}

const int N = 1e6 + 7;

int get(int *a, int n)
{
	if (n == 1)
		return a[0];
	assert(n % 5 == 0);
	for (int i = 0; i < n; i += 5)
		sort(a + i, a + i + 5);
	
	int *b = new int[n / 5];
	for (int i = 0; i < n; i += 5)
		b[i / 5] = a[i + 2];
	delete[] b;
	
	int m = get(b, n / 5);
	
	int *c = new int[n];
	int k = 0;
	for (int i = 0; i < n; i += 5)
	{
		c[k++] = a[i + 2];
		c[k++] = a[i + 3];
		c[k++] = a[i + 4];
		if (a[i + 2] > m)
		{
			c[k++] = a[i + 0];
			c[k++] = a[i + 1];
		}
	}
	
	dbg(k)
	return 0;
	int ret = get(c, k);
	delete[] c;
	return ret;
}

int a[N];

void run()
{
	int n;
	cin >> n;
	for (int i = 0; i < n; i++)
		cin >> a[i];
	cout << get(a, n) << endl;
	// не получилось из-за того, что размер массива не обязательно кратен 5s
}
/*
if (debug) printf("\n");
*/