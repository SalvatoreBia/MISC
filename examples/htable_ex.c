#include <stdio.h>
#include <string.h>
#include "misc/htable.h"

int main()
{
    // +------------------------------+
    // | count word frequency in text |
    // +------------------------------+

    misc_htable word_count = misc_htable_create_str(sizeof(int));
    if (word_count == NULL)
    {
        printf("misc_htable handle allocation failed. Exiting...\n");
        return 1;
    }

    char *words[] = {"hello", "world", "hello", "hash", "table", "world", "hello"};
    int n_words = 7;

    printf("Processing words: ");
    for (int i = 0; i < n_words; i++)
    {
        printf("%s ", words[i]);
        
        const char *word = words[i];
        int *count = (int*)misc_htable_get(word_count, &word);
        
        if (count == NULL)
        {
            int c = 1;
            misc_htable_put(word_count, &word, &c);
        }
        else
        {
            (*count)++;
        }
    }
    printf("\n\n");

    printf("Word frequencies:\n");
    for (int i = 0; i < n_words; i++)
    {
        const char *word = words[i];
        if (misc_htable_contains(word_count, &word))
        {
            int *count = (int*)misc_htable_get(word_count, &word);
            printf("  '%s' -> %d\n", words[i], *count);
            misc_htable_remove(word_count, &word);
        }
    }

    misc_htable_destroy(word_count);
    return 0;
}

