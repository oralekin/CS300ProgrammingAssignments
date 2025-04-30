#include "Task.h"

std::ostream &operator<<(std::ostream &os, const Task &t) {
	os << "Task(tid=" << t.task_id << ", p=" << t.priority << ")";
	return os;
}

Task::Task(int id, int prio, std::string loc, std::string cat) :
		task_id(id), priority(prio), location(loc), category(cat) {}

// comparison operators should compare priority
bool Task::operator<(const Task &rhs) const {
	return priority < rhs.priority;
}

bool Task::operator>(const Task &rhs) const {
	return rhs < (*this);
}

bool Task::operator>=(const Task &rhs) const {
	return !((*this) < rhs);
}

bool Task::operator<=(const Task &rhs) const {
	return !(rhs < (*this));
}
