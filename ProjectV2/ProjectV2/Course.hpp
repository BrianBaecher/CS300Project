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
	Course() {};

	Course(std::string courseId, std::string courseName) {
		this->courseId = courseId;
		this->courseName = courseName;
	}

	//get
	std::string GetCourseId() {
		return this->courseId;
	}

	std::string GetCourseName() {
		return this->courseName;
	}

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

	bool GetIsEmpty() {
		return (this->courseId.empty() && this->courseName.empty());
	}

	// set
	void AddCoursePrerequisite(std::string prereq, std::unordered_set<std::string> validCourses) {
		if (validCourses.find(prereq) != validCourses.end()) {
			this->prerequisites.push_back(prereq);
		}
	}

	void PrintCourseInfo() {
		std::cout << this->courseId << ", " << this->courseName << ", " << this->GetCoursePrerequisites() << std::endl;
	}

};
#endif // !Course_H