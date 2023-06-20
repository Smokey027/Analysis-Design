//============================================================================
// Name        : FinalProject.cpp
// Author      : Scot C. Batton
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <vector>

using namespace std;

class Course { // Course class for information about course
public:
    string title;
    vector<string> prerequisites;
};

struct CourseHash { // Hash function for course objects
    size_t operator()(const Course& c) const {
        return hash<string>()(c.title);
    }
};

struct CourseEqual { // Equality function for course objects
    bool operator()(const Course& c1, const Course& c2) const {
        return c1.title == c2.title;
    }
};

void loadCourses(unordered_map<string, Course>& courses) { // Function for loading the data into the hash table
    string filename;
    cout << "Enter the name of the course data file: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) { // Check if file is open
        cout << "Error: could not open file " << filename << endl;
        return;
}

    string line; // Parse data from each line of the file
    while (getline(file, line)) {
        string courseNumber = line.substr(0, 7);
        string title = line.substr(8, 50);
        vector<string> prerequisites;

        if (line.length() > 59) {
            string prereqString = line.substr(59);
            size_t pos = prereqString.find_first_of("0123456789");
            while (pos != string::npos) { // Parse prereqs from line
                string prereqNumber = prereqString.substr(pos, 7);
                prerequisites.push_back(prereqNumber);
                pos = prereqString.find_first_of("0123456789", pos + 1);
            }
        }

        Course course;
        course.title = title;
        course.prerequisites = prerequisites;
        courses[courseNumber] = course; // Add course to hash table
    }

    cout << "Course data loaded successfully." << endl;
}

void printCourseList(const unordered_map<string, Course>& courses) { // Function for printing course list
    vector<string> courseNumbers;
    for (const auto& pair : courses) {
        courseNumbers.push_back(pair.first);
    }
    sort(courseNumbers.begin(), courseNumbers.end());

    cout << "Course List:" << endl;
    for (const auto& number : courseNumbers) {
        cout << number << ": " << courses.at(number).title << endl;
    }
}

void printCourse(const unordered_map<string, Course>& courses) { // Function for printing course
    string courseNumber;
    cout << "Enter the course number: ";
    cin >> courseNumber;

    if (courses.count(courseNumber) == 0) {
        cout << "Error: invalid course number." << endl;
        return;
    }

    const Course& course = courses.at(courseNumber);

    cout << "Title: " << course.title << endl;
    cout << "Prerequisites: ";
    if (course.prerequisites.empty()) {
        cout << "None" << endl;
    } else {
        for (const auto& prereqNumber : course.prerequisites) {
            cout << prereqNumber << ": " << courses.at(prereqNumber).title << ", ";
        }
        cout << endl;
    }
}

int main() {
    unordered_map<string, Course> courses; // Hash table for courses

    while (true) { // Print menu
        cout << "Menu:" << endl;
        cout << "1. Load Data Structure" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course" << endl;
        cout << "4. Exit" << endl;

        int choice; // User will give menu choice
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            loadCourses(courses);
            break;

        case 2:
            if (courses.empty()) {
                cout << "Error: no course data loaded." << endl;
            }
            else {
                printCourseList(courses);
            }
            break;

        case 3:
            if (courses.empty()) {
                cout << "Error: no course data loaded." << endl;
            }
            else {
                printCourse(courses);
            }
            break;

        case 4:
            return 0;

        default:
            cout << "Error: invalid menu choice." << endl;
            break;
        }

        cout << endl;
    }
}