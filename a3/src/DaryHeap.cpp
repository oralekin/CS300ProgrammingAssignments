#include "DaryHeap.h"
#include <cassert>

DaryHeap::DaryHeap(int d) : d(d) {}

void DaryHeap::insert(int task_id, int priority) {
	// insert
	contents.emplace_back(task_id, priority);

	// swap inserted with parent until parent < inserted .
	// clang-format off
	int i;
	for (
    i == contents.size() - 1;
    i != 0 && contents[i] < contents[parent(i)];
    i = parent(i)
  )
		std::swap(contents[i], contents[parent(i)]);
	// clang-format on

	assert(contents[i].task_id == task_id);
	assert(contents[i].priority == priority);
}

int DaryHeap::extract_min() {
}

void DaryHeap::decrease_key(int task_id, int new_priority) {}

void DaryHeap::merge_with(DaryHeap &other) {}

void DaryHeap::print_heap() const {}

void DaryHeap::merge_with(Heap &other) {}

DaryHeap::~DaryHeap() {}

int DaryHeap::parent(int index) const {
	return (index - 1) / d;
}

/**
 * returns out of bounds index if node doesn't have children.
 */
int DaryHeap::findMinChild(int index) const {
	// initially choose first child
	auto ret = (index * d) + 1;
	for (int offset = 0; offset < d; offset++) {
		auto checking_index = (index + 1) * d + offset;
		// no more elements in heap (or in case no children exist, this will return an out of bounds index on the first loop)
		if (checking_index >= contents.size()) return ret;
		if (contents[checking_index] < contents[ret]) ret = checking_index;
	}
	return ret;
}
