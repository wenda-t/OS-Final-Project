/**
 * Main Menu: 
 * Choose between the available programs to run, 
 * and returns to the menu once you exit the program
 * 
 * Author: Wenda
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

static void print_menu(){
    printf("\n");
    printf("==== Main Menu ====\n");
    printf("1. File Management\n");
    printf("2. Peterson's Solution\n");
    printf("3. Memory Allocation + Compaction\n");
    printf("4. Amdahl's Law\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

void run(char *path){
    // Create Fork
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork Failed");
    } else if (pid == 0) {
        // Child, use execvp to run program
        char *args[] = {path, NULL};
        if (execvp(args[0], args) == -1) {
            perror("Execution Failed");
        }

        exit(1);

    } else {
        // Parent: Just wait for child 
        wait(NULL);
    }
}

int main(){
    int choice;

    while(1) {
        print_menu();

        // Valid input
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input, please enter a number\n");
            while(getchar() != '\n'); // Gets rid of input buffer
            continue;
        }

        switch (choice) {
            case 1: 
                run("./file_management");
                break;
            case 2: 
                run("./peterson"); 
                break;
            case 3: 
                run("./memory"); 
                break;
            case 4: 
                run("./amdahl"); 
                break;
            case 5: 
                printf("Exiting...\n");
                return 0;
            default:
            printf("Invalid choice\n");
            break;
        }
    }

    return 0;
}