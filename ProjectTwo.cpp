//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Jake DeMuesy
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Project Two
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <cctype>
#include <string>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// structure to hold course information
struct Course {
    string courseNumber; // unique identifier here
    string name;
    vector<string> prerequisites;

    Course() {
    }
};

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course) {
    cout << course.courseNumber << ": " << course.name;
    
    if (!course.prerequisites.empty()) {
        
        cout << " | Prerequisites: ";
        
        for (size_t i = 0; i < course.prerequisites.size(); i++) {
            
            cout << course.prerequisites[i];

            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
    }

    cout << endl;
    return;
}

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;
    int size;  // Added size tracking variable

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void postOrder(Node* node);
    void preOrder(Node* node);
    Node* removeNode(Node* node, string courseNumber);
    void destroyTree(Node* node);  // Added helper method for destructor

public:
    BinarySearchTree();

    virtual ~BinarySearchTree();
    void InOrder();
    void PostOrder();
    void PreOrder();
    void Insert(Course course);
    void Remove(string courseNumber);
    Course Search(string courseNumber);
    int Size();  // Added size method
    bool IsEmpty();  // Added isEmpty method
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // Initialize housekeeping variables
    root = nullptr;
    size = 0;  // Initialize size to 0
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // Recursively delete the tree using post-order traversal
    destroyTree(root);
}

/**
 * Helper method to recursively delete all nodes
 */
void BinarySearchTree::destroyTree(Node* node) {
    if (node != nullptr) {
        destroyTree(node->left);   // Delete left subtree
        destroyTree(node->right);  // Delete right subtree
        delete node;               // Delete current node
    }
}

/**
 * Returns the size of the tree
 */
int BinarySearchTree::Size() {
    return size;
}

/**
 * Check if the tree is empty
 */
bool BinarySearchTree::IsEmpty() {
    return (root == nullptr);
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    inOrder(root); // call inOrder function and pass root 
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
    postOrder(root); // postOrder root
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
    preOrder(root); // preOrder root
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) { // if root equal to nullptr
        root = new Node(course); // root is equal to new node course
        size++;  // Increment size counter
    }
    else {
        addNode(root, course); // add Node root and course
    }
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseNumber) {
    if (root != nullptr) {
        // If root matches the course to be deleted, update size
        if (root->course.courseNumber == courseNumber) {
            size--;  // Decrement size counter
        }
        root = removeNode(root, courseNumber);
    }
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseNumber) {
    Node* currentNode = root; // set current node equal to root

    while (currentNode != nullptr) // keep looping downwards until bottom reached or matching courseNumber found
    {
        transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper); //ensure correct validation on user input

        if (currentNode->course.courseNumber == courseNumber) {
            return currentNode->course; // if match found, return current course
        }
        else if (courseNumber < currentNode->course.courseNumber) {
            currentNode = currentNode->left; // if course is smaller than current node then traverse left
        }
        else {
            // else larger so traverse right
            currentNode = currentNode->right;
        }
    }

    Course course;  // Return empty course if not found
    return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    if (course.courseNumber < node->course.courseNumber) // if node is larger then add to left
    {
        if (node->left == nullptr) // if no left node
        {
            node->left = new Node(course); // this node becomes left
            size++;  // Increment size counter
        }
        else {
            addNode(node->left, course); // else recurse down the left node
        }
    }
    else if (course.courseNumber > node->course.courseNumber) {  // Handle duplicates
        if (node->right == nullptr) // if no right node
        {
            node->right = new Node(course); // this node becomes right
            size++;  // Increment size counter
        }
        else {
            addNode(node->right, course); // recurse down the right node
        }
    }
    // If courseNumber is equal, it's a duplicate - do nothing
}

void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) //if node is not equal to nullptr
    {
        inOrder(node->left); //InOrder left
        displayCourse(node->course); // output course info
        inOrder(node->right); //InOrder right
    }
}

void BinarySearchTree::postOrder(Node* node) {
    if (node != nullptr) { //if node is not equal to nullptr
        postOrder(node->left); //postOrder left
        postOrder(node->right); //postOrder right
        displayCourse(node->course); //output course info
    }
}

void BinarySearchTree::preOrder(Node* node) {
    if (node != nullptr) //if node is not equal to nullptr
    {
        displayCourse(node->course); //output course info
        preOrder(node->left); //preOrder left
        preOrder(node->right); //preOrder right    
    }
}

