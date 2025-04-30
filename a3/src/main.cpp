#include <iostream>
#include "BinomialHeap.h"
#include "DaryHeap.h"
#include "InputParser.h"
#include "LoggerHeap.h"

using namespace std;

using HeapPtr = unique_ptr<Heap>;

// basically, reducing using function from Heap x Command to Heap
// TODO: consider using some kinda interesting structure?
HeapPtr buildHeap(vector<Command> commands) {
	// heap should be created with a heap type command at the start.
	if (commands.size() < 1 || !holds_alternative<HeapType>(commands[0]))
		throw invalid_argument("no heap type");

	HeapPtr heap;

	visit(
		overloaded{
			[&](Binomial) { heap = make_unique<BinomialHeap>(); },
			[&](Dary dc) { heap = make_unique<DaryHeap>(dc.d); },
		},
		get<HeapType>(commands[0])
	);

	bool first;
	// do operations
	for (Command command : commands) {
		if (!first) {
			first = true;
			continue;
		}

		visit(
			overloaded{
				[&](HeapType c) {
					/*
					visit(
						overloaded{
							[&](Binomial) {
								// if it is already binomial, skip
								if (!dynamic_cast<BinomialHeap *>(heap.get())) {
									Heap *oldHeap = heap.release();
									heap = make_unique<BinomialHeap>(std::move(*oldHeap));
									delete oldHeap;
								}
							},
							[&](Dary dc) {
								// skip if already dary heap with correct d
								if (auto dh = dynamic_cast<DaryHeap *>(heap.get());
										dh && dh->d == dc.d) {
									Heap *oldHeap = heap.release();
									heap = make_unique<DaryHeap>(std::move(*oldHeap));
									delete oldHeap;
								}
							},
						},
						c
					);
          */
				},
				[&](Insert c) { heap.get()->insert(c.taskId, c.priority); },
				[&](DecreaseKey c) {
					heap.get()->decrease_key(c.taskId, c.newPriority);
				},
				[&](ExtractMin c) {
					cout << "Extracted: " << heap.get()->extract_min() << endl;
				},
				[&](Merge c) {
					heap.get()->merge_with(*(buildHeap(readFile(c.filename)).get()));
				},
				[&](Print c) { heap.get()->print_heap(); },
			},
			command
		);
	}
	return heap;
}

// TODO: merge dependency graph: allow different files to merge with the same other file, but detect cycles in dependency.
// TODO: lazy running: analyze input files for dependencies before doing ops

int main() {
	cout << "Hello World!" << endl;
	try {
		auto comms =
			readFile("/home/oralekin/Code/sabanci/cs300/a/a3/samples/simple.in");
		auto a = buildHeap(comms);
	} catch (UnrecognizedCommand uc) {
		cout << "err: unrecognized " << quoted(uc.command) << endl;
	}
	cout << "done reading" << endl;
}
