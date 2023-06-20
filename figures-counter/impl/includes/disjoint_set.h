#pragma once

#include <unordered_map>

namespace figures_counter {

class disjoint_set {
	std::unordered_map<int, int> _parent;

	// stores the depth of trees
	std::unordered_map<int, int> _rank;

	int _uniqueCount = 0;

public:
	// perform MakeSet operation
	int count_unique() const {
		// return uniqueCount;
		auto count = 0;
		for (const auto i : _parent) {
			if (i.first == i.second) {
				++count;
			}
		}
		return count;
	}

	// Find the root of the set in which element `k` belongs
	int find(int k) {
		// if `k` is not the root
		if (_parent[k] != k) {
			// path compression
			_parent[k] = find(_parent[k]);
		}

		return _parent[k];
	}

	// Perform Union of two subsets
	int unite(int a, int b) {
		// find the root of the sets in which elements `x` and `y` belongs
		int x = find(a);
		int y = find(b);

		// if `x` and `y` are present in the same set
		if (x == y) {
			return x;
		}

		--_uniqueCount;

		// Always attach a smaller depth tree under the root of the deeper tree.
		if (_rank[x] > _rank[y]) {
			_parent[y] = x;
			return x;
		} else if (_rank[x] < _rank[y]) {
			_parent[x] = y;
			return y;
		} else {
			_parent[x] = y;
			_rank[y]++;
			return y;
		}
	}

	int add_new() {
		int i = _parent.size();
		_parent[i] = i;
		_rank[i] = 0;

		++_uniqueCount;
		return i;
	}
};

} // namespace figures_counter