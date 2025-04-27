#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {

            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main(int argc, char *argv[]) {

    int n = atoi(argv[1]);
    int *arr = malloc(n * sizeof(int));
    srand(time(0));

    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100000;
    }

    struct tms start, end;
    long clocks_per_sec = sysconf(_SC_CLK_TCK);
    long clocks;
    times(&start);

    bubbleSort(arr, n);

    times(&end);

    free(arr);

    clocks = end.tms_utime - start.tms_utime + start.tms_stime;
    printf("Time taken: %ld ticks.\n", clocks);
    printf("Time taken: %lf sec.\n", (double)clocks / clocks_per_sec);
    return 0;
}
