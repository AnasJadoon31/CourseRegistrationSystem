#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "Models.h"
#include <iostream>
#include <vector>

// Node for Linked List (for User storage)
template <typename T>
struct Node {
    T data;
    Node* next;
    explicit Node(T d) : data(d), next(nullptr) {}
};

// Linked List class
template <typename T>
class LinkedList {
private:
    Node<T>* head;

public:
    LinkedList() : head(nullptr) {}

    ~LinkedList() {
        Node<T>* current = head;
        while (current != nullptr) {
            Node<T>* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void insert(T data) {
        auto* newNode = new Node<T>(data);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node<T>* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    Node<T>* getHead() { return head; }

    T* search(string key, bool (*comparator)(const T&, const string&)) {
        Node<T>* current = head;
        while (current != nullptr) {
            if (comparator(current->data, key)) {
                return &(current->data);
            }
            current = current->next;
        }
        return nullptr;
    }

    bool remove(string key, bool (*comparator)(const T&, const string&)) {
        if (head == nullptr) return false;

        if (comparator(head->data, key)) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            return true;
        }

        Node<T>* current = head;
        while (current->next != nullptr) {
            if (comparator(current->next->data, key)) {
                Node<T>* temp = current->next;
                current->next = current->next->next;
                delete temp;
                return true;
            }
            current = current->next;
        }
        return false;
    }
};

// Stack for Undo functionality
template <typename T>
class Stack {
private:
    struct StackNode {
        T data;
        StackNode* next;
        explicit StackNode(T d) : data(d), next(nullptr) {}
    };
    StackNode* top;

public:
    Stack() : top(nullptr) {}

    ~Stack() {
        while (top != nullptr) {
            StackNode* temp = top;
            top = top->next;
            delete temp;
        }
    }

    void push(T data) {
        auto* newNode = new StackNode(data);
        newNode->next = top;
        top = newNode;
    }

    bool pop(T& data) {
        if (top == nullptr) return false;
        data = top->data;
        StackNode* temp = top;
        top = top->next;
        delete temp;
        return true;
    }

    bool isEmpty() {
        return top == nullptr;
    }
};

// Queue for Waitlist
template <typename T>
class Queue {
private:
    struct QueueNode {
        T data;
        QueueNode* next;
        explicit QueueNode(T d) : data(d), next(nullptr) {}
    };
    QueueNode* front;
    QueueNode* rear;

public:
    Queue() : front(nullptr), rear(nullptr) {}

    ~Queue() {
        while (front != nullptr) {
            QueueNode* temp = front;
            front = front->next;
            delete temp;
        }
    }

    void enqueue(T data) {
        auto* newNode = new QueueNode(data);
        if (rear == nullptr) {
            front = rear = newNode;
            return;
        }
        rear->next = newNode;
        rear = newNode;
    }

    bool dequeue(T& data) {
        if (front == nullptr) return false;
        QueueNode* temp = front;
        data = temp->data;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete temp;
        return true;
    }

    bool isEmpty() {
        return front == nullptr;
    }
};

// Hash Table for User/Payment lookup
template <typename T>
class HashTable {
private:
    static const int TABLE_SIZE = 100;
    struct HashNode {
        string key;
        T value;
        HashNode* next;
        HashNode(string k, T v) : key(std::move(k)), value(v), next(nullptr) {}
    };
    HashNode* table[TABLE_SIZE];

    int hashFunction(const string& key) {
        int hash = 0;
        for (char c : key) {
            hash = (hash * 31 + c) % TABLE_SIZE;
        }
        return hash;
    }

public:
    HashTable() {
        for (auto & i : table) i = nullptr;
    }

    ~HashTable() {
        for (auto & current : table) {
            while (current != nullptr) {
                HashNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    void insert(string key, T value) {
        int index = hashFunction(key);
        auto* newNode = new HashNode(std::move(key), value);
        newNode->next = table[index];
        table[index] = newNode;
    }

    T* search(string key) {
        int index = hashFunction(key);
        HashNode* current = table[index];
        while (current != nullptr) {
            if (current->key == key) return &(current->value);
            current = current->next;
        }
        return nullptr;
    }
};

// BST Node for Course storage
struct BSTNode {
    Course data;
    BSTNode* left;
    BSTNode* right;
    explicit BSTNode(Course c) : data(std::move(c)), left(nullptr), right(nullptr) {}
};

// Binary Search Tree for Courses (sorted by code)
class BST {
private:
    BSTNode* root;

    static BSTNode* searchHelper(BSTNode* node, const string& code) {
        while (node != nullptr) {
            if (node->data.getCode() == code) {
                return node;
            }
            if (code < node->data.getCode()) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return nullptr;
    }

    static BSTNode* findMin(BSTNode* node) {
        while (node && node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

public:
    BST() : root(nullptr) {}

    ~BST() {
        if (root == nullptr) return;

        std::vector<BSTNode*> stack;
        stack.push_back(root);

        while (!stack.empty()) {
            BSTNode* node = stack.back();
            stack.pop_back();

            if (node->left) stack.push_back(node->left);
            if (node->right) stack.push_back(node->right);

            delete node;
        }
    }

    void insert(const Course& course) {
        if (root == nullptr) {
            root = new BSTNode(course);
            return;
        }
        BSTNode* current = root;
        while (true) {
            if (course.getCode() < current->data.getCode()) {
                if (current->left == nullptr) {
                    current->left = new BSTNode(course);
                    break;
                }
                current = current->left;
            } else if (course.getCode() > current->data.getCode()) {
                if (current->right == nullptr) {
                    current->right = new BSTNode(course);
                    break;
                }
                current = current->right;
            } else {
                break; // Duplicate
            }
        }
    }

    Course* search(const string& code) {
        BSTNode* result = searchHelper(root, code);
        return result ? &(result->data) : nullptr;
    }

    void displayInorder() {
        if (root == nullptr) {
            cout << "No courses available.\n";
            return;
        }

        std::vector<BSTNode*> stack;
        BSTNode* curr = root;

        while (curr != nullptr || !stack.empty()) {
            while (curr != nullptr) {
                stack.push_back(curr);
                curr = curr->left;
            }
            curr = stack.back();
            stack.pop_back();

            cout << "Code: " << curr->data.getCode()
                 << " | Name: " << curr->data.getName()
                 << " | Credit Hours: " << curr->data.getCreditHours()
                 << " | Available Seats: " << curr->data.getAvailableSeats()
                 << "/" << curr->data.getTotalSeats() << "\n";

            curr = curr->right;
        }
    }

    bool deleteCourse(const string& code) {
        if (root == nullptr) return false;

        BSTNode* parent = nullptr;
        BSTNode* current = root;

        // Find node
        while (current != nullptr && current->data.getCode() != code) {
            parent = current;
            if (code < current->data.getCode()) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        if (current == nullptr) return false; // Not found

        // Case 1: Node has two children
        if (current->left != nullptr && current->right != nullptr) {
            BSTNode* successorParent = current;
            BSTNode* succ = current->right;
            while (succ->left != nullptr) {
                successorParent = succ;
                succ = succ->left;
            }

            current->data = succ->data;

            // Now delete succ (which has at most one child, right child)
            current = succ;
            parent = successorParent;
        }

        // Now current has at most one child
        BSTNode* child = (current->left != nullptr) ? current->left : current->right;

        if (parent == nullptr) {
            root = child;
        } else {
            if (parent->left == current) {
                parent->left = child;
            } else {
                parent->right = child;
            }
        }

        delete current;
        return true;
    }

    BSTNode* getRoot() { return root; }
};

// Graph for Prerequisites
class Graph {
private:
    struct AdjListNode {
        string dest;
        AdjListNode* next;
        explicit AdjListNode(string d) : dest(std::move(d)), next(nullptr) {}
    };

    struct GraphNode {
        string courseCode;
        AdjListNode* head; // List of prerequisites
        GraphNode* next;
        explicit GraphNode(string c) : courseCode(std::move(c)), head(nullptr), next(nullptr) {}
    };

    GraphNode* head;

public:
    Graph() : head(nullptr) {}

    ~Graph() {
        GraphNode* current = head;
        while (current != nullptr) {
            GraphNode* temp = current;
            AdjListNode* adj = current->head;
            while (adj != nullptr) {
                AdjListNode* tempAdj = adj;
                adj = adj->next;
                delete tempAdj;
            }
            current = current->next;
            delete temp;
        }
    }

    // Check if a prerequisite already exists for a course
    bool hasPrerequisite(const string& course, const string& prereq) {
        GraphNode* node = findNode(course);
        if (node) {
            AdjListNode* curr = node->head;
            while (curr != nullptr) {
                if (curr->dest == prereq) {
                    return true;
                }
                curr = curr->next;
            }
        }
        return false;
    }

    // Returns true if added successfully, false if duplicate
    bool addPrerequisite(const string& course, const string& prereq) {
        // Check for duplicate prerequisite
        if (hasPrerequisite(course, prereq)) {
            return false;
        }

        // Check for self-reference (course cannot be its own prerequisite)
        if (course == prereq) {
            return false;
        }

        GraphNode* node = findNode(course);
        if (!node) {
            node = new GraphNode(course);
            node->next = head;
            head = node;
        }

        auto* newNode = new AdjListNode(prereq);
        newNode->next = node->head;
        node->head = newNode;
        return true;
    }

    GraphNode* findNode(const string& course) {
        GraphNode* current = head;
        while (current != nullptr) {
            if (current->courseCode == course) return current;
            current = current->next;
        }
        return nullptr;
    }

    vector<string> getPrerequisites(const string& course) {
        vector<string> prereqs;
        GraphNode* node = findNode(course);
        if (node) {
            AdjListNode* curr = node->head;
            while (curr != nullptr) {
                prereqs.push_back(curr->dest);
                curr = curr->next;
            }
        }
        return prereqs;
    }
};

// KMP Algorithm for String Matching
inline bool kmpSearch(string text, string pattern) {
    int n = static_cast<int>(text.length());
    int m = static_cast<int>(pattern.length());
    if (m == 0) return true;
    if (n != m) return false; // Passwords must be exact match in length
    if (text == pattern) return true; // Optimization for exact match

    // Compute LPS array
    int* lps = new int[m];
    int len = 0;
    lps[0] = 0;
    int i = 1;
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }

    // Search
    i = 0;
    int j = 0;
    bool found = false;
    while (i < n) {
        if (pattern[j] == text[i]) {
            j++;
            i++;
        }
        if (j == m) {
            found = true;
            break;
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0)
                j = lps[j - 1];
            else
                i = i + 1;
        }
    }
    delete[] lps;
    return found;
}

#endif

