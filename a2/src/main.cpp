#include <iostream>
using namespace std;

template <class T>
// coderunner runs on old c++, so quickly implement optional type
class Option {
	private:

	bool has;

	struct V {
		T value;
	};
	V *vv;

	public:
	// default/none constructor
	Option() : has(false), vv(nullptr) {};
	// "some" constructor
	Option(T _value) : has(true), vv(new V) {
		vv->value = _value;
	};
	// copy cons
	Option(const Option<T> &other) : has(other.has), vv(other.vv) {};
	// move cons
	Option(Option<T> &&other) {
		swap(vv, other.vv);
		swap(has, other.has);
	};

	constexpr T &operator*() const {
		return vv->value;
	};
	constexpr T operator->() const {
		return vv->value;
	};
	constexpr bool operator==(const Option<T> &other) const {
		return has && other.has && *vv->value == *other.vv->value;
	}
	constexpr operator bool() const {
		return has;
	}
};

template<class Item>
// works for all items overloading operator== or operator!=
class HashTable {
	private:
	struct Slot {
		enum SlotStatus {
			VIRGIN,
			INUSE,
			DELETED,
		};

		SlotStatus status;
		Item *value;
		Slot() : status(VIRGIN), value(nullptr) {};
	};

	int capacity;
	Slot *table = nullptr;
	int _size = 0;

	typedef int(*HashFunctionPointer)(const Item &);
	HashFunctionPointer hash;
	// can't construct without associated hash function
	HashTable() = delete;

	public:
	HashTable(int capacity, HashFunctionPointer hash) : capacity(capacity), hash(hash) {
		table = new Slot[capacity];
	}

	Option<Item &> find(const Item &predicate) {
		// TODO
		return {};
	};

	bool insert(Item i) {
		// TODO
		return false;
	};

	Option<Item> remove(const Item &predicate) {
		// TODO
		return {};
	};

	int size() const {
		return _size;
	};
};

// ---------------- specifics

struct StringCode {
	string str;
	int code;

	StringCode(string str, int code) : str(str), code(code) {};
	StringCode(char c) : StringCode({ c }, c) {};

	bool operator==(const StringCode &other) const {
		return str == other.str;
	}

	bool operator!=(const StringCode &other) const {
		return !operator==(other);
	}

	static int hash(const StringCode &sc) {
		// TODO
		return 0;
	}
};

class CodeTable : public HashTable<StringCode> {
	CodeTable() : HashTable(4096, &StringCode::hash) {
		for (char c = 1; c < 256; c++)
			insert({ c });
	};

	const Option<int &> find(const string &str) {
		return {};
	};
};

class Compressor {
	// TODO
};


class Decompressor {


	// desired usage:
	/*
	Decompressor d;
	while (d << cin);
	cout << output;
	*/
	istream &operator<<(istream &is) {
		// TODO

		return is;
	};
	friend ostream &operator<<(ostream &os, const Decompressor &d);
};

ostream &operator<<(ostream &os, const Decompressor &d) {
	// TODO

	return os;
}



int main() {
	cout << "Hello World!" << endl;
}