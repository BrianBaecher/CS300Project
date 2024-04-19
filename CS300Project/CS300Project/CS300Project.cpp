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


#pragma region Course Class Definition
class Course {
private:
	string courseId;
	string courseName;
	vector<string> prerequisites;

public:
	// Default Constructor
	Course() {};

	// Constructor with Course id and name
	Course(string courseId, string courseName) {
		this->courseId = courseId;
		this->courseName = courseName;
	}

	// Getter methods

	/**
	Returns course's courseId (string)
	*/
	string GetCourseId() {
		return this->courseId;
	}

	/**
	Returns course's courseName (string)
	*/
	string GetCourseName() {
		return this->courseName;
	}

	/**
	Returns a string containing the courseIds for every course within the Course object's prerequisites vector.
	*/
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

	/**
	Returns a bool indicating whether or not the Course object is empty.
	"Empty" meaning it is a placeholder object that does not hold values representing a course from the data.
	*/
	bool GetIsEmpty() {
		return (this->courseId.empty() && this->courseName.empty());
	}

	// Setter method
	/**
	Validates prereq string by ensuring an identical string exists in validCourses.
	If valid, appends prereq string to Course object's prerequisites member vector.
	@param prereq - string representing a courseId
	@param validCourses - unordered set of strings containing all valid courseIds from input file.
	*/
	void AddCoursePrerequisite(string prereq, unordered_set<string> validCourses) {
		if (validCourses.find(prereq) != validCourses.end()) {
			this->prerequisites.push_back(prereq);
		}
	}

	// Print method
	void PrintCourseInfo() {
		cout << this->courseId << ", " << this->courseName << ", " << this->GetCoursePrerequisites() << endl;
	}
};
#pragma endregion


#pragma region String manipulation functions
/**
Trims leading and trailing whitespace from string.
Credit: https://www.thecrazyprogrammer.com/2021/01/c-string-trim.html?expand_article=1
*/
static string TrimString(string str) {
	const char* typeOfWhitespaces = " \t\n\r\f\v";
	str.erase(str.find_last_not_of(typeOfWhitespaces) + 1);
	str.erase(0, str.find_first_not_of(typeOfWhitespaces));
	return str;
}


/**
Splits a string using comma ',' as delimiter.
Used when parsing the text input's comma separated string values.

@param str - string to be split
@return vector<string> - vector of substrings from str argument.
*/
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

bool IsNotAlphanumeric(char c) {
	return isalnum(c) == 0;
}

string TreatSearchInput(string input) {
	// remove any char that is not alphanumeric
	input.erase(remove_if(input.begin(), input.end(), [](char& c) { return isalnum(c) == 0; }), input.end());
	// capitalize all alphabetical chars
	for_each(input.begin(), input.end(), [](char& c) { c = toupper(c); });

	return input;
}
#pragma endregion


/**
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
			// removing Byte Order Marker
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


int main()
{

	cout << "Welcome to the course planner.\n" << endl;

	unordered_set<string> validCourses;
	vector<Course> courseList;

	bool isRunning = true;
	while (isRunning) {

		int decision = NULL;

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
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			try {
				decision = stoi(input);
				isValidDecision = 1 <= decision <=4;
			}
			catch (const invalid_argument& err) {
				cout << input << " is not a valid input. Please enter a number between 1 and 4." << endl;
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
			string input;
			cout << "Enter Course ID to search for." << endl;
			getline(cin, input);

			string searchTerm = TreatSearchInput(input);

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

