#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

/**
 * Simple file management system with basic file operations.
*/


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
                break;
            case 2:
                readFile();
                break;
            case 3:
                writeFile();
                break;
            case 4:
                deleteFile();
                break;
            case 5:
                listFiles();
                break;
            case 6:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 6);

    return 0;
}