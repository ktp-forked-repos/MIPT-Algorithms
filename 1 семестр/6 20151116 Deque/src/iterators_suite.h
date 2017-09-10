#ifndef ITERATORS_SUITE_H_
#define ITERATORS_SUITE_H_

BOOST_AUTO_TEST_SUITE (IteratorsSuite)

BOOST_AUTO_TEST_CASE(IteratorsTest1)
{
	MyDeque<std::pair<float, std::string>> d;
	d.push_back(std::make_pair(31415926535LL, "PI"));

	BOOST_CHECK_EQUAL(d.begin()->first, 31415926535LL);
	BOOST_CHECK_EQUAL(d.begin()->second, "PI");

	d.begin()->first = 1234567890LL;
	d.begin()->second = "simple";

	BOOST_CHECK_EQUAL(d.begin()->first, 1234567890LL);
	BOOST_CHECK_EQUAL(d.begin()->second, "simple");
}

BOOST_AUTO_TEST_CASE(IteratorsTest2)
{
	MyDeque<std::string> d;
	d.push_back("hello");

	BOOST_CHECK_EQUAL(*d.begin(), "hello");
	BOOST_CHECK_EQUAL(d.begin()[0], "hello");
	BOOST_CHECK_EQUAL(d.begin(), d.begin() + 0);
	BOOST_CHECK_EQUAL(d.begin(), d.begin() - 0);
	BOOST_CHECK_EQUAL(d.end(), d.begin() + 1);
	BOOST_CHECK_EQUAL(d.begin(), d.end() - 1);
	BOOST_CHECK_EQUAL(d.begin(), d.begin());
	BOOST_CHECK_EQUAL(d.end(), d.end());
}

BOOST_AUTO_TEST_CASE(IteratorsTest3)
{
	MyDeque<int> d;
	for (size_t i = 0; i < 10; ++i)
	d.push_front(i);

	for (size_t i = 0; i < d.size(); ++i)
	BOOST_CHECK_EQUAL(*(d.begin() + i), d[i]);
}

BOOST_AUTO_TEST_CASE(IteratorsTest4)
{
	MyDeque<int> d;
	for (size_t i = 0; i < 100; ++i)
	if (rand() % 2)
	d.push_back(rand());
	else
	d.push_front(rand());

	for (size_t i = 0; i < d.size(); ++i)
	{
		MyDeque<int>::iterator it = d.begin() + i;
		BOOST_CHECK_EQUAL(d[i], *it);
		BOOST_CHECK_EQUAL(d.begin()[i], *it);

		BOOST_CHECK_EQUAL(d.begin() + i, it);
		BOOST_CHECK_EQUAL(d.begin() - -i, it);
		BOOST_CHECK_EQUAL(d.end() - (d.size() - i), it);
		BOOST_CHECK_EQUAL(d.end() + -static_cast<int>(d.size() - i), it);
	}
}

BOOST_AUTO_TEST_CASE(IteratorsTest5)
{
	MyDeque<int> d;
	for (size_t i = 0; i < 100; ++i)
	if (rand() % 2)
	d.push_back(rand());
	else
	d.push_front(rand());

	for (size_t i = 0; i < d.size(); ++i)
	for (size_t j = 0; j < d.size(); ++j)
	{
		MyDeque<int>::iterator IteratorI = d.begin() + i;
		MyDeque<int>::iterator IteratorJ = d.begin() + j;
		int distance = static_cast<int>(j) - static_cast<int>(i);
		BOOST_CHECK_EQUAL(IteratorJ - IteratorI, distance);
		BOOST_CHECK_EQUAL(IteratorI[distance], *IteratorJ);
		BOOST_CHECK_EQUAL(IteratorI < IteratorJ, i < j);
		BOOST_CHECK_EQUAL(IteratorI > IteratorJ, i > j);
		BOOST_CHECK_EQUAL(IteratorI <= IteratorJ, i <= j);
		BOOST_CHECK_EQUAL(IteratorI >= IteratorJ, i >= j);

		MyDeque<int>::iterator temp;
		temp = IteratorI;
		BOOST_CHECK_EQUAL(temp += distance, IteratorJ);
		temp = IteratorI;
		BOOST_CHECK_EQUAL(temp -= -distance, IteratorJ);
		temp = IteratorI;
		BOOST_CHECK_EQUAL(temp + distance, IteratorJ);
		temp = IteratorI;
		BOOST_CHECK_EQUAL(temp - -distance, IteratorJ);
	}
}

BOOST_AUTO_TEST_SUITE_END()

#endif
