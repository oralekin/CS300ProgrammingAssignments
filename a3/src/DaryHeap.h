#pragma once
#include "Heap.h"

class DaryHeap : public Heap {
	public:
	DaryHeap(int d);
	void insert(int task_id, int priority) override;
	int extract_min() override;
	void decrease_key(int task_id, int new_priority) override;
	void merge_with(DaryHeap &other);
	void print_heap() const override;
};
