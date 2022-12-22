#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]){
    if (argc != 2)
        printf("Syntax: ./byseven {Integer}");
    for (int i = 1 ; i <= atoi(argv[1]) ; i++)
        printf("%i\n",i*7);
    return EXIT_SUCCESS;
}