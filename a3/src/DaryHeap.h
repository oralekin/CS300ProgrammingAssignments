#pragma once
#include <cstddef>
#include <vector>
#include <unordered_map>
#include "Heap.h"
#include "Task.h"

class DaryHeap : public Heap {
	// required API
	public:
	DaryHeap(int d);
	void insert(int task_id, int priority) override;
	int extract_min() override;
	void decrease_key(int task_id, int new_priority) override;
	void merge_with(DaryHeap &other);
	void print_heap() const override;

	// extra
	void merge_with(Heap &other) override;
	~DaryHeap();

	private:
	const int d;
	std::vector<Task> contents;
	std::unordered_map<int, Task*> task_map;

	// to pretend we have 1-indexed underlying array
	inline int parent(int index) const;
	int findMinChild(int index) const;
};
