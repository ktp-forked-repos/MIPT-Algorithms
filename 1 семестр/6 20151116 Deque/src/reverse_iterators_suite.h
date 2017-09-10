#ifndef REVERSE_ITERATORS_SUITE_H_
#define REVERSE_ITERATORS_SUITE_H_

BOOST_AUTO_TEST_SUITE (ReverseIteratorsSuite)

BOOST_AUTO_TEST_CASE(ReverseIteratorsTest1)
{
	MyDeque<int> d;
	for (size_t i = 0; i < 1000; ++i)
	d.push_front(rand());

	std::sort(d.rbegin(), d.rend());
	for (size_t i = 1; i < d.size(); ++i)
	BOOST_CHECK_GE(d[i - 1], d[i]);
	std::reverse(d.rbegin(), d.rend());
	for (size_t i = 1; i < d.size(); ++i)
	BOOST_CHECK_LE(d[i - 1], d[i]);
}

BOOST_AUTO_TEST_CASE(ReverseIteratorsTest2)
{
	MyDeque<int> d;
	for (size_t i = 0; i <= 7; ++i)
	d.push_front(i * 10 + 7);

	BOOST_CHECK_EQUAL(*d.rbegin(), 7);
	BOOST_CHECK_EQUAL(*--d.rend(), 77);
	*d.rbegin() += 70;
	d.rend()[-1] -= 70;
	BOOST_CHECK_EQUAL(*d.rbegin(), 77);
	BOOST_CHECK_EQUAL(*--d.rend(), 7);
}

BOOST_AUTO_TEST_CASE(ReverseIteratorsTest3)
{
	MyDeque<std::pair<int, int>> d;
	d.push_front(std::make_pair(2015, 1116));

	BOOST_CHECK_EQUAL(d.rbegin()->first, 2015);
	BOOST_CHECK_EQUAL(d.rbegin()->second, 1116);
	d.rbegin()->first++;
	d.rbegin()->second -= 5;
	BOOST_CHECK_EQUAL(d.rbegin()->first, 2016);
	BOOST_CHECK_EQUAL(d.rbegin()->second, 1111);
}

BOOST_AUTO_TEST_SUITE_END()

#endif
