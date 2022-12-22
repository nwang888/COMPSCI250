#include <stdio.h>
#include <stdlib.h>

struct HoopsPlayer {
    int playerNum;
    float avgPoints;
};

void swapPlayers(struct HoopsPlayer* player1, struct HoopsPlayer* player2) {
    struct HoopsPlayer temp = *player1;
    *player1 = *player2;
    *player2 = temp;
}

void bubbleSort(struct HoopsPlayer* x, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++)
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (x[j].avgPoints > x[j + 1].avgPoints)
                swapPlayers(&x[j], &x[j + 1]);
}

int main(int argc, char* argv[]) {
    struct HoopsPlayer hoopList[10];
    int count = 0;
    while (count < 10) {
        printf("Enter player number (int): \n");
        scanf("%i",&hoopList[count].playerNum);
        printf("Enter player average points (float): \n");
        scanf("%f",&hoopList[count].avgPoints);
        count++;
    }
    bubbleSort(hoopList, 10);
    for (int i = 0 ; i < 10 ; i++) {
        printf("Player number: %i Avg Points: %f\n", hoopList[i].playerNum, hoopList[i].avgPoints);
    }
}