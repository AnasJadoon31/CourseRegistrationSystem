#include "System.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

CourseRegistrationSystem::CourseRegistrationSystem() {
    currentUser = nullptr;
    loadData(); // Load data on startup
}

CourseRegistrationSystem::~CourseRegistrationSystem() {
    saveData(); // Save data on exit
}

void CourseRegistrationSystem::seedData() {
    // Only seed if no users exist (first run)
    if (users.getHead() != nullptr) return;

    // Create default admin account
    users.insert(User("admin", "admin123", "System Administrator", "ADMIN001", true));

    // Create sample students
    users.insert(User("Ali", "123", "Ali Ahmed", "02-134242-001", false));
    users.insert(User("Sara", "123", "Sara Khan", "02-134242-002", false));
    users.insert(User("Anas", "123", "Anas Khan", "02-134242-068", false));
    users.insert(User("Adil", "123", "Adil Shabbir", "02-134242-033", false));
    users.insert(User("Amjad", "123", "Amjad Ellahi", "02-134242-092", false));


    // Add sample courses
    courses.insert(Course("CS101", "Introduction to Programming", 3, 30));
    courses.insert(Course("CS201", "Data Structures and Algorithms", 4, 25));
    courses.insert(Course("CS301", "Database Systems", 3, 20));
    courses.insert(Course("CS401", "Software Engineering", 4, 15));
    courses.insert(Course("MATH101", "Calculus I", 3, 35));
    courses.insert(Course("ENG101", "English Composition", 2, 40));

    // Add prerequisites
    prerequisites.addPrerequisite("CS201", "CS101");
    prerequisites.addPrerequisite("CS301", "CS201");
    prerequisites.addPrerequisite("CS401", "CS301");

    // Enroll random courses for students
    enrollments.insert(Enrollment("Ali", "CS101"));
    enrollments.insert(Enrollment("Ali", "MATH101"));
    enrollments.insert(Enrollment("Sara", "CS101"));
    enrollments.insert(Enrollment("Sara", "ENG101"));
    enrollments.insert(Enrollment("Anas", "CS201"));
    enrollments.insert(Enrollment("Adil", "CS301"));
    enrollments.insert(Enrollment("Amjad", "CS401"));
    enrollments.insert(Enrollment("student", "CS101"));

    saveData(); // Save initial seed data
}

bool CourseRegistrationSystem::login(const string& username, const string& password) {
    User* user = users.search(username, userComparator);
    if (user != nullptr) {
        // Use KMP for password matching (demonstration purpose)
        if (kmpSearch(user->getPassword(), password) && user->getPassword().length() == password.length()) {
            currentUser = user;
            cout << "Login successful! Welcome, " << user->getFullName() << "\n";
            if (user->getIsAdmin()) {
                cout << "Logged in as Administrator.\n";
            } else {
                cout << "Logged in as Student.\n";
            }
            return true;
        }
    }
    return false;
}

void CourseRegistrationSystem::logout() {
    if (currentUser != nullptr) {
        cout << "Logged out successfully.\n";
        currentUser = nullptr;
    }
}

void CourseRegistrationSystem::registerUser(const string& username, const string& password, const string& fullName, const string& rollNo) {
    // Validate inputs
    if (username.empty()) {
        cout << "Error: Username cannot be empty!\n";
        return;
    }

    if (password.empty()) {
        cout << "Error: Password cannot be empty!\n";
        return;
    }

    if (password.length() < 3) {
        cout << "Error: Password must be at least 3 characters long!\n";
        return;
    }

    if (fullName.empty()) {
        cout << "Error: Full name cannot be empty!\n";
        return;
    }

    if (rollNo.empty()) {
        cout << "Error: Roll number cannot be empty!\n";
        return;
    }

    // Check for spaces in username
    if (username.find(' ') != string::npos) {
        cout << "Error: Username cannot contain spaces!\n";
        return;
    }

    User* existing = users.search(username, userComparator);
    if (existing != nullptr) {
        cout << "Error: Username already exists!\n";
        return;
    }

    User* existingRoll = users.search(rollNo, rollNoComparator);
    if (existingRoll != nullptr) {
        cout << "Error: User with this Roll No already exists!\n";
        return;
    }

    users.insert(User(username, password, fullName, rollNo, false));
    cout << "Registration successful! You can now login.\n";
    saveData();
}

