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
#include <sys/socket.h>
#include <sys/un.h>

// Logger
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

void shutdown_logger() {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "./logger_location", sizeof(addr.sun_path) - 1);

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
        write(fd, "SHUTDOWN", strlen("SHUTDOWN"));
    }
    close(fd);
}

// Print Menu
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

/**
 * Function to run program via fork exec
 */
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

    // Launch logger
    pid_t logger_pid = fork();
    if (logger_pid == 0) {
        char *args[] = {"./logger", "../logs/logs.txt", NULL};
        execvp(args[0], args);
        perror("Failed to launch logger");
        exit(1);
    }
    //let the logger scoket bind
    sleep(1);

    while(1) {
        print_menu();

        // Valid input
        if (scanf("%d", &choice) != 1) {
            log_action("Menu Choice: Invalid Input");
            printf("Invalid input, please enter a number\n");
            while(getchar() != '\n'); // Gets rid of input buffer
            continue;
        }

        switch (choice) {
            case 1:
                log_action("Menu Choice: 1"); 
                run("./file_management");
                break;
            case 2: 
                log_action("Menu Choice: 2"); 
                run("./peterson"); 
                break;
            case 3: 
                log_action("Menu Choice: 3"); 
                run("./memory"); 
                break;
            case 4: 
                log_action("Menu Choice: 4"); 
                run("./amdahl"); 
                break;
            case 5: 
                log_action("Menu Choice: 5"); 
                printf("Exiting...\n");
                shutdown_logger();
                return 0;
            default:
                log_action("Menu Choice: Invalid Input");
                printf("Invalid choice\n");
                break;
        }
    }

    return 0;
}