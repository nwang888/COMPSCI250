#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    
    char playerName[100];
    int height;
    int points;

    printf("Enter your favorite basketball player: \n");
    scanf("%s",playerName);
    printf("Enter height as an integer: \n");
    scanf("%i", &height);
    printf("Enter points as an integer: \n");
    scanf("%i", &points);
    
    printf("%s scores %f points per inch",playerName,(float)height/points);

    return EXIT_SUCCESS;
}