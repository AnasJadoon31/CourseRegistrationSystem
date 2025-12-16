# Course Registration System - Data Structures Documentation

## 📚 Project Overview
This Course Registration System implements **4 different data structures** to efficiently manage users, courses, and enrollments with separate admin and student roles.

---

## 🏗️ Data Structures Used

### 1. **Linked List (Generic Template)**
**File:** `DataStructures.h` (Lines 8-81)

#### Structure:
```
Node → Node → Node → NULL
 ↓      ↓      ↓
Data   Data   Data
```

#### Implementation Details:
- **Template-based**: `LinkedList<T>` - works with any data type
- **Singly Linked**: Each node points to the next node only
- **Dynamic memory allocation**: Grows and shrinks as needed

#### Where Used:
1. **Storing Users** (`System.h` Line 10)
   ```cpp
   LinkedList<User> users;
   ```

2. **Storing Enrollments** (`System.h` Line 12)
   ```cpp
   LinkedList<Enrollment> enrollments;
   ```

#### Operations & Time Complexity:
| Operation | Complexity | Usage |
|-----------|-----------|--------|
| `insert()` | O(n) | Add new user or enrollment at end |
| `search()` | O(n) | Find user by username during login |
| `remove()` | O(n) | Delete user or unenroll from course |
| `getHead()` | O(1) | Traverse all users/enrollments |

#### How It Helps:
✅ **Dynamic Size**: No need to pre-define array size for users  
✅ **Easy Insertion**: Adding new users/enrollments is straightforward  
✅ **Memory Efficient**: Only allocates memory when needed  
✅ **Sequential Access**: Perfect for displaying all users or viewing enrollment history  

#### Real Usage Examples:

**1. User Login (`System.cpp`):**
```cpp
User* user = users.search(username, userComparator);
// Searches through linked list of users to find matching username
// Time: O(n) where n = number of users
```

**2. View All Users (Admin Function):**
```cpp
Node<User>* current = users.getHead();
while (current != nullptr) {
    // Display user information
    current = current->next;
}
// Traverses entire linked list: O(n)
```

**3. View My Enrollment History (Student Function):**
```cpp
Node<Enrollment>* current = enrollments.getHead();
while (current != nullptr) {
    if (current->data.username == currentUser->getUsername()) {
        // Display enrolled course
    }
    current = current->next;
}
// Searches through all enrollments: O(n)
```

---

### 2. **Binary Search Tree (BST)**
**File:** `DataStructures.h` (Lines 127-250)

#### Structure:
```
        CS301
       /     \
   CS201     CS401
    /  \
CS101  MATH101
```

#### Implementation Details:
- **Non-generic**: Specifically stores `Course` objects
- **Sorted by Course Code**: Lexicographically ordered
- **Recursive operations**: Insert, search, delete all use recursion
- **Inorder traversal**: Displays courses in sorted order

#### Where Used:
**Storing Courses** (`System.h` Line 11)
```cpp
BST courses;
```

#### Operations & Time Complexity:
| Operation | Average | Worst Case | Usage |
|-----------|---------|------------|--------|
| `insert()` | O(log n) | O(n) | Add new course (admin) |
| `search()` | O(log n) | O(n) | Find course by code |
| `deleteCourse()` | O(log n) | O(n) | Remove course (admin) |
| `displayInorder()` | O(n) | O(n) | Show all courses sorted |

#### How It Helps:
✅ **Fast Searching**: O(log n) average time to find a course by code  
✅ **Automatic Sorting**: Inorder traversal gives courses in alphabetical order by code  
✅ **Efficient Updates**: Quick course lookups for enrollment/updates  
✅ **Organized Data**: Natural hierarchy matches course code structure  

#### Real Usage Examples:

**1. Search Course (Student/Admin):**
```cpp
Course* course = courses.search("CS201");
// Navigates BST comparing course codes
// Average: O(log n), finds course quickly
```

