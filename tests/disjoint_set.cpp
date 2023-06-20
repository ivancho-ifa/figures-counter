#include <disjoint_set.h>

#include <boost/test/unit_test.hpp>

constexpr size_t const_0(0);
constexpr size_t const_1(1);
constexpr size_t const_2(2);
constexpr size_t const_3(3);
constexpr size_t const_4(4);
constexpr size_t const_5(5);
constexpr size_t const_6(6);

boost::test_tools::predicate_result unite_with_equal_ranks(figures_counter::disjoint_set& sets, size_t s1, size_t s2) {
	const size_t parent1 = sets.find_parent(s1);
	const size_t parent2 = sets.find_parent(s2);
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
	BOOST_REQUIRE_EQUAL(sets.add_new(), const_0);
	BOOST_REQUIRE_EQUAL(sets.add_new(), const_1);
	BOOST_REQUIRE_EQUAL(sets.add_new(), const_2);
	BOOST_REQUIRE_EQUAL(sets.add_new(), const_3);
	BOOST_REQUIRE_EQUAL(sets.add_new(), const_4);
}

BOOST_AUTO_TEST_CASE(CountUnique) {
	figures_counter::disjoint_set sets;
	BOOST_CHECK_EQUAL(sets.add_new(), const_0);
	BOOST_CHECK_EQUAL(sets.add_new(), const_1);
	BOOST_CHECK_EQUAL(sets.add_new(), const_2);
	BOOST_CHECK_EQUAL(sets.add_new(), const_3);
	BOOST_CHECK_EQUAL(sets.add_new(), const_4);

	BOOST_REQUIRE_EQUAL(sets.count_unique(), 5);
}

BOOST_AUTO_TEST_CASE(Unite) {
	figures_counter::disjoint_set sets;

	BOOST_CHECK_EQUAL(sets.add_new(), const_0);
	BOOST_CHECK_EQUAL(sets.add_new(), const_1);
	BOOST_CHECK_EQUAL(sets.add_new(), const_2);
	BOOST_CHECK_EQUAL(sets.add_new(), const_3);
	BOOST_CHECK_EQUAL(sets.add_new(), const_4);

	// Equivalent sets are {0, 1, 4}, {2, 3}
	BOOST_REQUIRE(unite_with_equal_ranks(sets, const_1, const_4));
	BOOST_REQUIRE(
		unite_expect_parent(sets, const_0, const_4, sets.find_parent(const_4))); // const_4 has higher rank than const_0
	BOOST_REQUIRE(unite_with_equal_ranks(sets, const_2, const_3));
}

BOOST_AUTO_TEST_CASE(Find) {
	figures_counter::disjoint_set sets;
	BOOST_CHECK_EQUAL(sets.add_new(), const_0);
	BOOST_CHECK_EQUAL(sets.add_new(), const_1);
	BOOST_CHECK_EQUAL(sets.add_new(), const_2);
	BOOST_CHECK_EQUAL(sets.add_new(), const_3);
	BOOST_CHECK_EQUAL(sets.add_new(), const_4);
	BOOST_CHECK_EQUAL(sets.add_new(), const_5);
	BOOST_CHECK_EQUAL(sets.add_new(), const_6);

	// Equivalent sets are {0, 1, 4}, {2, 3, 6}, {5}
	BOOST_CHECK(unite_with_equal_ranks(sets, const_1, const_4));
	BOOST_CHECK(
		unite_expect_parent(sets, const_0, const_4, sets.find_parent(const_4))); // const_4 has higher rank than const_0
	BOOST_CHECK(unite_with_equal_ranks(sets, const_2, const_6));
	BOOST_CHECK(
		unite_expect_parent(sets, const_6, const_3, sets.find_parent(const_6))); // 6z has higher rank than const_3

	// Check groups are as expected
	const size_t group_1 = sets.find_parent(const_0);
	const size_t group_2 = sets.find_parent(const_2);
	const size_t group_3 = sets.find_parent(const_5);
	BOOST_REQUIRE_EQUAL(sets.find_parent(const_0), group_1);
	BOOST_REQUIRE_EQUAL(sets.find_parent(const_1), group_1);
	BOOST_REQUIRE_EQUAL(sets.find_parent(const_2), group_2);
	BOOST_REQUIRE_EQUAL(sets.find_parent(const_3), group_2);
	BOOST_REQUIRE_EQUAL(sets.find_parent(const_4), group_1);
	BOOST_REQUIRE_EQUAL(sets.find_parent(const_5), group_3);
	BOOST_REQUIRE_EQUAL(sets.find_parent(const_6), group_2);
}

BOOST_AUTO_TEST_SUITE_END()