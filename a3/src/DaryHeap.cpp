#include "DaryHeap.h"
#include <cassert>

DaryHeap::DaryHeap(int d) : d(d) {}

void DaryHeap::insert(int task_id, int priority) {
	// insert
	contents.emplace_back(task_id, priority);
	task_map[task_id] = contents.size() - 1;

	int finalSpot = bubbleUp(contents.size() - 1);

	assert(contents[finalSpot].task_id == task_id);
	assert(contents[finalSpot].priority == priority);
}

int DaryHeap::extract_min() {
	if (contents.size() == 0) return -1;

	Task tttt = contents.front();
	// move min element to the end, for easy removal.
	swap(0, contents.size() - 1);

	// remove last element & shrink vector
	Task ret = std::move(contents.back());
	contents.pop_back();
	assert(task_map[ret.task_id] == contents.size());

	sinkDown(0);

	return ret.task_id;
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

void DaryHeap::swap(int i1, int i2) {
	// swap in inner array
	std::swap(contents[i1], contents[i2]);

	// change index of tasks in task_map
	task_map[contents[i1].task_id] = i1;
	task_map[contents[i2].task_id] = i2;
}

int DaryHeap::bubbleUp(int start) {
	if (contents.size() == 0) return 0;
	// swap inserted with parent until parent < inserted .
	// clang-format off
	int i;
	for (
    i = start;
    i != 0 && contents[i] < contents[parent(i)];
    i = parent(i)
  )
		swap(i, parent(i));
	// clang-format on
	return i;
}

int DaryHeap::sinkDown(int start) {
	if (contents.size() == 0) return 0;
	// first element is now the old last element.
	// swap first with it's smallest child until order property restored.
	// continue until we're out of elements
	for (int i = start; i < contents.size();) {
		const int minChild_index = findMinChild(i);

		// no child exists, done
		if (minChild_index >= contents.size()) return i;
		// heap order satisfied, done
		if (contents[i] < contents[minChild_index]) return i;

		// swap with child
		swap(i, minChild_index);

		// proceed to child placement;
		i = minChild_index;
	}
	return -1;
}