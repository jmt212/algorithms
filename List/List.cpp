/*
Group 50 James Trollo, Zeyu Ni, Shuo Zhang
Note: Extra credit attempted
*/
#include "List.h"
#include <iostream>
#include <string>
#include <memory>
/*
Allocates and returns a new node with the given pointers. You may
create nodes with NULLs.
*/
template <class T>
shared_ptr<s_node<T>> new_node(T elem, shared_ptr<s_node<T>> next, shared_ptr<s_node<T>> previous) {
	shared_ptr<s_node<T>> newNode (new s_node<T>);
	newNode->elem = elem;
	newNode->next = next;
	newNode->prev = previous;
	return newNode;
}

/*
Inserts a node at the start of the list. DO NOT add a NULL pointer 
*/
template <class T>
void add_node(shared_ptr<s_node<T>> node, shared_ptr<s_node<T>> &head) {
	if (node) {
		node->prev = nullptr;
		node->next = head;
		head->prev = node;
		head = node;
	}
}

/*
Creates a new node with elem and adds it to head. DO NOT add a NULL
elem.
Do not iterate the list.
*/
template <class T>
void add_elem(T elem, shared_ptr<s_node<T>> &head) {
	add_node(new_node(elem, shared_ptr<s_node<T>>(new s_node<T>()), shared_ptr<s_node<T>>(new s_node<T>())), head);
}

/*
Adds a node to the end of a list. DO NOT add a NULL pointer or
a node with a NULL elem.
Iterate at most one time.
*/
template <class T>
void append(shared_ptr<s_node<T>> node, shared_ptr<s_node<T>> &head) {
	if (node) {
		node->next = nullptr;
		if (head->next == nullptr) {
			head->next = node;
			node->prev = head;
		}
		else {
			shared_ptr<s_node<T>> current = head;
			while (current->next != nullptr) {
				current = current->next;
			}
			current->next = node;
			node->prev = current;
		}
	}
}

/*
Adds a node at index n of a list or at the end of the list. if n is
too large.
Iterate at most one time.
*/
template <class T>
void add_node_at(shared_ptr<s_node<T>> node, shared_ptr<s_node<T>> &head, int n) {
	shared_ptr<s_node<T>> current = head;
	int currentIndex = 0;
	do {
		if (n == 0) {
			add_node(node, head);
			return;
		}
		if (n == currentIndex) {
			shared_ptr<s_node<T>> prev = current->prev;
			prev->next = node;
			node->prev = prev;
			node->next = current;
			current->prev = node;
			return;
		}
		currentIndex++;
		current = current->next;
	} while (current->next);
	current->next = node;
}

/*
Removes the first node from the list. Returns elem and frees the node from memory.
Do not iterate the list.
*/
template <class T>
T remove_node(shared_ptr<s_node<T>> &head) {
	T elem;
	if (head) {
		elem = head->elem;
		shared_ptr<s_node<T>> next = head->next;

		head->next = nullptr;
		head.~shared_ptr();
		head = nullptr;

		head = next;
		if (head) {
			head->prev = nullptr;
		}
		return elem;
	}
	return elem;
}

/*
Removes the last node from the list.
Iterate at most one time.
*/
template <class T>
T remove_last(shared_ptr<s_node<T>> &head) {
	shared_ptr<s_node<T>> tail = head;
	shared_ptr<s_node<T>> newTail = nullptr;
	while (tail->next) {
		tail = tail->next;
	}
	newTail = tail->prev;
	T elem = tail->elem;

	tail->prev = nullptr;
	newTail->next = nullptr;

	tail.~shared_ptr();
	tail = nullptr;

	return elem;
}

/*
Removes the node at index n or the last node.
Iterate at most one time.
*/
template <class T>
T remove_node_at(shared_ptr<s_node<T>> &head, int n) {
	shared_ptr<s_node<T>> target = node_at(head, n);
	shared_ptr<s_node<T>> prev = target->prev;
	shared_ptr<s_node<T>> next = target->next;

	prev->next = next;
	next->prev = prev;

	T elem = target->elem;
	target->prev = nullptr;
	target->next = nullptr;
	target.~shared_ptr();
	target = nullptr;

	return elem;
}

/*
Returns the value the length of the list.
Iterate at most one time.
*/
template <class T>
int count_s_node(shared_ptr<s_node<T>> head) {
	if (head && head->next) {
		int length = 1;
		while (head->next) {
			length++;
			head = head->next;
		}
		return length;
	}
	else if (head) {
		return 1;
	}
	else {
		return 0;
	}
}

/*
Frees all the nodes in the list.
CHALLENGE: Write in two lines for +5 extra credit (allowed to use other methods written in this assignment)
*/
template <class T>
void empty_list(shared_ptr<s_node<T>> &head) {
	while (head)
		remove_node(head);
}

/*
Returns a pointer to the node at index n or the last node.
Iterate at most one time.
*/
template <class T>
shared_ptr<s_node<T>> node_at(shared_ptr<s_node<T>> head, int n) {
	if (n == 0) {
		return head;
	}
	else {
		shared_ptr<s_node<T>> current = head;
		int currentIndex = 0;
		do  {
			if (n == currentIndex) {
				return current;
			}
			currentIndex++;
			current = current->next;
		} while (current->next);
		return current;
	}
}

/*
Returns the elem at index n or the last node.
Iterate at most one time.
*/
template <class T>
T elem_at(shared_ptr<s_node<T>> head, int n) {
	return node_at(head, n)->elem;
}

