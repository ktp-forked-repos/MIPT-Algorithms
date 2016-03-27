#ifndef CONS_IITERATORS_SUITE_H_
#define CONS_IITERATORS_SUITE_H_

BOOST_AUTO_TEST_SUITE (ConstIteratorsSuite)

BOOST_AUTO_TEST_CASE(ConstIteratorsTest1)
{
	MyDeque<int> d;
	for (size_t i = 0; i < 1000; ++i)
	if (rand() % 2)
	d.push_back(rand());
	else
	d.push_front(rand());

	MyDeque<int>::const_iterator it = d.cbegin();
	BOOST_CHECK_EQUAL(d[0], *it);

	for (size_t i = 1; i < d.size(); ++i)
	{
		MyDeque<int>::const_iterator temp = it;
		BOOST_CHECK_EQUAL(temp, it);
		BOOST_CHECK_EQUAL(++temp, it + 1);
		temp = it;
		BOOST_CHECK_EQUAL(temp++, it);
	}
}

BOOST_AUTO_TEST_SUITE_END()

#endif
