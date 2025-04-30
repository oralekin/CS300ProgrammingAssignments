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
	// map<tasks_id, task_index>
	std::unordered_map<int, int> task_map;

	inline int parent(int index) const;
	int findMinChild(int index) const;

	void swap(int i1, int i2);

	int bubbleUp(int start);
	int sinkDown(int start);

};
