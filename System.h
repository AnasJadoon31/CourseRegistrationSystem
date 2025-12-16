#ifndef SYSTEM_H
#define SYSTEM_H

#include "DataStructures.h"
#include <vector>

class CourseRegistrationSystem {
private:
    LinkedList<User> users;
    BST courses;
    LinkedList<Enrollment> enrollments;
    Stack<Enrollment> undoStack;
    HashTable<Payment> payments; // Added Payment Hash Table
    Graph prerequisites; // Added Graph for prerequisites
    User* currentUser;

    // Helper functions
    static bool userComparator(const User& u, const string& username) {
        return u.getUsername() == username;
    }

    static bool rollNoComparator(const User& u, const string& rollNo) {
        return u.getRollNo() == rollNo;
    }

    static bool enrollmentComparator(const Enrollment& e, const string& key) {
        return e.username + ":" + e.courseCode == key;
    }

    static void sortCoursesByName(vector<Course>& courseList);
    void collectCourses(BSTNode* node, vector<Course>& courseList);

public:
    CourseRegistrationSystem();
    ~CourseRegistrationSystem();

    // Common functions
    bool login(const string& username, const string& password);
    void logout();
    void registerUser(const string& username, const string& password, const string& fullName, const string& rollNo);
    void seedData();
    bool isCurrentUserAdmin();  // Check if current user is admin

    // Student functions
    void viewAllCourses(int sortOption); // 0: by code, 1: by name
    void searchCourse(const string& code);
    void enrollCourse(const string& code);
    void viewMyHistory();
    void undoLastAction();

    // Admin functions
    void addCourse(const string& code, const string& name, int creditHours, int totalSeats);
    void deleteCourse(const string& code);
    void updateCourse(const string& code);
    void viewAllUsers();
    void deleteUser(const string& username);
    void viewCourseEnrollments(const string& code);
    void viewAllEnrollments();

    // Payment functions
    void processPayment(const string& transactionId, double amount);
    void viewPaymentStatus(const string& transactionId);

    // Prerequisite functions
    void addPrerequisite(const string& course, const string& prereq);
    bool checkPrerequisites(const string& courseCode);

    // File Handling
    void saveData();
    void loadData();
};

#endif