bool CourseRegistrationSystem::isCurrentUserAdmin() {
    return currentUser != nullptr && currentUser->getIsAdmin();
}

void CourseRegistrationSystem::viewAllCourses(int sortOption) {
    vector<Course> courseList;
    collectCourses(courses.getRoot(), courseList);

    if (courseList.empty()) {
        cout << "No courses available.\n";
        return;
    }

    if (sortOption == 0) {
        // Sort by code (default BST inorder)
        cout << "\n--- All Courses (Sorted by Code) ---\n";
    } else {
        // Sort by name
        cout << "\n--- All Courses (Sorted by Name) ---\n";
        sortCoursesByName(courseList);
    }

    for (const auto& course : courseList) {
        cout << "Code: " << course.getCode()
             << " | Name: " << course.getName()
             << " | Credit Hours: " << course.getCreditHours()
             << " | Available Seats: " << course.getAvailableSeats()
             << "/" << course.getTotalSeats();

        vector<string> prereqs = prerequisites.getPrerequisites(course.getCode());
        if (!prereqs.empty()) {
            cout << " | Prerequisites: ";
            for (size_t i = 0; i < prereqs.size(); ++i) {
                cout << prereqs[i] << (i < prereqs.size() - 1 ? ", " : "");
            }
        } else {
            cout << " | Prerequisites: None";
        }
        cout << "\n";
    }
}

void CourseRegistrationSystem::collectCourses(BSTNode* node, vector<Course>& courseList) {
    if (node != nullptr) {
        collectCourses(node->left, courseList);
        courseList.push_back(node->data);
        collectCourses(node->right, courseList);
    }
}

void CourseRegistrationSystem::sortCoursesByName(vector<Course>& courseList) {
    sort(courseList.begin(), courseList.end(), [](const Course& a, const Course& b) {
        return a.getName() < b.getName();
    });
}

void CourseRegistrationSystem::searchCourse(const string& code) {
    Course* course = courses.search(code);
    if (course != nullptr) {
        cout << "\n--- Course Details ---\n";
        cout << "Code: " << course->getCode() << "\n";
        cout << "Name: " << course->getName() << "\n";
        cout << "Credit Hours: " << course->getCreditHours() << "\n";
        cout << "Available Seats: " << course->getAvailableSeats() << "/" << course->getTotalSeats() << "\n";

        vector<string> prereqs = prerequisites.getPrerequisites(course->getCode());
        cout << "Prerequisites: ";
        if (!prereqs.empty()) {
            for (size_t i = 0; i < prereqs.size(); ++i) {
                cout << prereqs[i] << (i < prereqs.size() - 1 ? ", " : "");
            }
        } else {
            cout << "None";
        }
        cout << "\n";
    } else {
        cout << "Course not found!\n";
    }
}

void CourseRegistrationSystem::enrollCourse(const string& code) {
    if (currentUser == nullptr) {
        cout << "Please login first!\n";
        return;
    }

    if (currentUser->getIsAdmin()) {
        cout << "Administrators cannot enroll in courses!\n";
        return;
    }

    Course* course = courses.search(code);
    if (course == nullptr) {
        cout << "Course not found!\n";
        return;
    }

    // Check if already enrolled
    Node<Enrollment>* current = enrollments.getHead();
    while (current != nullptr) {
        if (current->data.username == currentUser->getUsername() &&
            current->data.courseCode == code) {
            cout << "You are already enrolled in this course!\n";
            return;
        }
        current = current->next;
    }

    // Check prerequisites
    if (!checkPrerequisites(code)) {
        cout << "You have not completed the prerequisites for this course!\n";
        vector<string> prereqs = prerequisites.getPrerequisites(code);
        cout << "Prerequisites: ";
        for (const string& p : prereqs) cout << p << " ";
        cout << "\n";
        return;
    }

    if (course->enrollStudent()) {
        Enrollment enrollment(currentUser->getUsername(), code);
        enrollments.insert(enrollment);
        undoStack.push(enrollment);
        cout << "Successfully enrolled in " << course->getName() << "!\n";
        saveData();
    } else {
        cout << "No seats available!\n";
    }
}

