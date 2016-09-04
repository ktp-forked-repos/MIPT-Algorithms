#include "tests.h"
#include <iostream>

int main() {
	runTests();
	//timeTest(0, 1e3);
	//timeTest(-1e7, 1e7);
	printf("%.2f секунд\n", clock() / (float) CLOCKS_PER_SEC);
	return 0;
}