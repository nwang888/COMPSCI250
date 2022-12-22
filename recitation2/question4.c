#include <stdio.h>
#include <stdlib.h>

int sumArray(int* ptr) {
    int sum = 0;
    for (int i = 0 ; i < 100 ; i++) {
        sum += *ptr;   
        ptr = ptr + 1; //C assumes ptr stores integers so it goes to the next integer in the array (increments by 4 bytes)
    }
    return sum;
}

int main (int argc, char* argv[]) {

    int arr[100];
    for (int i = 0 ; i < 100 ; i++) {
        arr[i] = i;
    }
    printf("%i",sumArray(arr));
    return EXIT_SUCCESS;
}