void CourseRegistrationSystem::viewMyHistory() {
    if (currentUser == nullptr) {
        cout << "Please login first!\n";
        return;
    }

    if (currentUser->getIsAdmin()) {
        cout << "Administrators do not have enrollment history!\n";
        return;
    }

    cout << "\n--- My Enrolled Courses ---\n";
    bool hasEnrollments = false;
    Node<Enrollment>* current = enrollments.getHead();
    while (current != nullptr) {
        if (current->data.username == currentUser->getUsername()) {
            Course* course = courses.search(current->data.courseCode);
            if (course != nullptr) {
                cout << "Code: " << course->getCode() << " | Name: " << course->getName() << "\n";
                hasEnrollments = true;
            }
        }
        current = current->next;
    }
    if (!hasEnrollments) {
        cout << "No enrollments yet.\n";
    }
}

void CourseRegistrationSystem::undoLastAction() {
    if (currentUser == nullptr) {
        cout << "Please login first!\n";
        return;
    }

    if (currentUser->getIsAdmin()) {
        cout << "Undo is not available for administrators!\n";
        return;
    }

    Enrollment lastEnrollment("", "");
    if (undoStack.pop(lastEnrollment)) {
        if (lastEnrollment.username == currentUser->getUsername()) {
            string enrollKey = lastEnrollment.username + ":" + lastEnrollment.courseCode;
            if (enrollments.remove(enrollKey, enrollmentComparator)) {
                Course* course = courses.search(lastEnrollment.courseCode);
                if (course != nullptr) {
                    course->unenrollStudent();
                    cout << "Undo successful! Removed enrollment from " << course->getName() << "\n";
                    saveData();
                }
            }
        } else {
            cout << "No action to undo!\n";
        }
    } else {
        cout << "No action to undo!\n";
    }
}

// Admin Functions

void CourseRegistrationSystem::addCourse(const string& code, const string& name, int creditHours, int totalSeats) {
    if (currentUser == nullptr || !currentUser->getIsAdmin()) {
        cout << "Access denied! Admin privileges required.\n";
        return;
    }

    // Validate inputs
    if (code.empty()) {
        cout << "Error: Course code cannot be empty!\n";
        return;
    }

    if (name.empty()) {
        cout << "Error: Course name cannot be empty!\n";
        return;
    }

    if (creditHours <= 0) {
        cout << "Error: Credit hours must be a positive number!\n";
        return;
    }

    if (creditHours > 6) {
        cout << "Error: Credit hours cannot exceed 6!\n";
        return;
    }

    if (totalSeats <= 0) {
        cout << "Error: Total seats must be a positive number!\n";
        return;
    }

    Course* existing = courses.search(code);
    if (existing != nullptr) {
        cout << "Error: Course with this code already exists!\n";
        return;
    }

    // Check if course name already exists
    vector<Course> courseList;
    collectCourses(courses.getRoot(), courseList);
    for (const auto& c : courseList) {
        if (c.getName() == name) {
            cout << "Error: Course with this name already exists!\n";
            return;
        }
    }

    courses.insert(Course(code, name, creditHours, totalSeats));
    cout << "Course added successfully!\n";
    saveData();
}

void CourseRegistrationSystem::deleteCourse(const string& code) {
    if (currentUser == nullptr || !currentUser->getIsAdmin()) {
        cout << "Access denied! Admin privileges required.\n";
        return;
    }

    Course* course = courses.search(code);
    if (course == nullptr) {
        cout << "Course not found!\n";
        return;
    }

    // Remove all enrollments for this course
    Node<Enrollment>* current = enrollments.getHead();
    vector<string> toRemove;
    while (current != nullptr) {
        if (current->data.courseCode == code) {
            toRemove.push_back(current->data.username + ":" + code);
        }
        current = current->next;
    }

    for (const string& key : toRemove) {
        enrollments.remove(key, enrollmentComparator);
    }

    if (courses.deleteCourse(code)) {
        cout << "Course deleted successfully!\n";
        saveData();
    } else {
        cout << "Failed to delete course!\n";
    }
}

