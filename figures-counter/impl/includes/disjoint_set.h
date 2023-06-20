#pragma once

#include <unordered_map>

namespace figures_counter {

/**
 * @brief Disjoint set implementation designed specifically to keep figure IDs.
 *
 * Efficiently handles keeping records for equivalent values. Uses union ranking and path compression to keep paths
 * optimal. Attempts to use minimal memory by keeping IDs in @p std::vector and counting number of sets is constant.
 */

class disjoint_set {
public:
	/**
	 * @return The number of unique IDs.
	 */
	size_t count_unique() const noexcept;

	/**
	 * @brief Finds the parent of element @p child
	 * @param child The child whose parent is requested
	 * @return The @p child parent
	 */
	size_t find_parent(size_t child);

	/**
	 * @brief Unites the sets which contain @p a and @p b
	 * @return The parent of the resulting united set
	 */
	size_t unite(size_t a, size_t b);

	/**
	 * @brief Add the next available ID to a separate set
	 * @return The new ID
	*/
	size_t add_new();

private:
	std::vector<size_t> _parent; ///< Index is child and value is the child's parent
	std::vector<size_t> _rank; ///< Index is ID and value is its rank
	size_t _unique_count = 0; ///< Number of unique sets
};

} // namespace figures_counter