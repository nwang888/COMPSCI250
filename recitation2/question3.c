#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

    if (argc != 2) {
        printf("Syntax: ./arglen <arg> \n");
        return 0;
    }
    int i = 0;
    while (argv[1][i] != '\0'){
        i++;
    }
    printf("String len = %d\n", i);
    return EXIT_SUCCESS;
}