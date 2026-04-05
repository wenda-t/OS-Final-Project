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

/**
 * Speedup Calculation
 * 
 * Speedup = 1 / ( (1-P) + (P/N) )
 * Where P is the parallel fraction(0-1) and N is the number of cores.
 */
int main(){
    double p, speedup;
    int n;

    printf("\nAmdahl's Law Calculator\n");
    printf("-----------------------\n");

    // Get Parallel Portion
    printf("Enter the parallel portion of the program (0.0 to 1.0):\n");
    scanf("%lf", &p);
    if (p < 0 || p > 1) {
        printf("Invalid Number, please enter decimal between 0 and 1\n");
        return 1;
    }

    // Get Number of Cores
    printf("Enter number of cores:\n");
    scanf("%d", &n);
    if (n <= 0){
        printf("You must have 1 or more cores.\n");
        return 1;
    }

    // Calculate Speedup and Print Results
    speedup = 1.0 / ((1.0-p) + (p/n));
    printf("Theoretical Speedup: %.2f\n", speedup);
    printf("This means the program runs %.2f times faster (parallel) than on 1 core (serial).\n", speedup);

    return 0;
}