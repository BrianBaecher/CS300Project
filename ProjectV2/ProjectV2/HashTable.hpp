#ifndef HashTable_H
#define HashTable_H

#include "LinkedList.hpp"
#include <string>


class HashTable {
private:
	// assigning the vector to hold a LinkedList in each bucket
	std::vector<LinkedList> nodes;

	unsigned int tableSize;

	// private hashing method
	unsigned int Hash(std::string courseId);

public:
	HashTable();
	HashTable(unsigned int size);
	virtual ~HashTable();
	void Insert(Course& course);
	Course Search(std::string courseId);
};

/**
  Default constructor
 */
HashTable::HashTable() {
	// Initalize node structure by resizing tableSize
	nodes.resize(tableSize);
}

/**
  Constructor for specifying size of the table
 */
HashTable::HashTable(unsigned int size) {
	// invoke local tableSize to size with this->
	this->tableSize = size;
	nodes.resize(size);
}


/**
  Destructor
 */
HashTable::~HashTable() {
	// erase nodes begin - Note: vector::erase will call destructor for each LinkedList object within the nodes vector.
	nodes.erase(nodes.begin());
}

/**
  Calculate the hash value of a given key.
 */
unsigned int HashTable::Hash(std::string courseId) {
	int sum = 0;
	for (char c : courseId) {
		int asciiVal = static_cast<int>(c);
		sum += asciiVal;
	}
	return (sum % tableSize);
}

/**
  Insert a course
 */
void HashTable::Insert(Course& course) {
	unsigned int key = Hash(course.GetCourseId());

	nodes.at(key).Append(course);
}

/**
  Search for the specific course (by ID)
 */
Course HashTable::Search(std::string courseId) {
	unsigned int key = Hash(courseId);

	// Call LinkedList::Search on the appropriate index, will return an empty course object if unsuccessful.
	return nodes.at(key).Search(courseId);
}

#endif // !HashTable_H