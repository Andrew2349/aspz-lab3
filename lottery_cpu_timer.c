#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>

void handle_timer(int signum) {
    fprintf(stderr, "\nError: CPU time limit exceeded (signal %d)\n", signum);
    exit(EXIT_FAILURE);
}

void generate_unique(int *arr, int n, int maxval) {
    int i, j, num;
    for (i = 0; i < n; i++) {
        do {
            num = rand() % maxval + 1;
            for (j = 0; j < i; j++)
                if (arr[j] == num) break;
        } while (j < i);
        arr[i] = num;
    }
}

int main() {
    struct itimerval timer;

    signal(SIGVTALRM, handle_timer);
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &timer, NULL);

    srand(time(NULL));
    int draw1[7], draw2[6];

    while (1) {
        generate_unique(draw1, 7, 49);
        generate_unique(draw2, 6, 36);
    }

    return 0;
}

