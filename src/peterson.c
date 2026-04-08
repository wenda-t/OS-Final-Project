// Peterson's Solution
// Author: Danesh Elahi :)


#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>


int flag[2] = {0, 0};
int turn = 0;

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

void run_process(int self, int other) {
    char buffer[100];

    printf("\nProcess %d is in entry section\n", self + 1);
    snprintf(buffer, sizeof(buffer), "Peterson: Process %d entered entry section", self + 1);
    log_action(buffer);

    flag[self] = 1;
    turn = other;

    while (flag[other] && turn == other) {
    }

    printf("Process %d is in critical section\n", self + 1);
    snprintf(buffer, sizeof(buffer), "Peterson: Process %d entered critical section", self + 1);
    log_action(buffer);

    sleep(1);

    printf("Process %d is leaving critical section\n", self + 1);
    snprintf(buffer, sizeof(buffer), "Peterson: Process %d leaving critical section", self + 1);
    log_action(buffer);

    flag[self] = 0;

    printf("Process %d is in exit section\n", self + 1);
    snprintf(buffer, sizeof(buffer), "Peterson: Process %d entered exit section", self + 1);
    log_action(buffer);
}

// Thread argument struct
typedef struct {
    int self;
    int other;
} ThreadArgs;
 
// Thread entry point
void *thread_run(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    run_process(args->self, args->other);
    return NULL;
}


int main() {
    int choice;
    char buffer[100];

    while (1) {
        printf("\n--- Peterson's Solution ---\n");
        printf("1. Run Process 1\n");
        printf("2. Run Process 2\n");
        printf("3. Run Both Processes\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            log_action("Peterson: Invalid input\n");
            return 1;
        }

        snprintf(buffer, sizeof(buffer), "Peterson: User selected option %d", choice);
        log_action(buffer);

        if (choice == 1) {
            run_process(0, 1);
        } else if (choice == 2) {
            run_process(1, 0);
        } else if (choice == 3) {
            
            // Reset shared state before spawning threads
            flag[0] = 0;
            flag[1] = 0;
            turn = 0;

            pthread_t t1, t2;
            ThreadArgs args1 = {0, 1};
            ThreadArgs args2 = {1, 0};
 
            pthread_create(&t1, NULL, thread_run, &args1);
            pthread_create(&t2, NULL, thread_run, &args2);
 
            pthread_join(t1, NULL);
            pthread_join(t2, NULL);

        } else if (choice == 4) {
            log_action("Peterson: Program exited");
            printf("Exiting Peterson module\n");
            break;
        } else {
            printf("Invalid choice\n");
            log_action("Peterson: Invalid menu choice");
        }
    }

    return 0;
}