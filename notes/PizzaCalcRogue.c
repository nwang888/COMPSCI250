#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.14159265358979323846

typedef struct Pizza
{
    char name[64];
    float ppd;
    struct Pizza *next;
} Pizza;

struct Pizza *appendPizza(struct Pizza *header, struct Pizza *pointer)
{
    struct Pizza *current = header;
    if ((pointer->ppd > header->ppd) || (pointer->ppd == header->ppd && strcmp(pointer->name, header->name) < 0))
    {
        pointer->next = header;
        header = pointer;
    }
    else
    {
        while ((current->next != NULL && current->next->ppd > pointer->ppd) ||
              (current->next != NULL && current->next->ppd == pointer->ppd
                && strcmp(current->next->name, pointer->name) < 0))
        {
            current = current->next;
        }

        pointer->next = current->next;
        current->next = pointer;
    }

    return header;
}

int main(int argc, char *argv[])
{
    FILE *pizzaFile = fopen(argv[1], "r");
    char line[64];
    float diameter;
    float price;

    // first pizza
    if (fgets(line, 64, pizzaFile) == 0)
    {
        printf("PIZZA FILE IS EMPTY\n");
        return EXIT_SUCCESS;
    }

    if (strcmp(line, "DONE\n") == 0){
        return EXIT_SUCCESS;
    }

    struct Pizza *header = (struct Pizza *)malloc(sizeof(struct Pizza));
    strcpy(header->name, strtok(line, "\n"));       // strtok returns first token and assigns line to first token
    fscanf(pizzaFile, "%f", &diameter);
    fscanf(pizzaFile, "%f", &price);

    // calculations
    if (price == 0 || diameter == 0)
    {
        header->ppd = 0;
    }
    else
    {
        header->ppd = (pow(diameter, 2) * (PI / 4)) / price;
    }

    header->next = NULL;

    // other pizzas
    struct Pizza *others;
    fscanf(pizzaFile, "%s", line);
    while (strcmp(line, "DONE") != 0)
    {
        others = (struct Pizza *)malloc(sizeof(struct Pizza));
        strcpy(others->name, line);
        fscanf(pizzaFile, "%f", &diameter);
        fscanf(pizzaFile, "%f", &price);

        if (price == 0 || diameter == 0)
        {
            others->ppd = 0;
        }
        else
        {
            others->ppd = pow(diameter, 2) * (PI / 4) / price;
        }
        others->next = NULL;
        header = appendPizza(header, others);
        fscanf(pizzaFile, "%s", line);
    }

    fclose(pizzaFile);

    // print all pizzas

    while (header != NULL)
    {
        printf("%s ", header->name);
        printf("%f\n", header->ppd);
        struct Pizza *temphead = header;
        header = header->next;
        free(temphead);
    }

    return EXIT_SUCCESS;
}