**2. View All Courses Sorted by Code:**
```cpp
courses.displayInorder();
// Performs inorder traversal (Left → Root → Right)
// Output: CS101, CS201, CS301, CS401, ENG101, MATH101
// Time: O(n) - visits every node once
```

**3. Enroll in Course (Student):**
```cpp
Course* course = courses.search(code);  // O(log n)
if (course != nullptr) {
    course->enrollStudent();  // O(1)
}
// Fast lookup + instant enrollment
```

**4. Delete Course (Admin):**
```cpp
courses.deleteCourse("CS301");
// 1. Search for node: O(log n)
// 2. Rearrange tree: O(log n)
// 3. Delete node: O(1)
// Total: O(log n)
```

---

### 3. **Stack (LIFO - Last In First Out)**
**File:** `DataStructures.h` (Lines 84-125)

#### Structure:
```
TOP → [Enrollment 3]
         ↓
      [Enrollment 2]
         ↓
      [Enrollment 1]
         ↓
       NULL
```

#### Implementation Details:
- **Template-based**: `Stack<T>` - works with any data type
- **Linked implementation**: Uses internal StackNode structure
- **LIFO principle**: Last enrolled course can be undone first

#### Where Used:
**Undo Functionality** (`System.h` Line 13)
```cpp
Stack<Enrollment> undoStack;
```

#### Operations & Time Complexity:
| Operation | Complexity | Usage |
|-----------|-----------|--------|
| `push()` | O(1) | Record enrollment action |
| `pop()` | O(1) | Retrieve last enrollment for undo |
| `isEmpty()` | O(1) | Check if undo available |

#### How It Helps:
✅ **Instant Undo**: O(1) access to most recent action  
✅ **Simple Rollback**: Easy to reverse last enrollment  
✅ **Memory Efficient**: Only stores enrollment records, not full state  
✅ **Natural Fit**: Stack's LIFO matches undo behavior perfectly  

#### Real Usage Examples:

**1. Enroll in Course (Student):**
```cpp
if (course->enrollStudent()) {
    Enrollment enrollment(currentUser->getUsername(), code);
    enrollments.insert(enrollment);
    undoStack.push(enrollment);  // O(1) - Save for undo
    cout << "Successfully enrolled!\n";
}
```

**2. Undo Last Enrollment (Student):**
```cpp
Enrollment lastEnrollment("", "");
if (undoStack.pop(lastEnrollment)) {  // O(1) - Get last action
    // Remove from enrollments linked list
    enrollments.remove(key, enrollmentComparator);  // O(n)
    // Return seat to course
    course->unenrollStudent();  // O(1)
    cout << "Undo successful!\n";
}
```

**Why Stack is Perfect Here:**
- Students typically want to undo their **most recent** enrollment
- Stack provides O(1) access to the last action
- LIFO matches user expectations (undo last thing I did)
- No need to search through all enrollments

---

### 4. **Vector (Standard Library)**
**File:** `System.h` (Line 6), Used in `System.cpp`

#### Where Used:
**Sorting Courses by Name** (`System.cpp` - `viewAllCourses()` function)

#### Implementation:
```cpp
void CourseRegistrationSystem::viewAllCourses(int sortOption) {
    if (sortOption == 1) {
        vector<Course> courseList;
        collectCourses(courses.getRoot(), courseList);  // BST → Vector
        sortCoursesByName(courseList);  // Sort vector by name
        // Display sorted courses
    }
}
```

#### How It Helps:
✅ **Alternative Sorting**: BST sorts by code, vector allows sorting by name  
✅ **Random Access**: O(1) access to any element  
✅ **STL Algorithm Support**: Can use `std::sort()` with custom comparator  
✅ **Temporary Storage**: Doesn't modify BST structure  

#### Real Usage Example:

**Sort Courses by Name (Student/Admin):**
```cpp
// Step 1: Collect all courses from BST into vector - O(n)
void collectCourses(BSTNode* node, vector<Course>& courseList) {
    if (node != nullptr) {
        collectCourses(node->left, courseList);
        courseList.push_back(node->data);  // O(1) amortized
        collectCourses(node->right, courseList);
    }
}

// Step 2: Sort by name using STL - O(n log n)
sort(courseList.begin(), courseList.end(), 
     [](const Course& a, const Course& b) {
         return a.getName() < b.getName();
     });

// Output: Calculus I, Database Systems, Data Structures...
```

