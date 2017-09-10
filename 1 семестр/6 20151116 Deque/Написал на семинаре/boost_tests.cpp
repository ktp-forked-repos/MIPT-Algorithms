#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <bits/stdc++.h>

using namespace std;
#define dbg(x) std::cerr << #x << " = " << (x) << std::endl;

BOOST_AUTO_TEST_SUITE(MySuite)

BOOST_AUTO_TEST_CASE(MyTest)
{
	BOOST_ASSERT(false);
	BOOST_CHECK_EQUAL(5, 6);
	//int* a = new int[10]; // он нам скажет, что всё плохо
}

BOOST_AUTO_TEST_SUITE_END()