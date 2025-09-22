#// Laila Awad - 1230085 - Section 4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 60
#define DATE_LENGTH 20

#define SORT_BY_ID 0
#define SORT_BY_NAME 1

typedef struct Task {
    int id;
    char name[SIZE];
    char date[DATE_LENGTH];
    float duration;
    int IsPerformed;
    struct Task *left;
    struct Task *right;
} Task;

typedef struct Tree {
    int height;
    int size;
    int leaves;
    int internalNodes;
    int sortBy;
} TreeInfo;

int isLoaded = 0;
void Menu();
Task* addTask(Task *root, Task* newTask, TreeInfo* info, int currentHeight);
Task* createTask(int id, char *name, char *date, float duration);
Task* deleteTask(Task *root, int id, TreeInfo* info);
Task* searchById(Task *root, int id);
Task* searchTaskByName(Task *root, char name[]);
Task* loadTasks(Task *root, TreeInfo* info, const char *fileName);
void printTree(Task* root);
void addToNewTree(Task* task, Task* newRoot, TreeInfo* info);
int dateToNumber(char* date);
int checkTreeInfo(Task* root, int currentHeight, TreeInfo* info);

int viewPerformedById(Task *task);
int viewUnperformedByName(Task *task);

void performTask(Task *root, int id);
void restructureBST(Task *root, TreeInfo* info);
void viewBSTInfo(TreeInfo* info);

void printTask(Task *task);
void destroy(Task *root);
int checkDuplicateId(Task *root, int id);


int main() {
    int id, choice;
    char name[SIZE];
    char date[DATE_LENGTH];
    float duration;
    Task *root = NULL;
    TreeInfo* info = (TreeInfo*) malloc(sizeof(TreeInfo));

    info->height = 0;
    info->leaves = 0;
    info->internalNodes = 0;
    info->size = 0;
    info->sortBy = 0; // by id

    printf("Hello! Welcome to My Task Management System.\n");
    printf("Please Enter number of the wanted option: \n");

    while(1) {
        Menu();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
        case 1:
            if(info->sortBy != SORT_BY_ID) {
                info->sortBy = SORT_BY_ID;
                restructureBST(root, info);
            }

            root = loadTasks(root, info, "tasks.txt");
            break;

        case 2:
            printf("Enter Task ID: ");
            scanf("%d", &id);

            if(info->sortBy != SORT_BY_ID) {
                info->sortBy = SORT_BY_ID;
                restructureBST(root, info);
            }

            if(checkDuplicateId(root, id)) {
                printf("Task ID %d already exists!.\n", id);
                break;
            }

            printf("Enter Task Name: ");
            getchar();
            fgets(name, sizeof(name), stdin);
            strcpy(name, strtok(name, "\n"));

            printf("Enter Task Date (D/M/Y): ");
            fgets(date, sizeof(date), stdin);
            strcpy(date, strtok(date, "\n"));

            printf("Enter Task Duration in hours: ");
            scanf("%f", &duration);

            Task* task = createTask(id, name, date, duration);

            if(task) {
                root = addTask(root, task, info, 0);
                printf("Task %d added successfully!\n", id);
            }

            break;

        case 3:
            printf("Enter Task ID to delete: ");
            scanf("%d", &id);

            if(info->sortBy != SORT_BY_ID) {
                info->sortBy = SORT_BY_ID;
                restructureBST(root, info);
            }

            root = deleteTask(root, id, info);
            info->height = 0;
            info->internalNodes = 0;
            info->leaves = 0;
            info->size = 0;
            info->height = checkTreeInfo(root, 0, info);
            break;

        case 4:
            printf("Enter Task Name to search: ");
            getchar();
            fgets(name, sizeof(name), stdin);
            strcpy(name, strtok(name, "\n"));

            if(info->sortBy != SORT_BY_NAME) {
                info->sortBy = SORT_BY_NAME;
                restructureBST(root, info);
            }

            Task *searchedTask = searchTaskByName(root, name);

            if(searchedTask != NULL) {
                printf("Task with name %s details: ", name);
                printTask(searchedTask);
            } else
                printf("Task not found!\n");

            break;

        case 5:
            printf("Enter Task ID to perform: ");
            scanf("%d", &id);

            if(info->sortBy != SORT_BY_ID) {
                info->sortBy = SORT_BY_ID;
                restructureBST(root, info);
            }

            performTask(root, id);
            break;

        case 6:
            if(info->sortBy != SORT_BY_NAME) {
                info->sortBy = SORT_BY_NAME;
                restructureBST(root, info);
            }

            int found1 = viewUnperformedByName(root);

            if(found1) {
                printf("\nFound %d unperformed tasks (sorted by name).\n", found1);
            } else
                printf("There are no performed tasks.\n");

            break;

        case 7:
            if(info->sortBy != SORT_BY_ID) {
                info->sortBy = SORT_BY_ID;
                restructureBST(root, info);
            }

            int found2 = viewPerformedById(root);

            if(found2)
                printf("\nFound %d performed tasks (sorted by id).\n", found2);
            else
                printf("There are no performed tasks.\n");

            break;

        case 8:
            printf("Restructure BST by: 1)Name 2)Date 3)Duration: ");
            int restructureChoice;
            scanf("%d", &restructureChoice);

            if(info->sortBy != restructureChoice) {
                info->sortBy = restructureChoice;
                restructureBST(root, info);
            }

            printf("Tree got restructured.\n");
            break;

        case 9:
            printf("\nTree information: \n");
            viewBSTInfo(info);
            break;

        case 10:
            printTree(root);
            break;

        case 11:
            printf("Exiting the program.\n");
            destroy(root);
            return 0;

        default:
            printf("Invalid choice. Please pick a valid one.\n");
        }
    }

    return 0;
}

