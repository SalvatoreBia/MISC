#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "misc/vector.h"

int main()
{
    // +---------------------------------------------+
    // | find max value between a vector of integers |
    // +---------------------------------------------+

    srand(time(NULL));

    misc_vector vec = misc_vector_create(sizeof(int));
    for (size_t i = 0; i < 10; ++i)
    {
        int val = rand() % 100;
        printf("Inserted %d into the vector\n", val);
        misc_vector_pushback(vec, &val);
    }
    printf("\n");

    int *max = (int*)misc_vector_front(vec);
    size_t len = misc_vector_length(vec);
    for (size_t i = 1; i < len; ++i)
    {
        int *tmp = (int*)misc_vector_get(vec, i);
        if ((*max) < (*tmp))
            max = tmp;
    }

    printf("Max value found -> %d\n", (*max));

    misc_vector_destroy(vec);
    return 0;
}