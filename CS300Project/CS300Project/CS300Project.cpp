// CS300Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include <stdexcept>


using namespace std;


//Course class
class Course {
private:
	string courseId;
	string courseName;
	vector<string> prerequisites;

public:
	Course() {};

	Course(string courseId, string courseName) {
		this->courseId = courseId;
		this->courseName = courseName;
	}

	//get
	string GetCourseId() {
		return this->courseId;
	}

	string GetCourseName() {
		return this->courseName;
	}

	string GetCoursePrerequisites() {
		string prereqs;
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
	void AddCoursePrerequisite(string prereq, unordered_set<string> validCourses) {
		if (validCourses.find(prereq) != validCourses.end()) {
			this->prerequisites.push_back(prereq);
		}
	}

	void PrintCourseInfo() {
		cout << this->courseId << ", " << this->courseName << ", " << this->GetCoursePrerequisites() << endl;
	}
};



// trimming whitespace
// Credit: https://www.thecrazyprogrammer.com/2021/01/c-string-trim.html?expand_article=1
static string TrimString(string str) {
	const char* typeOfWhitespaces = " \t\n\r\f\v";
	str.erase(str.find_last_not_of(typeOfWhitespaces) + 1);
	str.erase(0, str.find_first_not_of(typeOfWhitespaces));
	return str;
}


// split function
vector<string> SplitOnComma(string str) {
	const char delimiter = ',';
	vector<string> splitStr;
	istringstream iss(str);

	string field;

	while (getline(iss, field, delimiter)) {
		field = TrimString(field);
		splitStr.push_back(field);
	}

	return splitStr;
}


/*
Open read text file
*/

vector<vector<string>> ReadAndParseRows(unordered_set<string>& validCourses) {
	string inputPath;
	vector<vector<string>> parsedRows;

	cout << "Enter path to file" << endl;
	cin >> inputPath;

	ifstream file(inputPath);

	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			bool hasBom = line.substr(0, 3).compare("\xEF\xBB\xBF") == 0;
			if (hasBom) {
				line = line.substr(3);
			}
			vector<string> parsedRow = SplitOnComma(line);

			if (parsedRow.size() < 2) {
				cerr << "Error processing line: \n" << line << endl;
			}
			else {
				validCourses.insert(parsedRow.at(0));
				parsedRows.push_back(parsedRow);
			}
		}

		file.close();
	}
	else {
		cout << "No file" << endl;
	}

	return parsedRows;
}



// sort helper
bool CourseSortHelper(Course& course1, Course& course2) {
	return course1.GetCourseId() < course2.GetCourseId();
}

// create course vector
vector<Course> CreateCourseList(vector<vector<string>> parsedRows, unordered_set<string> validCourses) {
	vector<Course> courses;

	courses.reserve(parsedRows.size());

	for (const vector<string>& row : parsedRows) {
		string courseId = row.at(0);
		string courseName = row.at(1);
		Course course = Course(courseId, courseName);
		if (row.size() > 2) {
			for (int i = 2; i < row.size(); i++) {
				course.AddCoursePrerequisite(row.at(i), validCourses);
			}
		}
		courses.push_back(course);
	}

	sort(courses.begin(), courses.end(), CourseSortHelper);
	return courses;
}


Course CourseVectorBinarySearch(vector<Course>& courseList, string searchTerm) {
	int lowIdx = 0;
	int highIdx = courseList.size() - 1;

	while (lowIdx <= highIdx) {
		int midIdx = lowIdx + (highIdx - lowIdx) / 2;

		if (courseList.at(midIdx).GetCourseId() == searchTerm) {
			return courseList.at(midIdx);
		}
		else if (courseList.at(midIdx).GetCourseId() < searchTerm) {
			lowIdx = midIdx + 1;
		}
		else {
			highIdx = midIdx - 1;
		}
	}
	Course c = Course();
	return c;
}

// i really don't like cpp
bool IsNotAlphanumeric(char c) {
	return isalnum(c) == 0;
}


int main()
{
	unordered_set<string> validCourses;
	vector<Course> courseList;

	bool isRunning = true;
	while (isRunning) {

		int decision;

		bool isValidDecision = false;
		while (!isValidDecision) {
			cout << "Menu:" << endl;
			cout << "  1. Load Courses" << endl;
			cout << "  2. Display Courses in order" << endl;
			cout << "  3. Search for Course by ID" << endl;
			cout << "  4. Exit Program" << endl;
			cout << "Enter choice: ";

			string input;
			cin >> input;

			try {
				decision = stoi(input);
				isValidDecision = true;
			}
			catch (const invalid_argument& err) {
				cout << err.what() << endl;
				cout << "try again" << endl;
				break;
			}
		}

		switch (decision)
		{
		case 1: {
			//load courses
			vector<vector<string>> parsedRows = ReadAndParseRows(validCourses);
			courseList = CreateCourseList(parsedRows, validCourses);
			break;
		}

		case 2: {
			// print courses in order
			for (Course& c : courseList) {
				c.PrintCourseInfo();
			}
			break;
		}

		case 3: {
			string searchTerm;
			cout << "Enter Course ID to search for." << endl;
			cin >> searchTerm;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			searchTerm.erase(remove_if(searchTerm.begin(), searchTerm.end(), IsNotAlphanumeric), searchTerm.end());
			for_each(searchTerm.begin(), searchTerm.end(), [](char& c) { c = ::toupper(c); });

			cout << searchTerm << endl;

			Course c = CourseVectorBinarySearch(courseList, searchTerm);

			if (!c.GetIsEmpty()) {
				c.PrintCourseInfo();
			}
			else {
				cout << "no results for search " << searchTerm << endl;
			}
			break;
		}

		case 4: {
			isRunning = false;
			break;
		}

		default:
			break;
		}

	}

}