void Menu() {
    printf("\nMenu Options:\n");
    printf("1. Load Tasks File\n");
    printf("2. Add a New Task\n");
    printf("3. Delete a Task\n");
    printf("4. Search for a Task\n");
    printf("5. Perform a Task\n");
    printf("6. View Unperformed Tasks\n");
    printf("7. View Performed Tasks\n");
    printf("8. Restructure the Tasks BST\n");
    printf("9. View BST Info\n");
    printf("10. Print Tree\n");
    printf("11. Exit\n");
}
Task* loadTasks(Task *root, TreeInfo* info, const char *fileName) {
    if(isLoaded) {
        printf("File already loaded!\n");
        return root;
    }

    FILE *file = fopen("tasks.txt", "r");

    if(file == NULL) {
        printf("Task list is empty.\n");

    }

    int id;
    float duration;
    char name[SIZE], date[DATE_LENGTH];

    int c = '1';
    int n = 0;
    float temp = 0;

    int length = 200;
    char buffer[length];
    char* token;

    while(fgets(buffer, length, file) != NULL) {
        if(buffer[0] == '\0' || buffer[0] == '\n')
            continue;

        token = strtok(buffer, "#");
        c = token[0];
        id = 0;
        n = 0;

        while(c != '\0') {
            id *= 10;
            id += c - '0';
            n++;
            c = token[n];
        }

        if(checkDuplicateId(root, id)) {
            printf("Skipping duplicate with id: %d\n", id);
            continue;
        }

        token = strtok(NULL, "#");

        strcpy(name, token);

        token = strtok(NULL, "#");

        strcpy(date, token);

        token = strtok(NULL, "#");
        c = token[0];
        duration = 0;
        n = 0;

        while(c != '\0' && c != '.' && c != '\n') {
            duration *= 10.0;
            duration += c - '0';
            n++;
            c = token[n];
        }

        if(c == '.') {
            token = strtok(token, ".");
            token = strtok(NULL, ".");
            c = token[0];
            n = 0;
            temp = 1.0;

            while(c != '\0' && c != '\n') {
                temp *= 10.0;
                duration += ((c - '0') / temp);
                n++;
                c = token[n];
            }
        }

        Task* task = createTask(id, name, date, duration);

        if(task)
            root = addTask(root, task, info, 0);
    }

    fclose(file);
    isLoaded = 1;

    if(root != NULL)
        printf("Tasks successfully loaded!\n");
    else
        printf("File is empty!\n");

    return root;
}

