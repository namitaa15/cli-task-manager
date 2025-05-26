#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "task.h"

#define FILE_NAME "data.txt"
#define BACKUP_FILE "backup.txt"
#define MAX_TASKS 1000

Task tasks[MAX_TASKS];
int task_count = 0;
void getTodayDate(char *buffer) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, 11, "%Y-%m-%d", tm_info);
}

void getCurrentTime(char *buffer) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, 30, "%Y-%m-%d %H:%M", tm_info);
}

void loadTasks() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) return;
    task_count = 0;
    while (fscanf(fp, "%d|%[^|]|%[^|]|%[^|]|%d|%[^\n]\n",
                  &tasks[task_count].id,
                  tasks[task_count].title,
                  tasks[task_count].category,
                  tasks[task_count].timestamp,
                  &tasks[task_count].is_done,
                  tasks[task_count].due_date) == 6) {
        task_count++;
    }
    fclose(fp);
}


void saveTasks() {
    FILE *fp = fopen(FILE_NAME, "w");
    for (int i = 0; i < task_count; i++) {
        fprintf(fp, "%d|%s|%s|%s|%d|%s\n", tasks[i].id, tasks[i].title, tasks[i].category,
            tasks[i].timestamp, tasks[i].is_done, tasks[i].due_date);
    }
    fclose(fp);
}

void backupTasks() {
    FILE *fp = fopen(BACKUP_FILE, "w");
    for (int i = 0; i < task_count; i++) {
        fprintf(fp, "%d|%s|%s|%s|%d|%s\n", tasks[i].id, tasks[i].title, tasks[i].category,
                tasks[i].timestamp, tasks[i].is_done, tasks[i].due_date);
    }
    fclose(fp);
}

void addTask() {
    loadTasks();
    if (task_count >= MAX_TASKS) {
        printf("\033[1;31mTask limit reached!\033[0m\n");
        return;
    }

    Task t;
    t.id = task_count + 1;

    printf("Enter task title: ");
    fgets(t.title, sizeof(t.title), stdin);
    t.title[strcspn(t.title, "\n")] = 0;

    printf("Enter category (e.g., todo/study/ideas): ");
    fgets(t.category, sizeof(t.category), stdin);
    t.category[strcspn(t.category, "\n")] = 0;

    printf("Enter due date (YYYY-MM-DD): ");
    fgets(t.due_date, sizeof(t.due_date), stdin);
    t.due_date[strcspn(t.due_date, "\n")] = 0;

    getCurrentTime(t.timestamp);
    t.is_done = 0;

    tasks[task_count++] = t;
    saveTasks();
    printf("\033[1;32mTask added successfully!\033[0m\n");
}



void addTaskCLI(const char *title) {
    loadTasks();
    if (task_count >= MAX_TASKS) return;
    Task t;
    t.id = task_count + 1;
    strncpy(t.title, title, 99);
    printf("Enter category (e.g., todo/study/ideas): ");
    fgets(t.category, sizeof(t.category), stdin);
    t.category[strcspn(t.category, "\n")] = 0;
    getCurrentTime(t.timestamp);
    t.is_done = 0;
    tasks[task_count++] = t;
    saveTasks();
    printf("Task added from CLI!\n");
}

void viewTasks() {
    loadTasks();
    printf("DEBUG: task_count = %d\n", task_count);  // 👈 Add this

    if (task_count == 0) {
        printf("\033[1;33mNo tasks found.\033[0m\n");
        return;
    }

    sortTasks();

    for (int i = 0; i < task_count; i++) {
        char today[11];
        getTodayDate(today);
        int overdue = strcmp(today, tasks[i].due_date) > 0 && !tasks[i].is_done;

        printf("%d. \033[%dm%s [%s] (%s) Due: %s%s\033[0m\n",
               tasks[i].id,
               tasks[i].is_done ? 32 : (overdue ? 31 : 33),
               tasks[i].title,
               tasks[i].category,
               tasks[i].timestamp,
               tasks[i].due_date,
               overdue ? " [OVERDUE]" : "");
    }
}



void deleteTask() {
    loadTasks();
    int id;
    printf("Enter task ID to delete: ");
    scanf("%d", &id);
    getchar();
    backupTasks();
    int found = 0;
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].id == id) {
            found = 1;
            for (int j = i; j < task_count - 1; j++) {
                tasks[j] = tasks[j + 1];
            }
            task_count--;
            saveTasks();
            printf("\033[1;32mTask deleted.\033[0m\n");
            break;
        }
    }
    if (!found) printf("\033[1;31mTask ID not found.\033[0m\n");
}

void editTask() {
    loadTasks();
    int id;
    printf("Enter task ID to edit: ");
    scanf("%d", &id);
    getchar();
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].id == id) {
            printf("Edit title: ");
            fgets(tasks[i].title, sizeof(tasks[i].title), stdin);
            tasks[i].title[strcspn(tasks[i].title, "\n")] = 0;
            printf("Edit category: ");
            fgets(tasks[i].category, sizeof(tasks[i].category), stdin);
            tasks[i].category[strcspn(tasks[i].category, "\n")] = 0;
            saveTasks();
            printf("\033[1;32mTask updated.\033[0m\n");
            return;
        }
    }
    printf("\033[1;31mTask ID not found.\033[0m\n");
}

