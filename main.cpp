#include <iostream>
#include "rb_tree.h"
#include "sequence.h"
#include "single_list.h"

#include <random>
#include <ctime>

//struct A {
//	int    a;
//	double b;
//	char   c;
//
//	A() { std::cout << "construct" << std::endl; }
//
//	A(int a, double b, char c) : a(a), b(b), c(c) {
//		std::cout << "construct" << std::endl;
//	}
//
//	~A() { std::cout << "destruct" << std::endl; }
//};
//
//struct indict_to {
//
//	int operator()(int key) const {
//		return key;
//	}
//};
//
//struct less {
//
//	bool operator()(int a, int b) const {
//		return a < b;
//	}
//};

int main() {

	std::mt19937 rand_engine((unsigned int) time(nullptr));
	std::uniform_int_distribution<int> rand_int(-100, 100);

//	tools::rb_tree<int, int, indict_to, less> tree;
//
//	tree.insert_equal(1);
//	tree.insert_equal(2);
//	tree.insert_equal(5);
//	tree.insert_equal(-3);
//	tree.insert_equal(-2);
//
//	std::vector<int> vec;
//
//	vec.begin() == vec.end();
//
//	for (auto i = tree.begin(); tree.end() != i; i++) {
//		std::cout << *i << " ";
//	}

//	tools::sequence<int> seq;
//
//	std::cout << seq.empty() << std::endl;
//
//	for (int i = 0; i < 10; ++i) {
//		seq.push_back(i);
//	}
//
//	for (auto iter = seq.begin(); seq.end() != iter; ++iter) {
//		std::cout << *iter << " ";
//	}
//
//	std::cout << std::endl << seq.empty() << ", " << seq.size() << ", " << seq.capacity() << std::endl;
//
//	auto iter = seq.end() - 1;
//	seq.erase(iter);
//
//	std::cout << seq.size();
//
//	for (auto iter = seq.begin(); seq.end() != iter; ++iter) {
//		std::cout << *iter << " ";
//	}

	tools::single_list<int> list;

	std::cout << "empty: " << list.empty() << std::endl;

	for (int i = 0; i < 10; ++i) {
		list.push_front(i);
		list.push_back(20 - i);
	}

	while (!list.empty()) {
		list.pop_front();
	}

	std::cout << "empty: " << list.empty() << std::endl;
	std::cout << "size : " << list.size() << std::endl;

	for (auto iter = list.begin(); list.end() != iter; ++iter) {
		std::cout << *iter << " ";
	}

	return 0;
}