void CourseRegistrationSystem::processPayment(const string& transactionId, double amount) {
    if (currentUser == nullptr) {
        cout << "Please login first!\n";
        return;
    }

    // Validate inputs
    if (transactionId.empty()) {
        cout << "Error: Transaction ID cannot be empty!\n";
        return;
    }

    if (amount <= 0) {
        cout << "Error: Amount must be a positive number!\n";
        return;
    }

    if (amount > 100000) {
        cout << "Error: Amount exceeds maximum limit!\n";
        return;
    }

    Payment* existing = payments.search(transactionId);
    if (existing != nullptr) {
        cout << "Error: Transaction ID already exists!\n";
        return;
    }

    Payment newPayment(transactionId, currentUser->getUsername(), amount, "Completed");
    payments.insert(transactionId, newPayment);
    cout << "Payment processed successfully! Transaction ID: " << transactionId << "\n";
    saveData();
}

void CourseRegistrationSystem::viewPaymentStatus(const string& transactionId) {
    if (currentUser == nullptr) {
        cout << "Please login first!\n";
        return;
    }

    if (transactionId.empty()) {
        cout << "Error: Transaction ID cannot be empty!\n";
        return;
    }

    Payment* payment = payments.search(transactionId);
    if (payment != nullptr) {
        if (currentUser->getIsAdmin() || payment->username == currentUser->getUsername()) {
            cout << "\n--- Payment Details ---\n";
            cout << "Transaction ID: " << payment->transactionId << "\n";
            cout << "User: " << payment->username << "\n";
            cout << "Amount: $" << payment->amount << "\n";
            cout << "Status: " << payment->status << "\n";
        } else {
            cout << "Access denied! You can only view your own payments.\n";
        }
    } else {
        cout << "Payment record not found!\n";
    }
}

void CourseRegistrationSystem::updateCourse(const string& code) {
    if (currentUser == nullptr || !currentUser->getIsAdmin()) {
        cout << "Access denied! Admin privileges required.\n";
        return;
    }

    Course* course = courses.search(code);
    if (course == nullptr) {
        cout << "Course not found!\n";
        return;
    }

    cout << "\n--- Update Course ---\n";
    cout << "Current Name: " << course->getName() << "\n";
    cout << "Enter new name (or press enter to keep): ";
    cin.ignore();
    string newName;
    getline(cin, newName);
    if (!newName.empty()) {
        course->setName(newName);
    }

    cout << "Current Credit Hours: " << course->getCreditHours() << "\n";
    cout << "Enter new credit hours (or 0 to keep): ";
    int newCreditHours;
    cin >> newCreditHours;
    if (newCreditHours > 0) {
        course->setCreditHours(newCreditHours);
    }

    cout << "Current Total Seats: " << course->getTotalSeats() << "\n";
    cout << "Enter new total seats (or 0 to keep): ";
    int newTotalSeats;
    cin >> newTotalSeats;
    if (newTotalSeats > 0) {
        int diff = newTotalSeats - course->getTotalSeats();
        course->setTotalSeats(newTotalSeats);
        course->setAvailableSeats(course->getAvailableSeats() + diff);
    }

    cout << "Course updated successfully!\n";
    saveData();
}

void CourseRegistrationSystem::viewAllUsers() {
    if (currentUser == nullptr || !currentUser->getIsAdmin()) {
        cout << "Access denied! Admin privileges required.\n";
        return;
    }

    cout << "\n--- All Users ---\n";
    Node<User>* current = users.getHead();
    while (current != nullptr) {
        cout << "Username: " << current->data.getUsername()
             << " | Name: " << current->data.getFullName()
             << " | Roll No: " << current->data.getRollNo()
             << " | Type: " << (current->data.getIsAdmin() ? "Admin" : "Student") << "\n";
        current = current->next;
    }
}

void CourseRegistrationSystem::deleteUser(const string& username) {
    if (currentUser == nullptr || !currentUser->getIsAdmin()) {
        cout << "Access denied! Admin privileges required.\n";
        return;
    }

    if (username == currentUser->getUsername()) {
        cout << "Cannot delete your own account!\n";
        return;
    }

    User* user = users.search(username, userComparator);
    if (user == nullptr) {
        cout << "User not found!\n";
        return;
    }

    // Remove all enrollments for this user
    Node<Enrollment>* current = enrollments.getHead();
    vector<string> toRemove;
    while (current != nullptr) {
        if (current->data.username == username) {
            toRemove.push_back(username + ":" + current->data.courseCode);
            // Return seats
            Course* course = courses.search(current->data.courseCode);
            if (course != nullptr) {
                course->unenrollStudent();
            }
        }
        current = current->next;
    }

    for (const string& key : toRemove) {
        enrollments.remove(key, enrollmentComparator);
    }

    if (users.remove(username, userComparator)) {
        cout << "User deleted successfully!\n";
        saveData();
    } else {
        cout << "Failed to delete user!\n";
    }
}

