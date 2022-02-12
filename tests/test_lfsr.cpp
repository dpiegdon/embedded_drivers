
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/included/unit_test.hpp>

#include "../lfsr.h"
#include <cstdio>


template <class T>
void test_lfsr()
{
	const unsigned long long expected_period = (1ULL << T::cWidth) - 1;
	T lfsr;
	typename T::BaseType first = lfsr.GetRegisterState();
	typename T::BaseType next;
	typename T::BaseType out;
	unsigned long long period;

	printf("%u bits:\n", T::cWidth);
	printf("  [ 0x%llx", static_cast<unsigned long long>(first));
	for (period = 1; period <= expected_period; ++period) {
		out = lfsr.Iterate();
		next = lfsr.GetRegisterState();
		if (period < 5)
			printf("\n    0x%llx out: 0x%llx",
					static_cast<unsigned long long>(next),
					static_cast<unsigned long long>(out));
		if (first == next)
			break;
	}
	printf(" ]\n");
	printf("  Period should be: %llu, is: %llu.\n"
		"  First: 0x%llx, last: 0x%llx.\n\n",
			expected_period,
			period,
			static_cast<unsigned long long>(first),
			static_cast<unsigned long long>(next));

	BOOST_REQUIRE(period == expected_period);
	BOOST_REQUIRE(first == next);
}



BOOST_AUTO_TEST_CASE(lfsr)
{
	test_lfsr<embedded_drivers::LfsrDefault4>();
	test_lfsr<embedded_drivers::LfsrDefault8>();
	test_lfsr<embedded_drivers::LfsrDefault9>();
	test_lfsr<embedded_drivers::LfsrDefault10>();
	test_lfsr<embedded_drivers::LfsrDefault15>();
	test_lfsr<embedded_drivers::LfsrFibonacci>();
	test_lfsr<embedded_drivers::LfsrDefault16>();
	test_lfsr<embedded_drivers::LfsrDefault17>();
	test_lfsr<embedded_drivers::LfsrDefault18>();
	test_lfsr<embedded_drivers::LfsrDefault24>();
	test_lfsr<embedded_drivers::LfsrDefault32>();
}
