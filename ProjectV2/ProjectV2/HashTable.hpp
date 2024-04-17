#ifndef HashTable_H
#define HashTable_H

#include "LinkedList.hpp"
#include <string>


class HashTable {
private:
	// the Bid struct is included via the header file BidStruct.hpp,
	// the Node struct is only used within LinkedList class, so it's in LinkedList.hpp

	// assigning the vector to hold a LinkedList in each bucket
	std::vector<LinkedList> nodes;

	unsigned int tableSize;

	unsigned int Hash(std::string courseId);

public:
	HashTable();
	HashTable(unsigned int size);
	virtual ~HashTable();
	void Insert(Course course);
	Course Search(std::string courseId);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
	// Initalize node structure by resizing tableSize
	nodes.resize(tableSize);
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
	// invoke local tableSize to size with this->
	// resize nodes size
	this->tableSize = size;
	nodes.resize(size);
}


/**
 * Destructor
 */
HashTable::~HashTable() {
	// FIXME (2): Implement logic to free storage when class is destroyed
	// erase nodes begin - Note: vector::erase will call destructor for each LinkedList object within the nodes vector.
	nodes.erase(nodes.begin());
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
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
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Course course) {
	// FIXME (5): Implement logic to insert a bid
	// create the key for the given bid
	unsigned int key = Hash(course.GetCourseId());

	// Append bid to be inserted to the relevant LinkedList object (nodes vector at hashkey idx)
	nodes.at(key).Append(course);
}

/**
 * Search for the specific course (by ID)
 *
 * @param courseId the course id to search for
 */
Course HashTable::Search(std::string courseId) {
	// create the key for the courseId
	unsigned int key = Hash(courseId);

	// Call LinkedList::Search on the appropriate index, will return an empty course object if unsuccessful.
	return nodes.at(key).Search(courseId);
}

#endif // !HashTable_H