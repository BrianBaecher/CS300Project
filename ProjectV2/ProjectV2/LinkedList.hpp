#ifndef linkedlist
#define linkedlist
#include <iostream>

#include "Course.hpp"

/**
 * Singly linked list class, used to handle chaining in buckets
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
	void PrintList();
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
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Course course) {
	// FIXME (2): Implement append logic
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
 * Output course info for all nodes in list
 */
void LinkedList::PrintList() {
	if (this->Size() > 0) {
		Node* current = head;

		// loop through the nodes, output their information
		while (current != nullptr) {
			//output current bidID, title, amount and fund
			current->course.PrintCourseInfo();
			//set current equal to next
			current = current->next;
		}
	}
}

//}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Course LinkedList::Search(std::string courseId) {
	// FIXME (6): Implement search logic
	if (this->Size() > 0) {
		// start at head
		Node* current = head;

		// loop through list, check each node's data for matching search term
		while (current != nullptr) {
			if (current->course.GetCourseId() == courseId) {
				// return the matching bid, end loop
				return current->course;
			}
			current = current->next;
		}
	}
	// if not found (or list is empty) return empty bid struct
	return Course();
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
	return size;
}

#endif