void CourseRegistrationSystem::viewCourseEnrollments(const string& code) {
    if (currentUser == nullptr || !currentUser->getIsAdmin()) {
        cout << "Access denied! Admin privileges required.\n";
        return;
    }

    Course* course = courses.search(code);
    if (course == nullptr) {
        cout << "Course not found!\n";
        return;
    }

    cout << "\n--- Enrollments for " << course->getName() << " ---\n";
    bool hasEnrollments = false;
    Node<Enrollment>* current = enrollments.getHead();
    while (current != nullptr) {
        if (current->data.courseCode == code) {
            User* user = users.search(current->data.username, userComparator);
            if (user != nullptr) {
                cout << "Username: " << user->getUsername()
                     << " | Name: " << user->getFullName()
                     << " | Roll No: " << user->getRollNo() << "\n";
                hasEnrollments = true;
            }
        }
        current = current->next;
    }
    if (!hasEnrollments) {
        cout << "No enrollments yet.\n";
    }
}

void CourseRegistrationSystem::viewAllEnrollments() {
    if (currentUser == nullptr || !currentUser->getIsAdmin()) {
        cout << "Access denied! Admin privileges required.\n";
        return;
    }

    cout << "\n--- All Enrollments ---\n";
    Node<Enrollment>* current = enrollments.getHead();
    bool hasEnrollments = false;
    while (current != nullptr) {
        User* user = users.search(current->data.username, userComparator);
        Course* course = courses.search(current->data.courseCode);
        if (user != nullptr && course != nullptr) {
            cout << "Student: " << user->getFullName()
                 << " | Course: " << course->getName()
                 << " (" << course->getCode() << ")\n";
            hasEnrollments = true;
        }
        current = current->next;
    }
    if (!hasEnrollments) {
        cout << "No enrollments yet.\n";
    }
}

void CourseRegistrationSystem::addPrerequisite(const string& course, const string& prereq) {
    if (currentUser == nullptr || !currentUser->getIsAdmin()) {
        cout << "Access denied! Admin privileges required.\n";
        return;
    }

    // Validate inputs
    if (course.empty()) {
        cout << "Error: Course code cannot be empty!\n";
        return;
    }

    if (prereq.empty()) {
        cout << "Error: Prerequisite code cannot be empty!\n";
        return;
    }

    // Check if course exists
    if (courses.search(course) == nullptr) {
        cout << "Error: Course '" << course << "' does not exist!\n";
        return;
    }

    // Check if prerequisite course exists
    if (courses.search(prereq) == nullptr) {
        cout << "Error: Prerequisite course '" << prereq << "' does not exist!\n";
        return;
    }

    // Check for self-reference
    if (course == prereq) {
        cout << "Error: A course cannot be its own prerequisite!\n";
        return;
    }

    // Check for duplicate prerequisite
    if (prerequisites.hasPrerequisite(course, prereq)) {
        cout << "Error: This prerequisite already exists for the course!\n";
        return;
    }

    // Check for circular dependency (if prereq requires course)
    if (prerequisites.hasPrerequisite(prereq, course)) {
        cout << "Error: Circular dependency detected! '" << prereq << "' already requires '" << course << "'.\n";
        return;
    }

    if (prerequisites.addPrerequisite(course, prereq)) {
        cout << "Prerequisite added successfully!\n";
        saveData();
    } else {
        cout << "Failed to add prerequisite!\n";
    }
}

