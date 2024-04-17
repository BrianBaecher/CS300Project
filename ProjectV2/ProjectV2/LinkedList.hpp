#ifndef linkedlist
#define linkedlist
#include <iostream>

#include "Course.hpp"

/**
 * Singly linked list class, used to handle chaining in HashTable's buckets
 */
class LinkedList {

private:
	struct Node {
		Course course;
		Node* next;

		// default constructor
		Node() {
			next = nullptr;
		}

		// initialize with a course
		Node(Course c) : Node() {
			course = c;
		}

	};

	Node* head;
	Node* tail;
	int size = 0;

public:
	LinkedList();
	virtual ~LinkedList();
	void Append(Course course);
	Course Search(std::string courseId);
	int Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
	head = nullptr;
	tail = nullptr;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
	// start at the head
	Node* current = head;
	Node* temp;

	// loop over each node, detach from list then delete
	while (current != nullptr) {
		temp = current; // hang on to current node
		current = current->next; // make current the next node
		delete temp; // delete the orphan node
	}
}

/**
 * Append a course to the end of the linked list
 */
void LinkedList::Append(Course course) {
	Node* newNode = new Node(course);

	//special case: list is empty
	if (head == nullptr && tail == nullptr) {
		// if node being appended is the only element in the list, it is both the head and tail of the list
		head = newNode;
		tail = newNode;
	}
	else {
		std::cout << "COLLISION" << std::endl;
		// newNode must now be list's tail, previous tail links to newNode
		tail->next = newNode;
		tail = newNode;
	}
	size++;
}

/**
 * Search for a course by courseId
 * 
 * @param courseId - inputted courseId to search for. Transform input string to adhere to the format "ABCD123", as the search is case and white-space sensitive.
 */
Course LinkedList::Search(std::string courseId) {
	if (this->Size() > 0) {
		// start at head
		Node* current = head;

		// loop through list, check each node's data for matching search term
		while (current != nullptr) {
			if (current->course.GetCourseId() == courseId) {
				// return the matching course, end loop
				return current->course;
			}
			current = current->next;
		}
	}
	// if not found (or list is empty) return empty course object
	return Course();
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
	return size;
}

#endif