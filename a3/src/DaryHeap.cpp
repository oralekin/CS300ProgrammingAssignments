#include "DaryHeap.h"
#include <cassert>

DaryHeap::DaryHeap(int d) : d(d) {}

void DaryHeap::insert(int task_id, int priority) {
}

int DaryHeap::extract_min() {
}

void DaryHeap::decrease_key(int task_id, int new_priority) {}

void DaryHeap::merge_with(DaryHeap &other) {}

void DaryHeap::print_heap() const {}

void DaryHeap::merge_with(Heap &other) {}

DaryHeap::~DaryHeap() {}

int DaryHeap::parent(int index) const {
	return ((index + 1) / d) + 1;
}

int DaryHeap::findMinChild(int index) const {
	int ret = (index + 1) * d;
	for (int i = 0; i < d; i++) {
		int checking_index = (index + 1) * d + i;
		// no more elements in heap
		if (checking_index >= contents.size()) return ret;
		if (contents[checking_index] < contents[ret]) ret = checking_index;
	}
	return ret;
}