Task* createTask(int id, char *name, char *date, float duration) {
    Task *newTask = (Task *)malloc(sizeof(Task));

    if(newTask == NULL) {
        printf("Couldn't allocate memory\n");
        return NULL;
    }

    newTask->id = id;
    strcpy(newTask->name, name);
    strcpy(newTask->date, date);
    newTask->duration = duration;
    newTask->IsPerformed = 0;
    newTask->left = newTask->right = NULL;

    return newTask;
}

Task* addTask(Task *root, Task* newTask, TreeInfo* info, int currentHeight) {
    if(root == NULL) {
        info->leaves++;
        info->size++;
        return newTask;
    }

    int cmp = 0;

    switch(info->sortBy) {
    case 0:
        cmp = newTask->id < root->id;
        break;

    case 1:
        cmp = strcmp(root->name, newTask->name);
        break;

    case 2:
        cmp = dateToNumber(root->date) > dateToNumber(newTask->date);
        break;

    case 3:
        cmp =  newTask->duration < root->duration;
        break;
    }

    if(cmp > 0) {
        root->left = addTask(root->left, newTask, info, ++currentHeight);

        if(root->left == newTask && root->right == NULL) {
            info->leaves--;
            info->internalNodes++;

            if(info->height < currentHeight)
                info->height++;
        }

    } else {
        root->right = addTask(root->right, newTask, info, ++currentHeight);

        if(root->right == newTask && root->left == NULL) {
            info->leaves--;
            info->internalNodes++;

            if(info->height < currentHeight)
                info->height++;
        }
    }

    return root;
}

Task* deleteTask(Task *root, int id, TreeInfo* info) {

    Task *temp;

    if(root == NULL) {
        printf("Task with ID %d not found.\n", id);
    } else if(id < root->id) {
        root->left = deleteTask(root->left, id, info);
    } else if(id > root->id) {
        root->right = deleteTask(root->right, id, info);
    } else {
        if(root->left && root->right) {
            temp = root->right;

            while(temp->left != NULL) {
                temp = temp->left;
            }

            root->id = temp->id;
            strcpy(root->name, temp->name);
            strcpy(root->date, temp->date);
            root->duration = temp->duration;

            root->right = deleteTask(root->right, root->id, info);
        } else {
            temp = root;

            if(root->left == NULL) {
                root = root->right;
            } else if(root->right == NULL) {
                root = root->left;
            }

            printf("Task with ID %d deleted.\n", temp->id);
            free(temp);
        }
    }

    return root;
}

void printTree(Task *root) {
    if(root == NULL)
        return;

    printTree(root->left);
    printTask(root);
    printTree(root->right);
}

Task* searchById(Task *root, int id) {
    if(root == NULL || root->id == id)
        return root;

    if(id < root->id)
        return searchById(root->left, id);
    else
        return searchById(root->right, id);
}

Task* searchTaskByName(Task *root, char name[]) {
    if(root == NULL)
        return NULL;

    int cmp = strcmp(name, root->name);

    if(cmp == 0)
        return root;
    else if(cmp < 0)
        return searchTaskByName(root->left, name);
    else
        return searchTaskByName(root->right, name);
}