---

## 🎯 Data Structure Selection Rationale

### Why Each Structure Was Chosen:

| Data | Structure | Reason |
|------|-----------|--------|
| **Users** | Linked List | Dynamic size, infrequent searches, sequential display |
| **Courses** | BST | Fast searching, automatic sorting by code, frequent lookups |
| **Enrollments** | Linked List | Many-to-many relationship, need to traverse all |
| **Undo History** | Stack | LIFO matches undo semantics, O(1) operations |
| **Sort by Name** | Vector | Temporary storage, STL support, different sort order |

---

## 📊 Performance Analysis

### User Operations:
| Operation | Primary Structure | Time Complexity | Secondary Operations |
|-----------|------------------|----------------|---------------------|
| Login | LinkedList (users) | O(n) | Password comparison |
| Register | LinkedList (users) | O(n) + O(1) | Search + Insert |
| Delete User | LinkedList (users) | O(n) | Also updates enrollments |

### Course Operations:
| Operation | Primary Structure | Time Complexity | Admin/Student |
|-----------|------------------|----------------|---------------|
| View (by code) | BST (courses) | O(n) | Both |
| View (by name) | Vector + BST | O(n log n) | Both |
| Search | BST (courses) | O(log n) | Both |
| Add Course | BST (courses) | O(log n) | Admin only |
| Update Course | BST (courses) | O(log n) | Admin only |
| Delete Course | BST (courses) | O(log n) | Admin only |

### Enrollment Operations:
| Operation | Structures Used | Time Complexity | Notes |
|-----------|----------------|----------------|-------|
| Enroll | BST + LinkedList + Stack | O(log n) + O(n) + O(1) | Search course + Add enrollment + Push to stack |
| View History | LinkedList (enrollments) | O(n) | Traverse all enrollments |
| Undo | Stack + LinkedList + BST | O(1) + O(n) + O(log n) | Pop + Remove enrollment + Update course |

---

## 🔄 Data Structure Interactions

### Enrollment Flow:
```
1. Student requests to enroll in "CS201"
   ↓
2. BST searches for course (O(log n))
   ↓
3. If found and seats available:
   → LinkedList adds enrollment record (O(n))
   → Stack pushes for undo (O(1))
   → Course object updates seat count (O(1))
```

### Admin Delete Course Flow:
```
1. Admin requests to delete "CS301"
   ↓
2. BST searches for course (O(log n))
   ↓
3. If found:
   → LinkedList removes all enrollments with this course (O(n))
   → BST deletes the course node (O(log n))
```

### Undo Flow:
```
1. Student clicks "Undo Last Action"
   ↓
2. Stack pops last enrollment (O(1))
   ↓
3. LinkedList removes that enrollment (O(n))
   ↓
4. BST finds course and increments seat (O(log n))
```

---

## 💡 Key Design Decisions

### 1. **Generic Linked List vs Separate Classes**
✅ **Chose**: Template-based generic LinkedList  
**Why**: Code reusability - one implementation for Users and Enrollments

### 2. **BST vs Hash Table for Courses**
✅ **Chose**: Binary Search Tree  
**Why**: 
- Natural sorted output (inorder traversal)
- Course codes have lexicographic ordering
- No hash collision handling needed
- O(log n) is sufficient for typical course catalog size

### 3. **Stack vs Queue for Undo**
✅ **Chose**: Stack (LIFO)  
**Why**: Undo should reverse **most recent** action, not oldest (FIFO)

### 4. **Array vs Linked List for Users**
✅ **Chose**: Linked List  
**Why**: 
- Unknown number of users at runtime
- Infrequent insertions/deletions
- No need for random access

---

## 📈 Space Complexity

