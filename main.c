#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"

void showMenu() {
    printf("\n\033[1;34m=== Task Manager CLI ===\033[0m\n");
    printf("1. Add Task\n");
    printf("2. View Tasks\n");
    printf("3. Delete Task\n");
    printf("4. Edit Task\n");
    printf("5. Search Task\n");
    printf("6. Mark Task as Done\n");
    printf("7. Clear All Tasks\n");
    printf("8. Export Tasks to CSV\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "-a") == 0 && argc >= 3) {
            addTaskCLI(argv[2]);
            return 0;
        } else if (strcmp(argv[1], "-s") == 0 && argc >= 3) {
            searchTaskCLI(argv[2]);
            return 0;
        } else if (strcmp(argv[1], "-v") == 0) {
            viewTasks();
            return 0;
        } else {
            printf("\033[1;31mInvalid CLI argument.\033[0m\n");
            return 1;
        }
    }

    int choice;
    while (1) {
        showMenu();
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1:
                addTask();
                break;
            case 2:
                viewTasks();
                break;
            case 3:
                deleteTask();
                break;
            case 4:
                editTask();
                break;
            case 5:
                searchTask();
                break;
            case 6:
                markDone();
                break;
            case 7:
                clearTasks();
                break;
            case 8:
                exportToCSV();
                break;
            case 0:
                printf("\033[1;32mGoodbye, Buchku! \xe2\x9c\x8c\033[0m\n");
                exit(0);
            default:
                printf("\033[1;31mInvalid choice! Try again.\033[0m\n");
        }
    }
    return 0;
}