void performTask(Task *root, int id) {
    Task *task = searchById(root, id);

    if(task != NULL) {
        if(!task->IsPerformed) {
            task->IsPerformed = 1;
            printf("Task with ID %d is marked as performed.\n", id);
        } else {
            printf("Task already performed.\n");
        }
    } else {
        printf("Task %d not found.\n", id);
    }
}
int viewUnperformedByName(Task *root) {
    if(root == NULL)
        return 0;

    int found = 0;
    found += viewUnperformedByName(root->left);

    if(!root->IsPerformed) {
        printTask(root);
        found += 1;
    }

    found += viewUnperformedByName(root->right);

    return found;
}

int viewPerformedById(Task *root) {
    if(root == NULL)
        return 0;

    int found = 0;

    found += viewPerformedById(root->left);

    if(root->IsPerformed) {
        printTask(root);
        found += 1;
    }

    found += viewPerformedById(root->right);

    return found;
}



void restructureBST(Task *root, TreeInfo* info) {
    if(root == NULL)
        return;

    Task* left = root->left;
    Task* right = root->right;
    root->left = NULL;
    root->right = NULL;
    info->height = 0;
    info->internalNodes = 0;
    info->size = 0;
    info->leaves = 0;

    addToNewTree(left, root, info);
    addToNewTree(right, root, info);
}

void addToNewTree(Task* task, Task* newRoot, TreeInfo* info) {
    if(task == NULL)
        return;

    Task* left = task->left;
    Task* right = task->right;
    task->left = NULL;
    task->right = NULL;
    addTask(newRoot, task, info, 0);
    addToNewTree(left, newRoot, info);
    addToNewTree(right, newRoot, info);
}

void viewBSTInfo(TreeInfo* info) {
    printf("Tree height: %d\n", info->height);
    printf("Tree leaves: %d\n", info->leaves);
    printf("Tree internal nodes: %d\n", info->internalNodes);
    printf("Tree size: %d\n", info->size);
    printf("Tree is sorted by: ");

    switch(info->sortBy) {
    case 0:
        printf("id");
        break;

    case 1:
        printf("name");
        break;

    case 2:
        printf("date");
        break;

    case 3:
        printf("duration");
        break;

    default:
        printf("id");
        break;
    }

    printf("\n");
}


void printTask(Task *task) {
    if(task != NULL) {
        printf("ID: %d \n Name: %s \n Date: %s \n Duration: %.1f hours \n",
               task->id, task->name, task->date, task->duration);
    }
}

void destroy(Task *root) {
    if(root == NULL)
        return;

    destroy(root->left);
    destroy(root->right);
    free(root);


}

int checkDuplicateId(Task *root, int id) {
    if(root == NULL) {
        return 0;
    }

    while(root != NULL) {
        if(root->id == id) {
            return 1;
        } else if(id < root->id) {
            root = root->left;
        } else {
            root = root->right;
        }
    }

}

int dateToNumber(char* date) {
    char dateCopy[DATE_LENGTH];
    strcpy(dateCopy, date);
    char* token = strtok(dateCopy, "/");
    char c = '1';
    int n = 0;
    int day = 0;

    c = token[0];
    n = 0;

    while(c != '\0') {
        day *= 10;
        day += c - '0';
        n++;
        c = token[n];
    }

    token = strtok(NULL, "/");
    int month = 0;

    c = token[0];
    n = 0;

    while(c != '\0') {
        month *= 10;
        month += c - '0';
        n++;
        c = token[n];
    }

    int year = 0;

    token = strtok(NULL, "/");
    c = token[0];
    n = 0;

    while(c != '\0') {
        year *= 10;
        year += c - '0';
        n++;
        c = token[n];
    }

    // 20250501
    return year * 10000 + month * 100 + day;
}

int checkTreeInfo(Task* root, int currentHeight, TreeInfo* info) {
    if(root == NULL)
        return currentHeight - 1;

    info->size++;

    if(root->left == NULL && root->right == NULL)
        info->leaves++;
    else
        info->internalNodes++;

    int left = checkTreeInfo(root->left, currentHeight + 1, info);
    int right = checkTreeInfo(root->right, currentHeight + 1, info);

    if(right > left)
        return right;

    return left;
}
