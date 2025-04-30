#pragma once
#include "Heap.h"
#include <iostream>

using namespace std;

class LoggerHeap : public Heap {
	public:
	void insert(int task_id, int priority) override {
    cout << "void insert " << task_id << ", " << priority << endl;
  }
	int extract_min() override {
    cout << "int extract_min" << endl;
    return 0;
  }
	void decrease_key(int task_id, int new_priority) override {
    cout << "void decrease_key "<< task_id << ", " << new_priority << endl;
  }
	void merge_with(Heap &other) override {
    cout << "void merge_with" << endl;
  }
	void print_heap() const override {
    cout << "void print_heap" << endl;
  }
	~LoggerHeap() override {
    cout << "~Heap" << endl;
  }
};