bool CourseRegistrationSystem::checkPrerequisites(const string& courseCode) {
    vector<string> prereqs = prerequisites.getPrerequisites(courseCode);
    if (prereqs.empty()) return true;

    for (const string& prereq : prereqs) {
        bool completed = false;
        // Check if user has enrolled in the prerequisite (simplified check)
        // In a real system, we would check for a passing grade.
        Node<Enrollment>* current = enrollments.getHead();
        while (current != nullptr) {
            if (current->data.username == currentUser->getUsername() &&
                current->data.courseCode == prereq) {
                completed = true;
                break;
            }
            current = current->next;
        }
        if (!completed) return false;
    }
    return true;
}

void CourseRegistrationSystem::saveData() {
    // Save Users
    ofstream userFile("users.txt");
    if (userFile.is_open()) {
        Node<User>* current = users.getHead();
        while (current != nullptr) {
            userFile << current->data.getUsername() << ","
                     << current->data.getPassword() << ","
                     << current->data.getFullName() << ","
                     << current->data.getRollNo() << ","
                     << current->data.getIsAdmin() << "\n";
            current = current->next;
        }
        userFile.close();
    }

    // Save Courses
    ofstream courseFile("courses.txt");
    if (courseFile.is_open()) {
        vector<Course> courseList;
        collectCourses(courses.getRoot(), courseList);
        for (const auto& course : courseList) {
            courseFile << course.getCode() << ","
                       << course.getName() << ","
                       << course.getCreditHours() << ","
                       << course.getTotalSeats() << ","
                       << course.getAvailableSeats() << "\n";
        }
        courseFile.close();
    }

    // Save Enrollments
    ofstream enrollFile("enrollments.txt");
    if (enrollFile.is_open()) {
        Node<Enrollment>* current = enrollments.getHead();
        while (current != nullptr) {
            enrollFile << current->data.username << ","
                       << current->data.courseCode << "\n";
            current = current->next;
        }
        enrollFile.close();
    }
}

void CourseRegistrationSystem::loadData() {
    // Load Users
    ifstream userFile("users.txt");
    if (userFile.is_open()) {
        string line;
        while (getline(userFile, line)) {
            if (line.empty()) continue; // Skip empty lines
            try {
                stringstream ss(line);
                string u, p, n, r, adminStr;
                getline(ss, u, ',');
                getline(ss, p, ',');
                getline(ss, n, ',');
                getline(ss, r, ',');
                getline(ss, adminStr, ',');

                // Validate data before inserting
                if (u.empty() || p.empty() || n.empty() || r.empty()) continue;

                bool isAdmin = (adminStr == "1");
                users.insert(User(u, p, n, r, isAdmin));
            } catch (...) {
                // Skip malformed lines
                continue;
            }
        }
        userFile.close();
    }

    // Load Courses
    ifstream courseFile("courses.txt");
    if (courseFile.is_open()) {
        string line;
        while (getline(courseFile, line)) {
            if (line.empty()) continue; // Skip empty lines
            try {
                stringstream ss(line);
                string c, n, chStr, tsStr, asStr;
                getline(ss, c, ',');
                getline(ss, n, ',');
                getline(ss, chStr, ',');
                getline(ss, tsStr, ',');
                getline(ss, asStr, ',');

                // Validate data before inserting
                if (c.empty() || n.empty() || chStr.empty() || tsStr.empty() || asStr.empty()) continue;

                int creditHours = stoi(chStr);
                int totalSeats = stoi(tsStr);
                int availableSeats = stoi(asStr);

                // Validate numeric values
                if (creditHours <= 0 || totalSeats <= 0 || availableSeats < 0) continue;
                if (availableSeats > totalSeats) availableSeats = totalSeats;

                Course course(c, n, creditHours, totalSeats);
                course.setAvailableSeats(availableSeats);
                courses.insert(course);
            } catch (...) {
                // Skip malformed lines
                continue;
            }
        }
        courseFile.close();
    }

    // Load Enrollments
    ifstream enrollFile("enrollments.txt");
    if (enrollFile.is_open()) {
        string line;
        while (getline(enrollFile, line)) {
            if (line.empty()) continue; // Skip empty lines
            try {
                stringstream ss(line);
                string u, c;
                getline(ss, u, ',');
                getline(ss, c, ',');

                // Validate data before inserting
                if (u.empty() || c.empty()) continue;

                enrollments.insert(Enrollment(u, c));
            } catch (...) {
                // Skip malformed lines
                continue;
            }
        }
        enrollFile.close();
    }
}
