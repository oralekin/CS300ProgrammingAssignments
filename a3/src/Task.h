#pragma once
#include <string>
#include <ostream>

class Task {
	public:
	int task_id;
	int priority;
	std::string location;
	std::string category;

	Task(int id, int prio, std::string loc = "", std::string cat = "") :
			task_id(id), priority(prio), location(loc), category(cat) {}

	// comparison operators should compare priority
	inline bool operator<(const Task &rhs) const {
		return priority < rhs.priority;
	}

	inline bool operator>(const Task &rhs) const {
		return rhs < (*this);
	}

	inline bool operator>=(const Task &rhs) const {
		return !((*this) < rhs);
	}

	inline bool operator<=(const Task &rhs) const {
		return !(rhs < (*this));
	}
};