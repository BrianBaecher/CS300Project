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
				prereqs += this->prerequisites.at(i);
				if (i < this->prerequisites.size() - 1) {
					prereqs += ", ";
				}
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
		cout << this->courseId << ", " << this->courseName << endl << this->GetCoursePrerequisites() << endl;
	}
};


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

/**
Removes all whitespace and non-alphanumeric characters from string, also capitalizes all alphabetical chars within string.

@param input - string from input to treat
@return string - treated input string. Example: "abc - 123" returns "ABC123"
*/
string TreatSearchInput(string input) {
	// Note - the third argument to remove_if is an anonymous function
	input.erase(remove_if(input.begin(), input.end(), [](char& c) { 
		if (isalnum(c)) {
			// char is alphanumeric, change to uppercase if possible (toupper will not throw error if char is non-alphabetical)
			c = toupper(c);
			// tell erase not to remove this char
			return false;
		}
		// else, tell erase to remove this char
		return true;
		}), 
		input.end());
	return input;
}
#pragma endregion


/**
Open read text file
*/
vector<vector<string>> ReadAndParseRows(unordered_set<string>& validCourses) {
	string inputPath;
	vector<vector<string>> parsedRows;

	cout << "Enter absolute path to file" << endl;
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
			// get cell contents by splitting each row by comma
			vector<string> parsedRow = SplitOnComma(line);

			if (parsedRow.size() < 2) {
				cerr << "Error processing line: \n" << line << endl;
			}
			else {
				// add row's first cell to set of valid courses
				validCourses.insert(parsedRow.at(0));
				// add the split row to collection
				parsedRows.push_back(parsedRow);
			}
		}
		file.close();
	}
	else {
		cout << "Could not open file at " << inputPath << endl;
	}

	return parsedRows;
}



/**
Sort helper to allow std::sort to make comparisons between Course objects. Course objects are compared based on the value of their courseId string member.
*/
bool CourseSortHelper(Course& course1, Course& course2) {
	return course1.GetCourseId() < course2.GetCourseId();
}

/**
Creates a vector holding Course objects.

@param parsedRows - vector of split strings obtained from reading input file.
@param validCourses - set of valid courseId strings, which should be populated when reading input file.

@return vector of Course objects, ordered from low-to-high courseId.
*/
vector<Course> CreateCourseVector(vector<vector<string>>& parsedRows, unordered_set<string>& validCourses) {
	vector<Course> courses;

	// reserving space for courses vector, one element for each row from input file. 
	// Used to prevent courses vector from having to re-size during additions.
	courses.reserve(parsedRows.size()); 

	for (const vector<string>& row : parsedRows) {
		string courseId = row.at(0);
		string courseName = row.at(1);
		Course course = Course(courseId, courseName);
		if (row.size() > 2) { // any element of a row that is at or above idx 2 represents a prerequisite courseId.
			for (int i = 2; i < row.size(); i++) {
				// calling Course method to add prereqs, pass in set of valid courses to validate that the prereq exists in course list.
				course.AddCoursePrerequisite(row.at(i), validCourses);
			}
		}
		courses.push_back(course);
	}
	// sorting vector before returning, using std::sort with the helper function
	sort(courses.begin(), courses.end(), CourseSortHelper);
	return courses;
}

/**
Search function for an ordered vector of Course objects.

@param courseVector - the vector used to store the Course objects
@param searchTerm - string to search for inputted by user

@return Course object - Course with courseId matching searchTerm OR empty Course object if no match exists.
*/
Course CourseVectorBinarySearch(vector<Course>& courseVector, string searchTerm) {
	//treat input to allow for accurate courseId comparisons
	string treatedSearchTerm = TreatSearchInput(searchTerm);
	int lowIdx = 0;
	int highIdx = courseVector.size() - 1;

	while (lowIdx <= highIdx) {
		int midIdx = lowIdx + (highIdx - lowIdx) / 2;

		if (courseVector.at(midIdx).GetCourseId() == treatedSearchTerm) {
			return courseVector.at(midIdx);
		}
		else if (courseVector.at(midIdx).GetCourseId() < treatedSearchTerm) {
			// restrict search to upper half of partition
			lowIdx = midIdx + 1;
		}
		else {
			// restrict search to lower half of partition
			highIdx = midIdx - 1;
		}
	}
	// return empty Course object if search is unsuccessful.
	return Course();
}

/**
Program driver.
*/
int main()
{
	cout << "Welcome to the course planner." << endl;

	unordered_set<string> validCourses;
	vector<Course> courseVector;

	bool isRunning = true;
	while (isRunning) {

		int decision = NULL;

		bool isValidDecision = false;
		while (!isValidDecision) {
			cout << endl;
			cout << "  1. Load Courses" << endl;
			cout << "  2. Display Courses in order" << endl;
			cout << "  3. Search for Course by ID" << endl;
			cout << "  4. Exit Program" << endl;
			cout << endl;
			cout << "What would you like to do? ";

			string input;
			cin >> input;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			try {
				decision = stoi(input);
				isValidDecision = 1 <= decision && decision <= 4;
				if (!isValidDecision) {
					cout << decision << " is not a valid option." << endl;
				}
			}
			catch (const invalid_argument& err) {
				cout << input << " is not a valid input. Please enter a number in range 1 to 4." << endl;
				break;
			}
		}

		switch (decision)
		{
		case 1: {
			//load courses to vector
			vector<vector<string>> parsedRows = ReadAndParseRows(validCourses);
			courseVector = CreateCourseVector(parsedRows, validCourses);
			break;
		}

		case 2: {
			// print courses in order (courseVector is already in order, sorted by CreateCourseVector function)
			cout << "Here is a sample schedule: " << endl << endl;

			for (Course& c : courseVector) {
				cout << c.GetCourseId() << ", " << c.GetCourseName() << endl;
			}

			cout << endl;

			break;
		}

		case 3: {
			// searching for a course 
			string input;
			cout << "What course do you want to know about? ";
			// read input from user
			getline(cin, input);

			// call binary search function on the vector with the user input
			// Note - user input is treated/cleaned within the function.
			Course c = CourseVectorBinarySearch(courseVector, input);
			
			cout << endl;

			if (!c.GetIsEmpty()) {
				// if not empty, search was a success
				c.PrintCourseInfo();
			}
			else {
				cout << "no results found for \"" << input << "\"" << endl;
			}
			break;
		}

		case 4: {
			// exit program
			isRunning = false; // ending main while loop
			cout << "Thank you for using the course planner!" << endl;
			break;
		}

		default:
			// default case should not run, as invalid inputs are dealt with before the switch statement begins.
			break;
		}

	}

}

