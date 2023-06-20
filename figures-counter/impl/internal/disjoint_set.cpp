#include <disjoint_set.h>

#include <cassert>
#include <stdexcept>

namespace figures_counter {

size_t disjoint_set::count_unique() const noexcept {
	return _unique_count;
}

size_t disjoint_set::find_parent(size_t k) {
	assert(k < _parent.size());
	if (_parent[k] != k) {
		// Path compression. This will lower the number of operations we need to get to the parent next time.
		_parent[k] = find_parent(_parent[k]);
	}

	return _parent[k];
}

size_t disjoint_set::unite(size_t a, size_t b) {
	const size_t x = find_parent(a);
	const size_t y = find_parent(b);

	// Both are already part of the same set.
	if (x == y) {
		return x;
	}

	assert(_unique_count > 0);
	--_unique_count;

	// Keep the larger rank. This way the count of operations for changing a parent in find will be lower.
	if (_rank[x] > _rank[y]) {
		_parent[y] = x;
		return x;
	}
	if (_rank[x] < _rank[y]) {
		_parent[x] = y;
		return y;
	}
	assert(_rank[x] == _rank[y]);

	// If ranks are equal it doesn't matter which becomes the parent.
	_parent[x] = y;
	_rank[y]++;
	return y;
}

size_t disjoint_set::add_new() {
	const size_t i = _parent.size();
	_parent.push_back(i);
	_rank.push_back(0);
	assert(_parent.size() == _rank.size());

	++_unique_count;
	return i;
}

} // namespace figures_counter