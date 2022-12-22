#include <stdio.h>
#include <stdlib.h>

int recurse(int n) {
    if (n == 0)
        return 2;
    return 3*(n-1)+recurse(n-1)+1;
}
int main(int argc, char * argv[]) {
    if (argc != 2)
        printf("Syntax: ./recurse {Integer}");
    
    printf("%d", recurse(atoi(argv[1])));
    return EXIT_SUCCESS;
}