#include <disjoint_set.h>

#include <boost/test/unit_test.hpp>

boost::test_tools::predicate_result unite_with_equal_ranks(figures_counter::disjoint_set& sets, size_t s1, size_t s2) {
	const size_t parent1 = sets.find(s1);
	const size_t parent2 = sets.find(s2);
	const size_t union_parent = sets.unite(s1, s2);

	// Equal rank - no matter which is the parent, just expect successful operation
	boost::test_tools::predicate_result check_result = (union_parent == parent1 || union_parent == parent2);
	if (!check_result) {
		check_result.message() << "parent of union is " << union_parent << ", expected " << parent1 << " or "
							   << parent2;
	}
	return check_result;
}

boost::test_tools::predicate_result unite_expect_parent(figures_counter::disjoint_set& sets, size_t s1, size_t s2,
                                                        size_t expected_parent) {
	const size_t union_parent = sets.unite(s1, s2);

	// Equal rank - no matter which is the parent, just expect successful operation
	boost::test_tools::predicate_result check_result = (union_parent == expected_parent);
	if (!check_result) {
		check_result.message() << "parent of union is " << union_parent << ", expected " << expected_parent;
	}
	return check_result;
}

BOOST_AUTO_TEST_SUITE(DisjointSet)

BOOST_AUTO_TEST_CASE(DefaultCreate) {
	BOOST_REQUIRE_NO_THROW(figures_counter::disjoint_set());
}

BOOST_AUTO_TEST_CASE(AddToUnion) {
	figures_counter::disjoint_set sets;
	BOOST_REQUIRE_EQUAL(sets.add_new(), 0z);
	BOOST_REQUIRE_EQUAL(sets.add_new(), 1z);
	BOOST_REQUIRE_EQUAL(sets.add_new(), 2z);
	BOOST_REQUIRE_EQUAL(sets.add_new(), 3z);
	BOOST_REQUIRE_EQUAL(sets.add_new(), 4z);
}

BOOST_AUTO_TEST_CASE(CountUnique) {
	figures_counter::disjoint_set sets;
	BOOST_CHECK_EQUAL(sets.add_new(), 0z);
	BOOST_CHECK_EQUAL(sets.add_new(), 1z);
	BOOST_CHECK_EQUAL(sets.add_new(), 2z);
	BOOST_CHECK_EQUAL(sets.add_new(), 3z);
	BOOST_CHECK_EQUAL(sets.add_new(), 4z);

	BOOST_REQUIRE_EQUAL(sets.count_unique(), 5);
}

BOOST_AUTO_TEST_CASE(Unite) {
	figures_counter::disjoint_set sets;

	BOOST_CHECK_EQUAL(sets.add_new(), 0z);
	BOOST_CHECK_EQUAL(sets.add_new(), 1z);
	BOOST_CHECK_EQUAL(sets.add_new(), 2z);
	BOOST_CHECK_EQUAL(sets.add_new(), 3z);
	BOOST_CHECK_EQUAL(sets.add_new(), 4z);

	// Equivalent sets are {0, 1, 4}, {2, 3}
	BOOST_REQUIRE(unite_with_equal_ranks(sets, 1z, 4z));
	BOOST_REQUIRE(unite_expect_parent(sets, 0z, 4z, sets.find(4z))); // 4z has higher rank than 0z
	BOOST_REQUIRE(unite_with_equal_ranks(sets, 2z, 3z));
}

BOOST_AUTO_TEST_CASE(Find) {
	figures_counter::disjoint_set sets;
	BOOST_CHECK_EQUAL(sets.add_new(), 0z);
	BOOST_CHECK_EQUAL(sets.add_new(), 1z);
	BOOST_CHECK_EQUAL(sets.add_new(), 2z);
	BOOST_CHECK_EQUAL(sets.add_new(), 3z);
	BOOST_CHECK_EQUAL(sets.add_new(), 4z);
	BOOST_CHECK_EQUAL(sets.add_new(), 5z);
	BOOST_CHECK_EQUAL(sets.add_new(), 6z);

	// Equivalent sets are {0, 1, 4}, {2, 3, 6}, {5}
	BOOST_CHECK(unite_with_equal_ranks(sets, 1z, 4z));
	BOOST_CHECK(unite_expect_parent(sets, 0z, 4z, sets.find(4z))); // 4z has higher rank than 0z
	BOOST_CHECK(unite_with_equal_ranks(sets, 2z, 6z));
	BOOST_CHECK(unite_expect_parent(sets, 6z, 3z, sets.find(6z))); // 6z has higher rank than 3z

	// Check groups are as expected
	const size_t group_1 = sets.find(0z);
	const size_t group_2 = sets.find(2z);
	const size_t group_3 = sets.find(5z);
	BOOST_REQUIRE_EQUAL(sets.find(0z), group_1);
	BOOST_REQUIRE_EQUAL(sets.find(1z), group_1);
	BOOST_REQUIRE_EQUAL(sets.find(2z), group_2);
	BOOST_REQUIRE_EQUAL(sets.find(3z), group_2);
	BOOST_REQUIRE_EQUAL(sets.find(4z), group_1);
	BOOST_REQUIRE_EQUAL(sets.find(5z), group_3);
	BOOST_REQUIRE_EQUAL(sets.find(6z), group_2);
}

BOOST_AUTO_TEST_SUITE_END()