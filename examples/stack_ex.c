#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "misc/stack.h"


int main()
{
    // +-----------------------------+
    // | check good parenthesization |
    // +-----------------------------+

    misc_stack stack = misc_stack_create(sizeof(char));
    if (stack == NULL)
    {
        printf("misc_stack handle allocation failed. Exiting...\n");
        return 1;
    }

    char *str = "(()(()))";

    int still_good = 1;
    for (size_t i = 0; i < strlen(str) && still_good; ++i)
    {
        if (str[i] == ')')
        {
            int tmp = misc_stack_pop(stack, NULL);
            if (!tmp)
            {
                still_good = 0;
            }
        }
        else if (str[i] == '(')
        {
            misc_stack_push(stack, &str[i]);
        }
    }

    if (!still_good || misc_stack_size(stack) != 0)
        printf("The provided expression '%s' is not well parenthesized\n", str);
    else
        printf("The provided expression '%s' is well parenthesized\n", str);


    misc_stack_destroy(stack);
    return 0;
}