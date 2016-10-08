#include<exception>
#include<vector>
#include<iostream>
using namespace std;

template <class T> class CQueue {
	private:
		int front, rear, capacity;
	public:
		vector <T> vec;

		CQueue(int cap) {
			vec = vector<T>(cap);
			front = 0;
			rear = cap - 1;
			capacity = cap;
		}

		bool isEmpty() const {
			return (front == (rear + 1) % capacity);
		}

		bool isFull() const {
			cout << "---------" << endl;
			cout << "front: " <<  front << endl;
			cout << "rear: " << rear << endl;
			cout << "---------" << endl;
			return (front == (rear + 2) % capacity);
		}

		const T& Front() const {
			if (isEmpty()) {
				//cout << ("NO ELEMENTS") << endl;
				return;
			}
			return vec[front];
		}

		void Enqueue(const T& newItem) {
			if (isFull()) {
				cout << "I AM FULL" << endl;
				return;
			}
			rear = (rear + 1) % (capacity);
			vec[rear] = newItem;
		}

		T Dequeue() {
			if (isEmpty()) {
				cout << "EMPTY" << endl;
			}
			front = (front + 1) % capacity;
			return vec[front];
		}
};

int main() {
	CQueue<int> myQueue(5);
	myQueue.Enqueue(1);
	for (std::vector<int>::const_iterator i = myQueue.vec.begin(); i != myQueue.vec.end(); ++i)
		std::cout << *i << ' ';
	cout << endl;
	myQueue.Enqueue(2);
	for (std::vector<int>::const_iterator i = myQueue.vec.begin(); i != myQueue.vec.end(); ++i)
		std::cout << *i << ' ';
	cout << endl;
	myQueue.Enqueue(3);
	for (std::vector<int>::const_iterator i = myQueue.vec.begin(); i != myQueue.vec.end(); ++i)
		std::cout << *i << ' ';
	cout << endl;
	myQueue.Enqueue(4);
	for (std::vector<int>::const_iterator i = myQueue.vec.begin(); i != myQueue.vec.end(); ++i)
		std::cout << *i << ' ';
	cout << endl;
	myQueue.Enqueue(5);
	for (std::vector<int>::const_iterator i = myQueue.vec.begin(); i != myQueue.vec.end(); ++i)
		std::cout << *i << ' ';
	cout << endl;
}