/**
 * Remove a course from some node (recursive)
 */
Node* BinarySearchTree::removeNode(Node* node, string courseNumber) {
    if (node == nullptr) {
        return node; // if nullptr return node
    }

    if (courseNumber < node->course.courseNumber) {
        node->left = removeNode(node->left, courseNumber);
    }
    else if (courseNumber > node->course.courseNumber) {
        node->right = removeNode(node->right, courseNumber);
    }
    else {
        //Leaf node (no children)
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
            return node;
        }
        //Node with only one child
        else if (node->left == nullptr) {
            Node* tempNode = node;
            node = node->right;
            delete tempNode;
            return node;
        }
        else if (node->right == nullptr) {
            Node* tempNode = node;
            node = node->left;
            delete tempNode;
            return node;
        }
        //Node with two children
        else {
            Node* tempNode = node->right; // Find the min value in right subtree
            
			while (tempNode->left != nullptr) {
                tempNode = tempNode->left;
            }
            
            node->course = tempNode->course; // Copy the successor's data to this node
            
            node->right = removeNode(node->right, tempNode->course.courseNumber); // Delete the successor
        }
    }

    return node;
}

/**
 * Load courses from a file
 *
 * @param filePath the path to the file to load
 * @param bst pointer to the binary search tree
 * @return number of courses read
 */
int loadCourses(string filePath, BinarySearchTree* bst) {
    
	ifstream file(filePath);
    
	if (!file.is_open()) {
        cerr << "Error: Could not open file " << filePath << endl;
        return 0;
    }

    int count = 0;
    string line;

    // Read each line from the file
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        vector<string> tokens;

        // Parse comma-separated values
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        // Ensure we have at least courseNumber and name
        if (tokens.size() >= 2) {
            Course course;
            course.courseNumber = tokens[0];
            course.name = tokens[1];

            // Add prerequisites if they exist
            for (size_t i = 2; i < tokens.size(); i++) {
                course.prerequisites.push_back(tokens[i]);
            }

            // Add course to the BST
            bst->Insert(course);
            count++;
        }
    }

    file.close();
    return count;
}

/**
 * Main Method
 */
int main(int argc, char* argv[]) {
    // Process command line arguments
    string csvPath;

    switch (argc) {
    
    case 2:
        csvPath = argv[1];
        break;
    
    default:
        csvPath = "CS 300 ABCU_Advising_Program_Input.csv";  // Default file name
    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all courses
    BinarySearchTree* bst = new BinarySearchTree();
    Course course;
    bool coursesLoaded = false;  // Flag to track if courses have been loaded

    int userInput = 0;
    int numCourses = 0;
    string courseId;

    while (userInput != 9) {
        cout << "\nMenu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  4. Remove Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        
        //Input validation added
        if (!(cin >> userInput)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input

            cout << "Invalid input. Please enter an appropriate selection";
        }
        
        switch (userInput) {
        case 1:
            // Initialize a timer variable before loading courses
            ticks = clock();

            // Load courses from file
            numCourses = loadCourses(csvPath, bst);

            if (numCourses > 0)
            {
                coursesLoaded = true;
                cout << numCourses << " courses read" << endl;
            }
            else {
                cout << "No courses were loaded. Please check the file path: " << csvPath << endl;
            }

            // Calculate elapsed time and display result
            ticks = clock() - ticks;
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            if (!coursesLoaded || bst->IsEmpty())
            {
                loadCourses(csvPath, bst);
                //cout << "\nError: No courses have been loaded. Please load courses first (Option 1)." << endl;
            }
			cout << "\nCourse List (sorted by course number):" << endl;
			bst->InOrder();
			cout << "\nTotal number of courses: " << bst->Size() << endl;

            break;

        case 3:
            cout << "\nEnter course number: ";
            cin >> courseId;

            ticks = clock();
            course = bst->Search(courseId);
            ticks = clock() - ticks;

            if (!course.courseNumber.empty()) {
                displayCourse(course);
            }
            else {
                cout << "Course " << courseId << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            cout << "\nEnter course number to remove: ";
            
            cin >> courseId;
            bst->Remove(courseId);

            cout << "Course " << courseId << " removed (if it existed)." << endl;
            break;
        
        case 9:
            break;

        default:
            cout << userInput << " is not a valid option.";
            break;
        }
    }

    cout << "\nThank you for using the course planner!" << endl;
    
    delete bst; // Clean up

    return 0;
}