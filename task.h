#ifndef TASK_H
#define TASK_H

// Task structure
typedef struct {
    int id;
    char title[100];
    char category[50];
    char timestamp[30];
    int is_done; // 0 = pending, 1 = done
    char due_date[15]; 
} Task;

// Core Features
void addTask();
void viewTasks();
void deleteTask();
void editTask();
void searchTask();
void markDone();
void clearTasks();
void restoreBackup(); // Undo feature
void sortTasks();  // Sort tasks by status + timestamp
// Advanced Features
void addTaskCLI(const char *title);     // For ./cli -a "task"
void searchTaskCLI(const char *kw);     // For ./cli -s "word"
void exportToCSV();                     // Export to export.csv

// Utility
void getCurrentTime(char *buffer);
void loadTasks();
void saveTasks();
void backupTasks();

// 🔐 Protected & Filters
void filterTasksCLI(const char *filter);
void showStats();

// 📅 Due & Clean-up
void autoDeleteOldTasks();
void getTodayDate(char *buffer);

// 📝 Notes Mode
void notesMode();


#endif