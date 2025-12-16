#ifndef MODELS_H
#define MODELS_H

#include <string>
using namespace std;

// Base User class
class User {
protected:
    string username;
    string password;
    string fullName;
    string rollNo;
    bool isAdmin;

public:
    User() : username(""), password(""), fullName(""), rollNo(""), isAdmin(false) {}
    User(string u, string p, string n, string r, bool admin = false)
        : username(u), password(p), fullName(n), rollNo(r), isAdmin(admin) {}

    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getFullName() const { return fullName; }
    string getRollNo() const { return rollNo; }
    bool getIsAdmin() const { return isAdmin; }

    void setUsername(string u) { username = u; }
    void setPassword(string p) { password = p; }
    void setFullName(string n) { fullName = n; }
    void setRollNo(string r) { rollNo = r; }
};

// Course class
class Course {
private:
    string code;
    string name;
    int creditHours;
    int totalSeats;
    int availableSeats;

public:
    Course() : code(""), name(""), creditHours(0), totalSeats(0), availableSeats(0) {}
    Course(string c, string n, int ch, int ts)
        : code(c), name(n), creditHours(ch), totalSeats(ts), availableSeats(ts) {}

    string getCode() const { return code; }
    string getName() const { return name; }
    int getCreditHours() const { return creditHours; }
    int getTotalSeats() const { return totalSeats; }
    int getAvailableSeats() const { return availableSeats; }

    void setCode(string c) { code = c; }
    void setName(string n) { name = n; }
    void setCreditHours(int ch) { creditHours = ch; }
    void setTotalSeats(int ts) { totalSeats = ts; }
    void setAvailableSeats(int as) { availableSeats = as; }

    bool enrollStudent() {
        if (availableSeats > 0) {
            availableSeats--;
            return true;
        }
        return false;
    }

    void unenrollStudent() {
        if (availableSeats < totalSeats) {
            availableSeats++;
        }
    }
};

// Enrollment record
struct Enrollment {
    string username;
    string courseCode;
    Enrollment(string u, string c) : username(u), courseCode(c) {}
};

struct Payment {
    string transactionId;
    string username;
    double amount;
    string status; // "Pending", "Completed"
    Payment() : amount(0.0) {}
    Payment(string t, string u, double a, string s) : transactionId(t), username(u), amount(a), status(s) {}
};

#endif
