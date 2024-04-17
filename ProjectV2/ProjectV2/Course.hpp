#ifndef Course_H
#define Course_H


#include <vector>
#include <iostream>
#include <unordered_set>
#include <string>


//Course class
class Course {
private:
	std::string courseId;
	std::string courseName;
	std::vector<std::string> prerequisites;

public:
	// Default Constructor
	Course() {};

	// Constructor with Course attributes (prerequisites are assigned through the AddCoursePrerequisites method)
	Course(std::string courseId, std::string courseName) {
		this->courseId = courseId;
		this->courseName = courseName;
	}

	// Getter methods

	/**
	Returns course's courseId (string)
	*/
	std::string GetCourseId() {
		return this->courseId;
	}

	/**
	Returns course's courseName (string)
	*/
	std::string GetCourseName() {
		return this->courseName;
	}

	/**
	Returns a string containing the courseIds for every course within the Course object's prerequisites vector.
	*/
	std::string GetCoursePrerequisites() {
		std::string prereqs;
		if (!this->prerequisites.empty()) {
			prereqs = "Prerequisites: ";
			for (int i = 0; i < this->prerequisites.size(); i++) {
				prereqs += this->prerequisites.at(i) + ", ";
			}
		}
		else {
			prereqs = "No prerequisites";
		}
		return prereqs;
	}

	/**
	Returns a bool indicating whether or not the Course object is empty. 
	"Empty" meaning it is a placeholder object that does not hold values representing a course from the data.
	*/
	bool GetIsEmpty() {
		// if Course object's courseId and courseName are empty, I'm considering it an "empty" Course object.
		return (this->courseId.empty() && this->courseName.empty());
	}

	// Setter method
	/**
	Validates prereq string by ensuring an identical string exists in validCourses. 
	If valid, appends prereq string to Course object's prerequisites member vector.
	@param prereq - string representing a courseId
	@param validCourses - unordered set of strings containing all valid courseIds from input file.
	*/
	void AddCoursePrerequisite(std::string prereq, std::unordered_set<std::string> validCourses) {
		if (validCourses.find(prereq) != validCourses.end()) {
			this->prerequisites.push_back(prereq);
		}
	}

	// Print method
	void PrintCourseInfo() {
		std::cout << this->courseId << ", " << this->courseName << ", " << this->GetCoursePrerequisites() << std::endl;
	}

};
#endif // !Course_H