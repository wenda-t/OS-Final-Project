#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>

/**
 * Simple file management system with basic file operations.
*/

//logger function
void log_action(const char *message) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "./logger_location", sizeof(addr.sun_path) - 1);

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
        write(fd, message, strlen(message));
    }
    close(fd);
}


// Creates file with given filename
void createFile() {
    char filename[100];
    char buffer[150];
    FILE *fp;

    printf("Enter filename to create: ");
    scanf("%s", filename);
    snprintf(buffer, sizeof(buffer), "FILE_MANAGEMENT: Created file '%s'", filename);
    log_action(buffer);

    // try to open file to see if it was created successfully
    fp = fopen(filename, "w");
    

    if (fp == NULL) {
        printf("Error creating file.\n");
        snprintf(buffer, sizeof(buffer), "FILE_MANAGEMENT: Failed to create file '%s'", filename);
        log_action(buffer);
        return;
    }

    printf("File created successfully.\n");
    fclose(fp);
}

// reads file contents of given filename
void readFile() {
    char filename[100];
    char buffer[200];
    char ch;
    FILE *fp;

    printf("Enter filename to read: ");
    scanf("%s", filename);
    snprintf(buffer, sizeof(buffer), "FILE_MANAGEMENT: Trying to read file '%s'", filename);
    log_action(buffer);

    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error opening file.\n");
        snprintf(buffer, sizeof(buffer), "FILE_MANAGEMENT: Failed to read file '%s'", filename);
        log_action(buffer);
        return;
    }

    // prints contents of file one character at a time until EOF
    printf("\nContents of \"%s\":\n", filename);
    while ((ch = fgetc(fp)) != EOF) {
        printf("%c", ch);
    }
    printf("\n");

    fclose(fp);
}

// writes to file of give filename
void writeFile() {
    char filename[100];
    char text[200];
    char buffer[300];
    FILE *fp;

    printf("Enter filename to write: ");
    scanf("%s", filename);

    snprintf(buffer, sizeof(buffer), "FILE_MANAGEMENT: Attempting to write to file '%s'", filename);
    log_action(buffer);

    fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        snprintf(buffer, sizeof(buffer), "FILE_MANAGEMENT: Failed to open file '%s'", filename);
        log_action(buffer);
        return;
    }

    printf("Enter content to write:\n");
    getchar();
    fgets(text, sizeof(text), stdin);

    // Strip trailing newline before logging
    text[strcspn(text, "\n")] = '\0';

    fprintf(fp, "%s\n", text);
    snprintf(buffer, sizeof(buffer), "FILE_MANAGEMENT: Wrote '%.150s' to file '%s'", text, filename);
    log_action(buffer);

    printf("Content written to \"%s\".\n", filename);
    fclose(fp);
}


// deletes file of given filename
void deleteFile() {
    char filename[100];
    char buffer[150];

    printf("Enter file name to delete: ");
    scanf("%s", filename);
    snprintf(buffer, sizeof(buffer), "FILE_MANAGEMENT: Attempting to delete file '%s'", filename);
    log_action(buffer);

    if (remove(filename) == 0) {
        printf("\nFile deleted successfully.\n");
        snprintf(buffer, sizeof(buffer), "FILE_MANAGEMENT: Deleted file '%s'", filename);
        log_action(buffer);
    } else {
        printf("\nError deleting file.\n");
        snprintf(buffer, sizeof(buffer), "FILE_MANAGEMENT: Failed to delete file '%s'", filename);
        log_action(buffer);
    }
}


// opens the current directory, reads each file, and prints the names
void listFiles() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");

    if (dir == NULL) {
        printf("Error opening directory.\n");
        log_action("FILE_MANAGEMENT: Failed to open directory");
        return;
    }

    printf("\nFiles in current directory:\n");

    // reads and prints files until none left
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

// main menu for the file management system
int main() {
    int choice;

    do {
        printf("\n=== File Management ===\n");
        printf("1. Create File\n");
        printf("2. Read File\n");
        printf("3. Write File\n");
        printf("4. Delete Files\n");
        printf("5. List File\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createFile();
                log_action("FILE_MANAGEMENT: Selected Create File");
                break;
            case 2:
                readFile();
                log_action("FILE_MANAGEMENT: Selected read file");
                break;
            case 3:
                writeFile();
                log_action("FILE_MANAGEMENT: Selected write to file");
                break;
            case 4:
                deleteFile();
                log_action("FILE_MANAGEMENT: Selected delete file");
                break;
            case 5:
                listFiles();
                log_action("FILE_MANAGEMENT: Selected list files");
                break;
            case 6:
                printf("Exiting...\n");
                log_action("FILE_MANAGEMENT: Exited successfully");
                break;
            default:
                printf("Invalid choice.\n");
                log_action("FILE_MANAGEMENT: Invalid choice");

        }

    } while (choice != 6);

    return 0;
}