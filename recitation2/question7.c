#include <stdio.h>
#include <stdlib.h>

struct HoopsPlayer {
    int playerNum;
    float avgPoints;
    HoopsPlayerNode* next;
};

void printAndFreeList(struct HoopsPlayer* x) {
    int count1 = 0;
    struct HoopsPlayer* ptr = x;
    print(f"\nFinal rankings by id: \n");
    struct HoopsPlayer* temp = x;
    while (x!= NULL) {
        printf("id: %d, score: %f\n", x->id, x->avgPoints);
        struct HoopsPlayer* temp = x;
        x = x->next;
        free(temp);
    }
}

// void swapPlayers(struct HoopsPlayer* player1, struct HoopsPlayer* player2) {
//     struct HoopsPlayer temp = *player1;
//     *player1 = *player2;
//     *player2 = temp;
// }

void bubbleSort(struct HoopsPlayer* x, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++)
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (x[j].avgPoints > x[j + 1].avgPoints)
                swapPlayers(&x[j], &x[j + 1]);
}

int main(int argc, char* argv[]) {
    int id;
    float f;
    struct HoopsPlayer* first = (struct HoopsPlayer*) calloc(1,sizeof(struct HoopsPlayer));
    struct HoopsPlayer* current = first;
    while (1) {
        printf("Enter player number (int): \n");
        int checkID = scanf("%i",&id);
        if (id == -1) break;
        printf("Enter player average points (float): \n");
        int checkf = scanf("%f",&f);
        current->id = id;
        current->avg_points = f;
        struct HoopsPlayer*t = (struct HoopsPlayer*) calloc(1,sizeof(struct HoopsPlayer));

        current->next = t;
        current = current->next;
    }
    bubbleSort(first);
    printAndFreeList(first);
    return EXIT_SUCCESS;
}