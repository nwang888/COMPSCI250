#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.14159265358979323846

//Linked List node
struct node {
    char name[64];
    float ppd; //pizza per dollar
    struct node* next;
};

//sort the pizza list
int main(int argc, char * argv[]) {
    //read the inputted file
    FILE * f = fopen(argv[1], "r");

    //define start of linked list
    struct node * head = NULL;
    //loop over each 3 lines in the file, exit when seeing DONE
    while (true) {
        char name[64];
        float diameter;
        float cost;
        float ppd;

        //read the set of 3 lines
        if (fscanf(f, "%s", name) == EOF) {//also enters into name, diameter, cost
            printf("PIZZA FILE IS EMPTY"); // If Empty File, print PIZZA FILE IS EMPTY and exit
            break;
        }
        if (strcmp(name,"DONE") == 0) //exit loop if file is done
            break;
        fscanf(f, "%f", &diameter);
        fscanf(f, "%f", &cost);
        
        //calculate ppd
        if (cost == 0 || diameter == 0) {
            ppd = 0;
        }
        else
            ppd = (diameter*diameter*PI/4)/(cost);
        
        //put information into a new node
        struct node * newnode;
        newnode = (struct node*)calloc(1,sizeof(struct node));
        strcpy(newnode->name, name);
        newnode->ppd = ppd;

        //put the new node in its sorted position on the linked list
        //insertion sort
        struct node * cur;

        if (head == NULL || newnode->ppd >= head->ppd) {//if the linked list is empty or if newnode is the greatest ppd
            newnode->next = head;
            head = newnode;
        }
        else {
            cur = head;
            //while (cur->next != NULL && cur->next->ppd > newnode->ppd)
            while (cur->next != NULL && (cur->next->ppd > newnode->ppd || (cur->next->ppd == newnode->ppd && strcmp(cur->next->name,newnode->name) < 0))) //iterate until newnode is bigger than the next one after current or the next one is NULL
                cur = cur->next;
            newnode->next = cur->next; //insert newnode between current and current->next
            cur->next = newnode;
        }
    }
    fclose(f);
    //print list and free memory
    while (head != NULL) {
        printf("%s %f\n", head->name, head->ppd);
        struct node * temp = head; //temporary node to free memory
        head = head->next;
        free(temp);
    }
    return EXIT_SUCCESS;
}