void searchTask() {
    loadTasks();
    char keyword[100];
    printf("Enter keyword or category: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;
    int found = 0;
    for (int i = 0; i < task_count; i++) {
        if (strstr(tasks[i].title, keyword) || strstr(tasks[i].category, keyword)) {
            printf("%d. \033[%dm%s [%s] (%s)\033[0m\n", tasks[i].id,
                   tasks[i].is_done ? 32 : 31, tasks[i].title, tasks[i].category, tasks[i].timestamp);
            found = 1;
        }
    }
    if (!found) printf("\033[1;33mNo matching tasks.\033[0m\n");
}

void searchTaskCLI(const char *kw) {
    loadTasks();
    int found = 0;
    for (int i = 0; i < task_count; i++) {
        if (strstr(tasks[i].title, kw) || strstr(tasks[i].category, kw)) {
            printf("%d. \033[%dm%s [%s] (%s)\033[0m\n", tasks[i].id,
                   tasks[i].is_done ? 32 : 31, tasks[i].title, tasks[i].category, tasks[i].timestamp);
            found = 1;
        }
    }
    if (!found) printf("No matching tasks.\n");
}

void markDone() {
    loadTasks();
    int id;
    printf("Enter task ID to mark as done: ");
    scanf("%d", &id);
    getchar();
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].id == id) {
            tasks[i].is_done = 1;
            saveTasks();
            printf("\033[1;32mMarked as done!\033[0m\n");
            return;
        }
    }
    printf("\033[1;31mTask ID not found.\033[0m\n");
}

void clearTasks() {
    char confirm;

    // Flush leftover newline
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    printf("[DEBUG] Clear All Tasks function called.\n");
    printf("Are you sure you want to delete all tasks? (y/n): ");
    scanf("%c", &confirm);

    if (confirm == 'y' || confirm == 'Y') {
        backupTasks();
        FILE *fp = fopen(FILE_NAME, "w");
        if (fp) fclose(fp);
        task_count = 0;
        printf("\033[1;32mAll tasks cleared.\033[0m\n");
    } else {
        printf("\033[1;33mCancelled.\033[0m\n");
    }
}



void exportToCSV() {
    loadTasks();
    FILE *fp = fopen("export.csv", "w");
    fprintf(fp, "ID,Title,Category,Timestamp,Status\n");
    for (int i = 0; i < task_count; i++) {
        fprintf(fp, "%d,%s,%s,%s,%s\n", tasks[i].id, tasks[i].title, tasks[i].category,
                tasks[i].timestamp, tasks[i].is_done ? "Done" : "Pending");
    }
    fclose(fp);
    printf("\033[1;32mExported to export.csv\033[0m\n");
}
int compareTasks(const void *a, const void *b) {
    Task *t1 = (Task *)a;
    Task *t2 = (Task *)b;

    // 1. Pending (0) before Done (1)
    if (t1->is_done != t2->is_done) {
        return t1->is_done - t2->is_done;
    }

    // 2. Newer timestamp first
    return strcmp(t2->timestamp, t1->timestamp);  // descending order
}

void sortTasks() {
    qsort(tasks, task_count, sizeof(Task), compareTasks);
}

void restoreBackup() {
    FILE *src = fopen("backup.txt", "r");
    if (!src) {
        printf("\033[1;31mNo backup file found.\033[0m\n");
        return;
    }

    FILE *dest = fopen("data.txt", "w");
    if (!dest) {
        printf("\033[1;31mFailed to restore backup.\033[0m\n");
        fclose(src);
        return;
    }

    char ch;
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dest);
    }

    fclose(src);
    fclose(dest);
    printf("\033[1;32mBackup restored successfully!\033[0m\n");
}

void filterTasksCLI(const char *filter) {
    loadTasks();
    char today[11];
    getTodayDate(today);
    int found = 0;

    for (int i = 0; i < task_count; i++) {
        if ((strcmp(filter, "today") == 0 && strncmp(tasks[i].due_date, today, 10) == 0) ||
            (strcmp(filter, tasks[i].category) == 0)) {
            printf("%d. \033[%dm%s [%s] (%s) Due: %s\033[0m\n", tasks[i].id,
                   tasks[i].is_done ? 32 : 31, tasks[i].title, tasks[i].category,
                   tasks[i].timestamp, tasks[i].due_date);
            found = 1;
        }
    }

    if (!found) printf("\033[1;33mNo matching tasks found.\033[0m\n");
}
void showStats() {
    loadTasks();
    int done = 0, pending = 0;
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].is_done) done++;
        else pending++;
    }
    printf("\033[1;36mTotal: %d | Done: %d | Pending: %d\033[0m\n", task_count, done, pending);
}

void autoDeleteOldTasks() {
    loadTasks();
    time_t now = time(NULL);
    struct tm task_tm;
    int kept = 0;
    Task updated[MAX_TASKS];

    for (int i = 0; i < task_count; i++) {
        memset(&task_tm, 0, sizeof(task_tm));
        strptime(tasks[i].timestamp, "%Y-%m-%d %H:%M", &task_tm);
        time_t task_time = mktime(&task_tm);
        double diff = difftime(now, task_time) / (60 * 60 * 24); // in days

        if (diff <= 7) {
            updated[kept++] = tasks[i];
        }
    }

    task_count = kept;
    memcpy(tasks, updated, sizeof(updated));
    saveTasks();
    printf("\033[1;32mOld tasks deleted.\033[0m\n");
}

void notesMode() {
    char line[256];
    FILE *fp = fopen("notes.txt", "a+");
    if (!fp) {
        printf("Error opening notes.\n");
        return;
    }

    printf("=== Notes Mode ===\nType your note (type 'exit' to quit):\n");
    while (1) {
        printf("> ");
        fgets(line, sizeof(line), stdin);
        if (strncmp(line, "exit", 4) == 0) break;
        fputs(line, fp);
    }

    fclose(fp);
    printf("Notes saved to notes.txt\n");
}