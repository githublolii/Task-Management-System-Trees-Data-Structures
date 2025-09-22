# Task Management System (BST-based)

This project implements a Task Management System in C, utilizing a Binary Search Tree (BST) to store and manage tasks. It provides functionalities for loading, adding, deleting, searching, performing tasks, viewing tasks based on performance status and sorting criteria, restructuring the BST, and viewing tree information.

## Features

*   **Load Tasks from File:** Load initial tasks from a specified text file (`tasks.txt`).
*   **Add New Task:** Create and add a new task with an ID, name, date, and duration into the BST.
*   **Delete Task:** Remove a task from the BST by its ID.
*   **Search for Task:** Find tasks by either their ID or name.
*   **Perform Task:** Mark a task as "performed" without removing it from the tree.
*   **View Unperformed Tasks:** Display all tasks that have not yet been marked as performed (sorted by name).
*   **View Performed Tasks:** Display all tasks that have been marked as performed (sorted by ID).
*   **Restructure BST:** Rebuild the BST based on different sorting criteria (ID, Name, Date, Duration).
*   **View BST Info:** Display the height, number of leaves, internal nodes, size of the tree, and current sorting criterion.
*   **Duplicate ID Check:** Prevents adding tasks with an ID that already exists.
*   **Memory Management:** Proper allocation and deallocation of memory for tasks.

## Data Structure

The system uses a Binary Search Tree (BST) where each node represents a task.

```c
typedef struct Task {
    int id;
    char name[SIZE];
    char date[DATE_LENGTH];
    float duration;
    int IsPerformed; // Flag to indicate if a task is performed (0 for unperformed, 1 for performed)
    struct Task *left;
    struct Task *right;
} Task;

typedef struct Tree {
    int height;
    int size;
    int leaves;
    int internalNodes;
    int sortBy; // 0: ID, 1: Name, 2: Date, 3: Duration
} TreeInfo;
##How to Compile and Run
##Prerequisites
A C compiler (e.g., GCC)
##Compilation
Save the provided C code as main.c.
Open a terminal or command prompt.
Navigate to the directory where you saved main.c.
Compile the code using a C compiler:
code
Bash
gcc main.c -o task_bst_manager
##Running the Program
After successful compilation, run the executable:
code
Bash
./task_bst_manager
The program will present a menu of options. Follow the prompts to interact with the task management system.
##tasks.txt File Format
The loadTasks function expects a tasks.txt file with each task on a new line, using # as a delimiter:
code
Code
ID#Task Name#Date (D/M/Y)#Duration
Example tasks.txt:
code
Code
101#Prepare presentation#01/06/2024#2.5
102#Review code changes#03/06/2024#4.0
103#Schedule meeting#30/05/2024#0.5
104#Research new tech#05/06/2024#1.5
##Functions Overview
Menu(): Displays the main menu options to the user.
loadTasks(Task *root, TreeInfo* info, const char *fileName): Loads tasks from a file and inserts them into the BST.
createTask(int id, char *name, char *date, float duration): Allocates memory and initializes a new Task node.
addTask(Task *root, Task* newTask, TreeInfo* info, int currentHeight): Inserts a new task into the BST based on the current sortBy criterion.
deleteTask(Task *root, int id, TreeInfo* info): Deletes a task by ID from the BST.
printTree(Task *root): Performs an in-order traversal to print all tasks in the BST.
searchById(Task *root, int id): Searches for a task by its ID.
searchTaskByName(Task *root, char name[]): Searches for a task by its name (requires restructuring by name).
performTask(Task *root, int id): Marks a task as performed.
viewUnperformedByName(Task *root): Traverses the tree to find and print unperformed tasks (sorted by name if the tree is structured this way).
viewPerformedById(Task *root): Traverses the tree to find and print performed tasks (sorted by ID if the tree is structured this way).
restructureBST(Task *root, TreeInfo* info): Rebuilds the BST based on a new sorting criterion.
addToNewTree(Task* task, Task* newRoot, TreeInfo* info): Helper for restructureBST, recursively adds tasks to a new tree.
viewBSTInfo(TreeInfo* info): Displays current information about the BST (height, size, leaves, etc.).
printTask(Task *task): Helper function to print a single task's details.
destroy(Task *root): Frees all allocated memory in the BST.
checkDuplicateId(Task *root, int id): Checks if a task with the given ID already exists in the BST.
dateToNumber(char* date): Converts a date string (D/M/Y) into an integer for comparison.
checkTreeInfo(Task* root, int currentHeight, TreeInfo* info): Recursively calculates and updates TreeInfo fields like height, leaves, and internal nodes.
##Author
Laila Awad - 1230085 - Section 4
