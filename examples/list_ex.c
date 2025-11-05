#include <stdio.h>
#include <string.h>
#include "misc/list.h"

int main()
{
    // +----------------------------------------+
    // | manage a todo list using a linked list |
    // +----------------------------------------+

    misc_list todo = misc_list_create(sizeof(char) * 50);
    if (todo == NULL)
    {
        printf("misc_list handle allocation failed. Exiting...\n");
        return 1;
    }

    printf("=== Todo List Manager ===\n\n");

    char task1[50] = "Write documentation";
    char task2[50] = "Fix bug in parser";
    char task3[50] = "Review pull requests";
    char task4[50] = "Deploy to production";

    misc_list_pushback(todo, task1);
    misc_list_pushback(todo, task2);
    misc_list_pushback(todo, task3);
    misc_list_pushfront(todo, task4);

    printf("Current tasks (%zd total):\n", misc_list_size(todo));
    for (size_t i = 0; i < misc_list_size(todo); ++i)
    {
        char *task = (char*)misc_list_get(todo, i);
        printf("%zu. %s\n", i + 1, task);
    }
    printf("\n");

    printf("First task: %s\n", (char*)misc_list_front(todo));
    printf("Last task: %s\n\n", (char*)misc_list_back(todo));

    char completed[50];
    misc_list_popfront(todo, completed);
    printf("Completed task: %s\n", completed);

    char new_task[50] = "Update README";
    misc_list_insert(todo, 1, new_task);
    printf("Inserted new task at position 2\n\n");

    printf("Updated task list (%zd remaining):\n", misc_list_size(todo));
    for (size_t i = 0; i < misc_list_size(todo); ++i)
    {
        char *task = (char*)misc_list_get(todo, i);
        printf("%zu. %s\n", i + 1, task);
    }
    printf("\n");

    char removed[50];
    misc_list_remove(todo, 2, removed);
    printf("Removed task at position 3: %s\n", removed);

    printf("\nFinal task list (%zd remaining):\n", misc_list_size(todo));
    for (size_t i = 0; i < misc_list_size(todo); ++i)
    {
        char *task = (char*)misc_list_get(todo, i);
        printf("%zu. %s\n", i + 1, task);
    }

    misc_list_destroy(todo);
    return 0;
}
