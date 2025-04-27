#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

    if (argc != 2) {
        return 1;
    }

    int n = atoi(argv[1]);
    int *arr = malloc(n * sizeof(int));
    srand(time(0));

    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100000;
    }

    bubbleSort(arr, n);

    free(arr);

    return 0;
}
