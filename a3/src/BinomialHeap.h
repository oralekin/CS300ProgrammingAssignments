#pragma once
#include "Heap.h"

class BinomialHeap : public Heap {
	public:
	void insert(int task_id, int priority) override;
	int extract_min() override;
	void decrease_key(int task_id, int new_priority) override;
	void merge_with(BinomialHeap &other);
	void print_heap() const override;
};
