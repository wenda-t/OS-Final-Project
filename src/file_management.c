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
    FILE *fp;

    printf("Enter filename to create: ");
    scanf("%s", filename);

    // try to open file to see if it was created successfully
    fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Error creating file.\n");
        return;
    }

    printf("File created successfully.\n");
    fclose(fp);
}

// reads file contents of given filename
void readFile() {
    char filename[100];
    char ch;
    FILE *fp;

    printf("Enter filename to read: ");
    scanf("%s", filename);

    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error opening file.\n");
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
    FILE *fp;

    printf("Enter filename to write: ");
    scanf("%s", filename);

    fp = fopen(filename, "a"); // append mode

    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter content to write:\n");
    getchar(); // clears new line from thebuffer
    fgets(text, sizeof(text), stdin); // reads all input text

    fprintf(fp, "%s", text); // writes to the file

    printf("Content written to \"%s\".\n", filename);
    fclose(fp);
}


// deletes file of given filename
void deleteFile() {
    char filename[100];

    printf("Enter file name to delete: ");
    scanf("%s", filename);

    if (remove(filename) == 0) {
        printf("\nFile deleted successfully.\n");
    } else {
        printf("\nError deleting file.\n");
    }
}


// opens the current directory, reads each file, and prints the names
void listFiles() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");

    if (dir == NULL) {
        printf("Error opening directory.\n");
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
                log_action("FILE_MANAGEMENT: Created file");
                break;
            case 2:
                readFile();
                log_action("FILE_MANAGEMENT: Read file");
                break;
            case 3:
                writeFile();
                log_action("FILE_MANAGEMENT: Wrote to file");
                break;
            case 4:
                deleteFile();
                log_action("FILE_MANAGEMENT: Deleted file");
                break;
            case 5:
                listFiles();
                log_action("FILE_MANAGEMENT: Listed file");
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