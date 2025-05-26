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
    printf("9. Undo (Restore Backup)\n");
    printf("10. Show Task Stats\n");
    printf("11. Auto-delete Tasks Older Than 7 Days\n");
    printf("12. Notes Mode\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
}

void askPassword() {
    char input[20];
    printf("Enter PIN: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strcmp(input, "1234") != 0) {
        printf("\033[1;31mWrong PIN!\033[0m\n");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    // 🧩 CLI mode
    if (argc > 1) {
        if (strcmp(argv[1], "-a") == 0 && argc >= 3) {
            askPassword();
            addTaskCLI(argv[2]);
            return 0;
        } else if (strcmp(argv[1], "-s") == 0 && argc >= 3) {
            searchTaskCLI(argv[2]);
            return 0;
        } else if (strcmp(argv[1], "-v") == 0) {
            askPassword();
            viewTasks();
            return 0;
        } else if (strcmp(argv[1], "-filter") == 0 && argc >= 3) {
            askPassword();
            filterTasksCLI(argv[2]);
            return 0;
        } else if (strcmp(argv[1], "-clock") == 0) {
            system("date");
            return 0;
        } else if (strcmp(argv[1], "-battery") == 0) {
            system("pmset -g batt");
            return 0;
        } else if (strcmp(argv[1], "-storage") == 0) {
            system("df -h");
            return 0;
        } else if (strcmp(argv[1], "-weather") == 0) {
            system("curl wttr.in?format=3");
            return 0;
        } else {
            printf("\033[1;31mInvalid CLI argument.\033[0m\n");
            return 1;
        }
    }

    // 🧑‍💻 Interactive mode
    askPassword();
    int choice;
    char input[10];

    while (1) {
        showMenu();
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%d", &choice);

        switch (choice) {
            case 1:  addTask(); break;
            case 2:  viewTasks(); break;
            case 3:  deleteTask(); break;
            case 4:  editTask(); break;
            case 5:  searchTask(); break;
            case 6:  markDone(); break;
            case 7:  clearTasks(); break;
            case 8:  exportToCSV(); break;
            case 9:  restoreBackup(); break;
            case 10: showStats(); break;
            case 11: autoDeleteOldTasks(); break;
            case 12: notesMode(); break;
            case 0:
                printf("\033[1;32mGoodbye! ✌️\033[0m\n");
                exit(0);
            default:
                printf("\033[1;31mInvalid choice! Try again.\033[0m\n");
        }
    }

    return 0;
}