| Data Structure | Space Used | Notes |
|----------------|------------|-------|
| LinkedList<User> | O(u) | u = number of users |
| BST (courses) | O(c) | c = number of courses |
| LinkedList<Enrollment> | O(e) | e = number of enrollments |
| Stack<Enrollment> | O(s) | s = number of student actions |
| **Total** | **O(u + c + e + s)** | Linear in data size |

---

## 🎓 Learning Outcomes

### Data Structures in Practice:
1. **Linked Lists**: Dynamic storage, sequential access patterns
2. **Binary Search Trees**: Balanced search performance, sorted traversal
3. **Stacks**: LIFO operations, undo/redo mechanisms
4. **Vectors**: Random access, algorithm integration

### Algorithm Design:
- Recursive tree operations (insert, search, delete)
- Linked list traversal patterns
- Stack-based state management
- Custom comparator functions

### Real-World Applications:
- User authentication systems (linked list search)
- Course catalogs (BST for fast lookup)
- Transaction history (stack for rollback)
- Data presentation (multiple sort orders)

---

## 🔧 Code Locations Reference

| Concept | File | Lines |
|---------|------|-------|
| Node Structure | DataStructures.h | 8-13 |
| LinkedList Class | DataStructures.h | 17-81 |
| Stack Class | DataStructures.h | 84-125 |
| BSTNode Structure | DataStructures.h | 127-132 |
| BST Class | DataStructures.h | 134-250 |
| Data Structure Usage | System.h | 10-13 |
| User Model | Models.h | 8-33 |
| Course Model | Models.h | 36-73 |
| Enrollment Model | Models.h | 76-80 |

---

## 🧮 Algorithms Documentation

This section details the specific algorithms implemented in the system, their role, and a comparative analysis with alternative approaches.

### 1. Sorting Algorithms
**Algorithm Used:** **Introsort** (via C++ `std::sort`) / **Merge Sort Logic**
- **Where Used:** Sorting the course list by Name (`System.cpp`).
- **How it Helps:** 
  - It organizes data for better readability and user experience.
  - It guarantees **O(N log N)** time complexity, making it efficient even for a large number of courses.
