#ifndef BASIC_SUITE_H_
#define BASIC_SUITE_H_

BOOST_AUTO_TEST_SUITE (BasicSuite)

BOOST_AUTO_TEST_CASE(BasicTest1)
{
	MyDeque<int> d;
	BOOST_ASSERT(d.empty());
	BOOST_CHECK_EQUAL(d.size(), 0u);
}

BOOST_AUTO_TEST_CASE(BasicTest2)
{
	MyDeque<int> d;
	d.push_back(7);
	BOOST_ASSERT(!d.empty());
	BOOST_CHECK_EQUAL(d.size(), 1u);
	BOOST_CHECK_EQUAL(d[0], 7);
	BOOST_CHECK_EQUAL(d[0], d.front());
	BOOST_CHECK_EQUAL(d[0], d.back());
}

BOOST_AUTO_TEST_CASE(BasicTest3)
{
	int *array = new int[10];
	for (size_t i = 0; i < 10; ++i)
	array[i] = 10 + i;

	MyDeque<int> d;
	for (size_t i = 0; i < 5; ++i)
	d.push_back(array[5 + i]);
	for (size_t i = 0; i < 5; ++i)
	d.push_front(array[4 - i]);

	BOOST_ASSERT(!d.empty());
	BOOST_CHECK_EQUAL(d.size(), 10u);
	for (size_t i = 0; i < 10; ++i)
	BOOST_CHECK_EQUAL(d[i], array[i]);
	delete[] array;
}

BOOST_AUTO_TEST_CASE(BasicTest4)
{
	MyDeque<int> d;

	d.push_back(1);
	d.push_front(2);
	BOOST_ASSERT(!d.empty());
	BOOST_CHECK_EQUAL(d.size(), 2u);

	d.pop_back();
	d.pop_front();
	BOOST_ASSERT(d.empty());
	BOOST_CHECK_EQUAL(d.size(), 0u);
}

BOOST_AUTO_TEST_CASE(BasicTest5)
{
	MyDeque<size_t> d;

	for (size_t i = 0; i < 10; ++i)
	d.push_back(i);

	BOOST_CHECK_EQUAL(d.size(), 10u);

	for (size_t i = 0; i < d.size(); ++i)
	BOOST_CHECK_EQUAL(d[i], i);

	for (size_t i = 0; i < d.size(); ++i)
	d[i] += 10;

	for (size_t i = 0; i < d.size(); ++i)
	BOOST_CHECK_EQUAL(d[i], 10 + i);
}

BOOST_AUTO_TEST_CASE(BasicTest6)
{
	MyDeque<int> d;

	for (size_t i = 0; i < 10; ++i)
	d.push_front(static_cast<int>(i));

	int &referenceFront = d.front();
	int &referenceBack = d.back();

	BOOST_CHECK_EQUAL(referenceFront, 9);
	BOOST_CHECK_EQUAL(referenceBack, 0);
	++referenceFront;
	referenceBack += 7;
	BOOST_CHECK_EQUAL(d.front(), 10);
	BOOST_CHECK_EQUAL(d.back(), 7);
}

BOOST_AUTO_TEST_CASE(BasicTest7)
{
	MyDeque<int> d1, d2, d3;
	d1.push_front(5);
	d1.push_back(7);
	d3 = d2 = d1;
	BOOST_CHECK_EQUAL(d2.size(), 2u);
	BOOST_CHECK_EQUAL(d3.size(), 2u);
	BOOST_CHECK_EQUAL(d2.front(), 5);
	BOOST_CHECK_EQUAL(d2.back(), 7);
	BOOST_CHECK_EQUAL(d3.front(), 5);
	BOOST_CHECK_EQUAL(d3.back(), 7);
}

BOOST_AUTO_TEST_SUITE_END()

#endif
