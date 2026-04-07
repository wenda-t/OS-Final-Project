/**
 * amdahl.c | Amdahl's Law Calculation
 * 
 * Calculates the theoretical speedup from a user given parallel fraction 
 * and core count.
 * 
 * Author: Wenda
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>


// For Logger
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


/**
 * Amdahl's Module Menu
 */
static void print_menu(){
    printf("\n");
    printf("==== Amdahl's Module ====\n");
    printf("1. Calculate Speedup\n");
    printf("2. Compare Serial vs Parallel\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}


/**
 * Speedup Calculation
 * 
 * Speedup = 1 / ( (1-P) + (P/N) )
 * p : Part of program that is parallel
 * n : number of cores
 * 
 */
double amdahl_speedup(double p, int n){
    // Validate parallel portion
    if (p < 0 || p > 1) {
        log_action("AMDAHL: Invalid Parallel Portion Input");
        printf("Invalid number, please enter a decimal between 0 and 1.\n");
        return -1.0;
    }
    // Validate cores
    if (n <= 0) {
        log_action("AMDAHL: Invalid Core Input");
        printf("You must have 1 or more cores.\n");
        return -1.0;
    }

    return 1.0 / ((1.0 - p) + (p / n));
}

/**
 * Calculate Speedup of a p parallel program with n cores.
 * 
 * p : Part of program that is parallel
 * n : number of cores
 */
void calculate_speedup(){
    double p, speedup;
    int n;
    char buffer[100];
    
    // Get Parallel Portion
    printf("Enter the parallel portion of the program (0.0 to 1.0):\n");
    scanf("%lf", &p);
    // Logging 
    snprintf(buffer, sizeof(buffer), "AMDAHL: User Input for Parallel Portion: %lf", p);
    log_action(buffer);

    // Get Number of Cores
    printf("Enter number of cores:\n");
    scanf("%d", &n);
    // Logging
    snprintf(buffer, sizeof(buffer), "AMDAHL: User Input for Cores: %d", n);
    log_action(buffer);

    // Calculate Speedup and Print Results
    speedup = amdahl_speedup(p, n);
    if (speedup < 0) {
        return;
    }

    printf("Theoretical Speedup: %.2f\n", speedup);
}

/**
 * Comparing a Program that's Parallel vs Serial 
 */
void compare_serial_parallel(){
    double p;
    int n;
    char buffer[100];
    
    // Get Parallel Portion
    printf("Enter the parallel portion of the program (0.0 to 1.0):\n");
    scanf("%lf", &p);
    // Logging 
    snprintf(buffer, sizeof(buffer), "AMDAHL: User Input for Parallel Portion: %lf", p);
    log_action(buffer);

    // Get Number of Cores
    printf("Enter number of cores:\n");
    scanf("%d", &n);
    // Logging
    snprintf(buffer, sizeof(buffer), "AMDAHL: User Input for Cores: %d", n);
    log_action(buffer);

    // Calculate Speedup and Print Results
    double serial_speedup = amdahl_speedup(p, 1);
    double parallel_speedup = amdahl_speedup(p, n);
    
    // Error Handling
    if (serial_speedup < 0 || parallel_speedup < 0) {
        return;
    }

    printf("\nSerial Speed: %.2f\n", serial_speedup);
    printf("Parallel Speedup:  %.2f\n", parallel_speedup);
    printf("The Parallel program is %.2fx faster than the serial program.\n", parallel_speedup/serial_speedup);
}


int main(){
    int choice;

    while(1){
        print_menu();

        if (scanf("%d", &choice) != 1) {
            log_action("AMDAHL: Invalid Input");
            printf("Invalid input, please enter a number\n");
            while(getchar() != '\n'); // Gets rid of input buffer
            continue;
        }

        switch(choice){
            case 1:
                log_action("AMDAHL: Calculate Speedup");
                calculate_speedup();
                break;
            case 2:
                log_action("AMDAHL: Compare Serial vs Parallel");
                break;
            case 3: 
                log_action("AMDAHL: Exit");
                printf("Exiting...\n");
                return 0;
            default:
                log_action("AMDAHL: Invalid Choice");
                printf("Invalid choice\n");
                break;

        }
    }

    return 0;
}