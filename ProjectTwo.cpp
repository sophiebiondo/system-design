// ProjectTwo.cpp
// Advising Assistance Program for ABCU Computer Science Department
// Reads course data from a CSV file, stores it in a map, and provides
// menu-driven functionality to list courses and display course details.
// 08/13/2025
// Sophie Biondolillo

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

// Represents a single course with code, title, and prerequisites
class Course {
public:
    Course() = default;
    Course(const std::string& code,
        const std::string& name,
        const std::vector<std::string>& prereqs)
        : courseCode(code), title(name), prerequisites(prereqs) {
    }

    const std::string& getCode() const { return courseCode; }
    const std::string& getTitle() const { return title; }
    const std::vector<std::string>& getPrereqs() const { return prerequisites; }

private:
    std::string courseCode;
    std::string title;
    std::vector<std::string> prerequisites;
};

// Function prototypes
void displayMenu();
int getMenuChoice();
bool loadData(const std::string& filename,
    std::map<std::string, Course>& catalog);
void printAllCourses(const std::map<std::string, Course>& catalog);
void printCourseInfo(const std::map<std::string, Course>& catalog);

int main() {
    std::map<std::string, Course> catalog;  // Stores courses keyed by course code
    bool dataLoaded = false;                // Tracks if data has been loaded

    while (true) {
        displayMenu();
        int choice = getMenuChoice();

        switch (choice) {
        case 1: {
            std::cout << "Enter course data file name: ";
            std::string filename;
            std::getline(std::cin, filename);
            if (loadData(filename, catalog)) {
                dataLoaded = true;
                std::cout << "Data loaded successfully.\n\n";
            }
            else {
                std::cout << "Failed to load data. Please check the file name and format.\n\n";
            }
            break;
        }
        case 2:
            if (!dataLoaded) {
                std::cout << "Please load data first (Option 1).\n\n";
            }
            else {
                printAllCourses(catalog);
            }
            break;

        case 3:
            if (!dataLoaded) {
                std::cout << "Please load data first (Option 1).\n\n";
            }
            else {
                printCourseInfo(catalog);
            }
            break;

        case 9:
            std::cout << "Thank you for using the course planner!\n";
            return 0;

        default:
            std::cout << "Invalid option. Please try again.\n\n";
        }
    }

    return 0;
}

// Displays the main menu options to the user
void displayMenu() {
    std::cout << "===== Advising Assistance Menu =====\n"
        << "1. Load course data from file\n"
        << "2. Print all courses (alphanumeric)\n"
        << "3. Print course title and prerequisites\n"
        << "9. Exit\n"
        << "Select an option: ";
}

// Reads and validates the user's menu choice
int getMenuChoice() {
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    int choice;
    if (!(iss >> choice)) {
        return -1;  // Non-integer input
    }
    return choice;
}

// Loads course information from a CSV file into the catalog map.
// Returns true on success, false on failure.
bool loadData(const std::string& filename,
    std::map<std::string, Course>& catalog) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        return false;
    }

    catalog.clear();
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;

        std::istringstream linestream(line);
        std::string code, title, prereqField;

        // Parse CSV fields: code, title, prerequisites
        std::getline(linestream, code, ',');
        std::getline(linestream, title, ',');
        std::getline(linestream, prereqField);

        // Trim whitespace from parsed strings
        auto trim = [](std::string& s) {
            s.erase(0, s.find_first_not_of(" \t\r\n"));
            s.erase(s.find_last_not_of(" \t\r\n") + 1);
            };
        trim(code);
        trim(title);
        trim(prereqField);

        // Split prerequisites by semicolon if any
        std::vector<std::string> prereqs;
        if (!prereqField.empty()) {
            std::istringstream ps(prereqField);
            std::string token;
            while (std::getline(ps, token, ';')) {
                trim(token);
                if (!token.empty()) {
                    prereqs.push_back(token);
                }
            }
        }

        // Insert the new course into the map
        catalog.emplace(code, Course(code, title, prereqs));
    }

    infile.close();
    return !catalog.empty();
}

// Prints all courses in alphanumeric order (map is already sorted by key)
void printAllCourses(const std::map<std::string, Course>& catalog) {
    std::cout << "\n--- Course List (Alphanumeric) ---\n";
    for (const auto& pair : catalog) {
        std::cout << pair.first
            << ": " << pair.second.getTitle() << "\n";
    }
    std::cout << "\n";
}

// Prompts user for a course code and prints its title and prerequisites
void printCourseInfo(const std::map<std::string, Course>& catalog) {
    std::cout << "Enter course code: ";
    std::string code;
    std::getline(std::cin, code);

    auto it = catalog.find(code);
    if (it == catalog.end()) {
        std::cout << "Course " << code << " not found.\n\n";
        return;
    }

    const Course& course = it->second;
    std::cout << "\nCourse: " << course.getCode()
        << "\nTitle: " << course.getTitle() << "\n";

    const auto& prereqs = course.getPrereqs();
    if (prereqs.empty()) {
        std::cout << "Prerequisites: None\n\n";
    }
    else {
        std::cout << "Prerequisites:\n";
        for (const auto& preCode : prereqs) {
            auto preIt = catalog.find(preCode);
            if (preIt != catalog.end()) {
                std::cout << "  " << preCode
                    << ": " << preIt->second.getTitle() << "\n";
            }
            else {
                std::cout << "  " << preCode
                    << ": (title not found)\n";
            }
        }
        std::cout << "\n";
    }
}