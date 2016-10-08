/*
James Trollo
CS590-A
Lab 2 - Random Access Iterator
*/
#include <vector>
#include <deque>
#include <iostream>
using namespace std;

//How to handle templating for this class was taken from https://isocpp.org/wiki/faq/templates#template-of-template 
template<typename T>
class MyIterator {
private:
	int start = 0;
	typedef typename T::value_type value_type;
	value_type* current;
	T *cntr;
public:
	MyIterator(T &container, int index) {
		current = &container[index];
		cntr = &container;
	}

	value_type operator*() {
		return *current;
	}

	MyIterator<T> &operator++(value_type) {
		++current;
		return *this;
	}

	MyIterator<T> &operator++() {
		++current;
		return *this;
	}

	MyIterator<T> &operator--(value_type) {
		--current;
		return *this;
	}

	MyIterator<T> &operator--() {
		--current;
		return *this;
	}

	bool operator==(MyIterator other) {
		return ((*current == *(other.current)) && cntr == other.cntr);
	}

	bool operator!=(MyIterator other) {
		return ((*current != *(other.current)) || cntr != other.cntr);
	}
};


template<class T>
void output_all(T begin, T end) {
	T current = begin;
	while (current != end) {
		cout << *current << endl;
		++current;
	}
}

int main() {
	vector<int>myVec{ 1,2,3,4,5,6 };
	MyIterator<vector<int>> it(myVec, 0);
	MyIterator<vector<int>> it2(myVec, 5);
	//output_all(it, it2);

	deque<int>myD{ 1,2,3,4,5,6 };
	deque<int>myD2{ 1,2,3,4,5,6 };
	MyIterator<deque<int>>itD(myD, 0);
	MyIterator<deque<int>>itD2(myD2, 5);
	output_all(it, it2);


}