/*
    PROJECT: COURSE REGISTRATION SYSTEM
    COURSE: DATA STRUCTURES AND ALGORITHMS

    GROUP MEMBERS:
    1. MUHAMMAD ANAS (02-134242-068)
    2. MUHAMMAD ADIL SHABBIR (02-134242-033)
    3. AMJAD ELLAHI (02-134242-092)

    SUBMITTED TO: MISS RABIA AMJAD
*/

#include <iostream>
#include "System.h"

using namespace std;

void displayMenu() {
    cout << "\n1. Login\n2. Register\n3. Exit\nChoice: ";
}

void studentMenu() {
    cout << "\n--- Student Dashboard ---\n";
    cout << "1. View All Courses (Sort by Code)\n";
    cout << "2. View All Courses (Sort by Name)\n";
    cout << "3. Search Course\n";
    cout << "4. Enroll in Course\n";
    cout << "5. View My History\n";
    cout << "6. Undo Last Action\n";
    cout << "7. Make Payment\n";
    cout << "8. Check Payment Status\n";
    cout << "9. Logout\n";
    cout << "Choice: ";
}

void adminMenu() {
    cout << "\n--- Admin Dashboard ---\n";
    cout << "1. View All Courses (Sort by Code)\n";
    cout << "2. View All Courses (Sort by Name)\n";
    cout << "3. Search Course\n";
    cout << "4. Add Course\n";
    cout << "5. Delete Course\n";
    cout << "6. Update Course\n";
    cout << "7. View All Users\n";
    cout << "8. Delete User\n";
    cout << "9. View Course Enrollments\n";
    cout << "10. View All Enrollments\n";
    cout << "11. Check Payment Status\n";
    cout << "12. Add Prerequisite\n";
    cout << "13. Logout\n";
    cout << "Choice: ";
}

int main() {
    CourseRegistrationSystem sys;
    sys.seedData();

    int choice;
    while (true) {
        displayMenu();

        // Input validation
        if (!(cin >> choice)) {
            cin.clear();  // Clear error flags
            cin.ignore(10000, '\n');  // Discard invalid input
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            string u, p;
            cout << "Username: "; cin >> u;
            cout << "Password: "; cin >> p;

            if (sys.login(u, p)) {
                int subChoice;
                bool loggedIn = true;

                while (loggedIn) {
                    if (sys.isCurrentUserAdmin()) {
                        // Admin menu
                        adminMenu();

                        if (!(cin >> subChoice)) {
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << "Invalid input. Please enter a number.\n";
                            continue;
                        }

                        switch (subChoice) {
                            case 1: sys.viewAllCourses(0); break;
                            case 2: sys.viewAllCourses(1); break;
                            case 3: {
                                string code;
                                cout << "Enter Course Code: "; cin >> code;
                                sys.searchCourse(code);
                                break;
                            }
                            case 4: {
                                string code, name;
                                int creditHours, totalSeats;
                                cout << "Enter Course Code: "; cin >> code;
                                cin.ignore();
                                cout << "Enter Course Name: "; getline(cin, name);
                                cout << "Enter Credit Hours: "; cin >> creditHours;
                                cout << "Enter Total Seats: "; cin >> totalSeats;
                                sys.addCourse(code, name, creditHours, totalSeats);
                                break;
                            }
                            case 5: {
                                string code;
                                cout << "Enter Course Code to delete: "; cin >> code;
                                sys.deleteCourse(code);
                                break;
                            }
                            case 6: {
                                string code;
                                cout << "Enter Course Code to update: "; cin >> code;
                                sys.updateCourse(code);
                                break;
                            }
                            case 7: sys.viewAllUsers(); break;
                            case 8: {
                                string username;
                                cout << "Enter Username to delete: "; cin >> username;
                                sys.deleteUser(username);
                                break;
                            }
                            case 9: {
                                string code;
                                cout << "Enter Course Code: "; cin >> code;
                                sys.viewCourseEnrollments(code);
                                break;
                            }
                            case 10: sys.viewAllEnrollments(); break;
                            case 11: {
                                string tid;
                                cout << "Enter Transaction ID: "; cin >> tid;
                                sys.viewPaymentStatus(tid);
                                break;
                            }
                            case 12: {
                                string course, prereq;
                                cout << "Enter Course Code: "; cin >> course;
                                cout << "Enter Prerequisite Course Code: "; cin >> prereq;
                                sys.addPrerequisite(course, prereq);
                                break;
                            }
                            case 13: sys.logout(); loggedIn = false; break;
                            default: cout << "Invalid choice.\n";
                        }
                    } else {
                        // Student menu
                        studentMenu();

                        if (!(cin >> subChoice)) {
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << "Invalid input. Please enter a number.\n";
                            continue;
                        }

                        switch (subChoice) {
                            case 1: sys.viewAllCourses(0); break;
                            case 2: sys.viewAllCourses(1); break;
                            case 3: {
                                string code;
                                cout << "Enter Course Code: "; cin >> code;
                                sys.searchCourse(code);
                                break;
                            }
                            case 4: {
                                string code;
                                cout << "Enter Course Code: "; cin >> code;
                                sys.enrollCourse(code);
                                break;
                            }
                            case 5: sys.viewMyHistory(); break;
                            case 6: sys.undoLastAction(); break;
                            case 7: {
                                string tid;
                                double amount;
                                cout << "Enter Transaction ID: "; cin >> tid;
                                cout << "Enter Amount: "; cin >> amount;
                                sys.processPayment(tid, amount);
                                break;
                            }
                            case 8: {
                                string tid;
                                cout << "Enter Transaction ID: "; cin >> tid;
                                sys.viewPaymentStatus(tid);
                                break;
                            }
                            case 9: sys.logout(); loggedIn = false; break;
                            default: cout << "Invalid choice.\n";
                        }
                    }
                }
            } else {
                cout << "Invalid credentials.\n";
            }
        }
        else if (choice == 2) {
            string u, p, n, r;
            cout << "Username: ";
            cin >> u;
            cout << "Password: ";
            cin >> p;
            cin.ignore(10000, '\n');  // Clear entire line
            cout << "Full Name: ";
            getline(cin, n);
            cout << "Roll No: ";
            cin >> r;
            sys.registerUser(u, p, n, r);
        }
        else if (choice == 3) {
            cout << "Exiting system. Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}