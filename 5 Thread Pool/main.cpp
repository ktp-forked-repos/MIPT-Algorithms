#include <iostream>
#include <vector>
#include <future>
using namespace std;

int main() {
	future<int> a;
	auto lambda = [a = std::move(a)] { return 7; };
	lambda();
}