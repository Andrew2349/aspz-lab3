
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <signal.h>


void handle_fsize(int signum) {
    fprintf(stderr, "Error: file size limit exceeded (signal %d)\n", signum);
    exit(EXIT_FAILURE);
}

int main() {
 
    signal(SIGXFSZ, handle_fsize);

    FILE *fp;
    int roll;

    fp = fopen("dice_results.txt", "w");
    if (fp == NULL) {
        perror("Cannot open file for writing");
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    while (1) {
        roll = rand() % 6 + 1;
        if (fprintf(fp, "Dice roll: %d\n", roll) < 0) {
            perror("Error writing to file");
            break;
        }
        fflush(fp);  
    }

    fclose(fp);
    return EXIT_SUCCESS;
}

