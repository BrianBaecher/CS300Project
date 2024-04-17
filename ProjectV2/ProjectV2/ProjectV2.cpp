// ProjectV2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_set>
#include <sstream>
#include <algorithm>
#include "HashTable.hpp"

using namespace std;


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

// read file
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



bool IsNotAlphanumeric(char c) {
	return isalnum(c) == 0;
}


HashTable* CreateAndPopulateTable(vector<vector<string>> parsedRows, unordered_set<string> validCourses) {
	int tableSize = parsedRows.size();
	HashTable* table = new HashTable(tableSize);
	for (vector<string>& row : parsedRows) {
		string courseId = row.at(0);
		string courseName = row.at(1);
		Course newCourse = Course(courseId, courseName);
		if (row.size() > 2) {
			for (int i = 2; i < row.size(); i++) {
				newCourse.AddCoursePrerequisite(row.at(i), validCourses);
			}
		}
		table->Insert(newCourse);
	}
	return table;
}


int main()
{
	unordered_set<string> validCourses;
	HashTable* courseTable = nullptr;

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

			courseTable = CreateAndPopulateTable(parsedRows, validCourses);

			break;
		}

		case 2: {
			vector<string> validCopy;
			// print courses in order
			for (string s : validCourses) {
				validCopy.push_back(s);
			}
			sort(validCopy.begin(), validCopy.end());

			for (string s : validCopy) {
				Course c = courseTable->Search(s);
				if (!c.GetIsEmpty()) {
					c.PrintCourseInfo();
				}
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

			//Course c = CourseVectorBinarySearch(courseList, searchTerm);

			Course c = courseTable->Search(searchTerm);

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

