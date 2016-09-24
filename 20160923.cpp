#include <bits/stdc++.h>
using namespace std;

class Semaphore
{
public:
	void wait()
	{
		unique_lock<mutex> lock(m);
		cv.wait(lock, [this]{ return x > 0; });
		--x;
	}
	
	void post()
	{
		lock_guard<mutex> lock(m);
		++x;
		cv.notify_one();
	}
	
private:
	int x;
	mutex m;
	condition_variable cv;
};

int main()
{
    Semaphore s;
    printf("0\n");
    s.post();
    printf("1\n");
    s.post();
    printf("2\n");
    s.wait();
    printf("3\n");
    s.wait();
    printf("4\n");
    return 0;
}