- **Why this instead of others?**
  - We needed an algorithm faster than O(N²). `std::sort` is highly optimized, using a hybrid of QuickSort, HeapSort, and InsertionSort.
  - It is generally faster than a pure Merge Sort in practice due to better cache locality and no auxiliary memory requirement (unlike Merge Sort's O(N) space).

#### **Alternatives & Comparison:**

| Algorithm | Time Complexity | Space Complexity | Comparison with Used Algorithm |
|-----------|----------------|------------------|--------------------------------|
| **Selection Sort** | O(N²) | O(1) | **Slower.** Selection sort repeatedly scans the unsorted part to find the minimum. For 1000 courses, Selection Sort does ~1,000,000 comparisons, while Merge/Quick Sort does ~10,000. |
| **Bubble Sort** | O(N²) | O(1) | **Slower.** Swaps adjacent elements repeatedly. Very inefficient for large datasets. |
| **Merge Sort** | O(N log N) | O(N) | **Similar Speed.** Merge Sort is "Stable" (preserves order of equal elements) but requires extra memory. Our implementation prioritizes speed and memory efficiency. |

---

### 2. Searching Algorithms
**Algorithm Used:** **Binary Search** (via BST)
- **Where Used:** Searching for a course by its Course Code (`DataStructures.h`).
- **How it Helps:** 
  - Instead of checking every single course, it compares the target code with the root. If smaller, it goes left; if larger, it goes right.
  - Reduces the search space by **half** at every step.
- **Why this instead of others?**
  - Course codes are unique and sortable strings. A BST structure naturally supports Binary Search logic.
  - It reduces lookup time from **O(N)** to **O(log N)**.

#### **Alternatives & Comparison:**

| Algorithm | Time Complexity | Structure Required | Comparison with Used Algorithm |
|-----------|----------------|--------------------|--------------------------------|
| **Linear Search** | O(N) | Array / Linked List | **Slower.** Linear search checks elements one by one. If the course is at the end of the list, it checks everything. Binary search skips half the tree instantly. |
| **Hashing** | O(1) | Hash Table | **Faster.** Hashing is faster on average but doesn't support ordered display (e.g., printing courses A-Z). BST offers a balance of fast search + sorted data. |

---

### 3. String Matching Algorithms
**Algorithm Used:** **Knuth-Morris-Pratt (KMP)**
- **Where Used:** User Authentication (Password Validation) (`DataStructures.h`).
- **How it Helps:** 
  - It pre-processes the pattern (password) to create an LPS (Longest Prefix Suffix) array.
  - If a mismatch occurs, it doesn't go back to the beginning of the text; it skips ahead based on the LPS array.
- **Why this instead of others?**
  - It guarantees **O(N + M)** time complexity (where N is text length, M is pattern length).
  - It avoids the unnecessary backtracking found in the Naive approach.

#### **Alternatives & Comparison:**

| Algorithm | Time Complexity | Comparison with Used Algorithm |
|-----------|----------------|--------------------------------|
| **Naive String Match** | O(N * M) | **Slower.** The Naive approach checks the pattern at every position. In the worst case (e.g., text="AAAAA...", pattern="AAAB"), it performs redundant comparisons. KMP avoids this. |
| **Rabin-Karp** | O(N + M) avg | **Complex.** Uses hashing. Good for multiple pattern matching but prone to hash collisions. KMP is more deterministic for single password checks. |

---

### 4. Graph Traversal Algorithms
**Algorithm Used:** **Depth First Search (DFS) Logic** (Adjacency List Traversal)
- **Where Used:** Prerequisite Checking (`DataStructures.h`).
- **How it Helps:** 
  - It traverses the dependency graph of courses. If Course A requires Course B, and Course B requires Course C, the algorithm follows this chain.
- **Why this instead of others?**
  - We used an **Adjacency List** representation because the graph is sparse (most courses have few prerequisites).
  - DFS is intuitive for detecting chains or cycles (though simple traversal is used here for checking existence).

#### **Alternatives & Comparison:**

| Algorithm | Usage | Comparison with Used Algorithm |
|-----------|-------|--------------------------------|
| **Breadth First Search (BFS)** | Shortest Path | **Different Goal.** BFS explores neighbors layer by layer. DFS explores deep into the prerequisite chain, which fits the "chain of prerequisites" model well. |
| **Adjacency Matrix** | Dense Graphs | **Memory Heavy.** A matrix uses O(V²) space. Since courses have few connections, a Matrix would waste memory storing "zeros" (no relation). List uses O(V + E). |

---

## 🚀 Efficiency Optimizations

### Current Optimizations:
✅ BST for O(log n) course searches vs O(n) linear search  
✅ Stack for O(1) undo vs O(n) history search  
✅ Template classes for code reusability  
✅ Comparator functions for flexible searching  

### Possible Enhancements:
🔹 **Self-Balancing BST** (AVL/Red-Black) - Guarantee O(log n) in worst case  
🔹 **Hash Table for Users** - O(1) login vs O(n) linked list search  
🔹 **Index Structures** - Secondary indices for faster enrollment queries  
🔹 **Caching** - Store frequently accessed courses in memory  

---

## 📝 Summary

This Course Registration System demonstrates **practical application of fundamental data structures**:

- **Linked Lists** provide flexible, dynamic storage for users and enrollments
- **Binary Search Tree** enables efficient course management with automatic sorting
- **Stack** implements intuitive undo functionality with constant-time operations
- **Vector** bridges BST structure with standard library algorithms

Each structure was **deliberately chosen** based on access patterns, performance requirements, and operation frequency, resulting in an efficient and maintainable system that separates admin and student concerns while providing rich functionality.

---

**Project by:**  
- Muhammad Anas (02-134242-068)  
- Muhammad Adil Shabbir (02-134242-033)  
- Amjad Ellahi (02-134242-092)

**Course:** Data Structures and Algorithms  
**Instructor:** Miss Rabia Amjad  
**Date:** December 14, 2025
