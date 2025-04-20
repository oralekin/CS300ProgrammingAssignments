#pragma once

class Heap {
	public:
	virtual void insert(int task_id, int priority) = 0;
	virtual int extract_min() = 0;
	virtual void decrease_key(int task_id, int new_priority) = 0;
	virtual void merge_with(Heap &other) = 0;
	virtual void print_heap() const = 0;
	virtual ~Heap() = default;
};