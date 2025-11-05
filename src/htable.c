#include "misc/htable.h"
#include "misc/vector.h"
#include "misc/list.h"
#include <stdlib.h>
#include <string.h>

#define HTABLE_DEFAULT_CAPACITY 16
#define LOAD_FACTOR_THRESHOLD 0.75

typedef struct misc_htable_entry
{

    void *key;
    void *value;

} misc_htable_entry;

struct misc_generic_hashtable
{

    misc_vector buckets;

    size_t capacity;
    size_t size;

    size_t key_size;
    size_t value_size;

    misc_hash_fn hash_func;
    misc_key_cmp_fn key_cmp;

};


#define FNV_OFFSET_BASIS 0xcbf29ce484222325UL
#define FNV_PRIME        0x100000001b3UL

static size_t _misc_str_hash_fn(const void *key)
{
    const char *str = *(const char**)key;
    size_t hash = FNV_OFFSET_BASIS;

    while (*str)
    {
        hash ^= (unsigned char)(*str++);
        hash *= FNV_PRIME;
    }
    
    return hash;
}

static int _misc_str_key_cmp(const void* key1, const void *key2)
{
    const char *s1 = *(const char**)key1;
    const char *s2 = *(const char**)key2;
    return strcmp(s1, s2);
}


static size_t _misc_int_hash_fn(const void *key)
{
    size_t x = (size_t)(*(const int*)key);

    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9UL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebUL;
    x =  x ^ (x >> 31);

    return x;
}


static int _misc_int_key_cmp(const void *key1, const void *key2)
{
    int a = *(const int*)key1;
    int b = *(const int*)key2;

    if (a < b) return -1;
    if (a > b) return 1;

    return 0;
}


misc_htable misc_htable_create(size_t value_size, size_t key_size,
                               misc_hash_fn hash_func, 
                               misc_key_cmp_fn key_cmp)
{
    if (value_size == 0 || key_size == 0) return NULL;
    if (hash_func == NULL || key_cmp == NULL) return NULL;

    misc_htable ht = (misc_htable)malloc(sizeof(struct misc_generic_hashtable));
    if (ht == NULL) return NULL;

    ht->buckets = misc_vector_create(sizeof(misc_list));
    if (ht->buckets == NULL)
    {
        free(ht);
        return NULL;
    }

    for (size_t i = 0; i < HTABLE_DEFAULT_CAPACITY; i++)
    {
        misc_list bucket = NULL;
        misc_vector_pushback(ht->buckets, &bucket);
    }

    ht->capacity = HTABLE_DEFAULT_CAPACITY;
    ht->size = 0;
    ht->key_size = key_size;
    ht->value_size = value_size;
    ht->hash_func = hash_func;
    ht->key_cmp = key_cmp;

    return ht;
}

misc_htable misc_htable_create_str(size_t value_size)
{
    return misc_htable_create(value_size, sizeof(char*), _misc_str_hash_fn, _misc_str_key_cmp);
}

misc_htable misc_htable_create_int(size_t value_size)
{
    return misc_htable_create(value_size, sizeof(int), _misc_int_hash_fn, _misc_int_key_cmp);
}

void misc_htable_destroy(misc_htable ht)
{
    if (ht == NULL) return;

    for (size_t i = 0; i < ht->capacity; i++)
    {
        misc_list *bucket = (misc_list*)misc_vector_get(ht->buckets, i);
        if (*bucket != NULL)
        {
            size_t list_size = misc_list_size(*bucket);
            for (size_t j = 0; j < list_size; j++)
            {
                misc_htable_entry *entry = (misc_htable_entry*)misc_list_get(*bucket, j);
                free(entry->key);
                free(entry->value);
            }
            misc_list_destroy(*bucket);
        }
    }

    misc_vector_destroy(ht->buckets);
    free(ht);
}

void misc_htable_clear(misc_htable ht)
{
    if (ht == NULL) return;

    for (size_t i = 0; i < ht->capacity; i++)
    {
        misc_list *bucket = (misc_list*)misc_vector_get(ht->buckets, i);
        if (*bucket != NULL)
        {
            size_t list_size = misc_list_size(*bucket);
            for (size_t j = 0; j < list_size; j++)
            {
                misc_htable_entry *entry = (misc_htable_entry*)misc_list_get(*bucket, j);
                free(entry->key);
                free(entry->value);
            }
            misc_list_clear(*bucket);
        }
    }

    ht->size = 0;
}

size_t misc_htable_size(const misc_htable ht)
{
    if (ht == NULL) return 0;
    return ht->size;
}

int misc_htable_contains(const misc_htable ht, const void *key)
{
    if (ht == NULL || key == NULL) return 0;

    size_t hash = ht->hash_func(key);
    size_t bucket_idx = hash % ht->capacity;

    misc_list *bucket = (misc_list*)misc_vector_get(ht->buckets, bucket_idx);
    if (*bucket == NULL) return 0;

    size_t list_size = misc_list_size(*bucket);
    for (size_t i = 0; i < list_size; i++)
    {
        misc_htable_entry *entry = (misc_htable_entry*)misc_list_get(*bucket, i);
        if (ht->key_cmp(entry->key, key) == 0)
        {
            return 1;
        }
    }

    return 0;
}

void* misc_htable_get(const misc_htable ht, const void *key)
{
    if (ht == NULL || key == NULL) return NULL;

    size_t hash = ht->hash_func(key);
    size_t bucket_idx = hash % ht->capacity;

    misc_list *bucket = (misc_list*)misc_vector_get(ht->buckets, bucket_idx);
    if (*bucket == NULL) return NULL;

    size_t list_size = misc_list_size(*bucket);
    for (size_t i = 0; i < list_size; i++)
    {
        misc_htable_entry *entry = (misc_htable_entry*)misc_list_get(*bucket, i);
        if (ht->key_cmp(entry->key, key) == 0)
        {
            return entry->value;
        }
    }

    return NULL;
}

static int _misc_htable_rehash(misc_htable ht)
{
    if (ht == NULL) return 0;

    size_t old_capacity = ht->capacity;
    size_t new_capacity = old_capacity * 2;

    misc_vector new_buckets = misc_vector_create(sizeof(misc_list));
    if (new_buckets == NULL) return 0;

    for (size_t i = 0; i < new_capacity; i++)
    {
        misc_list bucket = NULL;
        if (!misc_vector_pushback(new_buckets, &bucket))
        {
            misc_vector_destroy(new_buckets);
            return 0;
        }
    }

    for (size_t i = 0; i < old_capacity; i++)
    {
        misc_list *old_bucket = (misc_list*)misc_vector_get(ht->buckets, i);
        if (*old_bucket == NULL) continue;

        size_t list_size = misc_list_size(*old_bucket);
        for (size_t j = 0; j < list_size; j++)
        {
            misc_htable_entry *entry = (misc_htable_entry*)misc_list_get(*old_bucket, j);
            
            size_t hash = ht->hash_func(entry->key);
            size_t new_bucket_idx = hash % new_capacity;

            misc_list *new_bucket = (misc_list*)misc_vector_get(new_buckets, new_bucket_idx);
            
            if (*new_bucket == NULL)
            {
                *new_bucket = misc_list_create(sizeof(misc_htable_entry));
                if (*new_bucket == NULL)
                {
                    for (size_t k = 0; k < new_capacity; k++)
                    {
                        misc_list *b = (misc_list*)misc_vector_get(new_buckets, k);
                        if (*b != NULL) misc_list_destroy(*b);
                    }
                    misc_vector_destroy(new_buckets);
                    return 0;
                }
            }

            if (!misc_list_pushback(*new_bucket, entry))
            {
                for (size_t k = 0; k < new_capacity; k++)
                {
                    misc_list *b = (misc_list*)misc_vector_get(new_buckets, k);
                    if (*b != NULL) misc_list_destroy(*b);
                }
                misc_vector_destroy(new_buckets);
                return 0;
            }
        }
    }

    for (size_t i = 0; i < old_capacity; i++)
    {
        misc_list *old_bucket = (misc_list*)misc_vector_get(ht->buckets, i);
        if (*old_bucket != NULL)
        {
            misc_list_destroy(*old_bucket);
        }
    }
    misc_vector_destroy(ht->buckets);

    ht->buckets = new_buckets;
    ht->capacity = new_capacity;

    return 1;
}

int misc_htable_put(misc_htable ht, const void *key, const void *value)
{
    if (ht == NULL || key == NULL || value == NULL) return 0;

    double load_factor = (double)ht->size / (double)ht->capacity;
    if (load_factor >= LOAD_FACTOR_THRESHOLD)
    {
        if (!_misc_htable_rehash(ht))
        {
            return 0;
        }
    }

    size_t hash = ht->hash_func(key);
    size_t bucket_idx = hash % ht->capacity;

    misc_list *bucket = (misc_list*)misc_vector_get(ht->buckets, bucket_idx);
    
    if (*bucket == NULL)
    {
        *bucket = misc_list_create(sizeof(misc_htable_entry));
        if (*bucket == NULL) return 0;
    }

    size_t list_size = misc_list_size(*bucket);
    for (size_t i = 0; i < list_size; i++)
    {
        misc_htable_entry *entry = (misc_htable_entry*)misc_list_get(*bucket, i);
        if (ht->key_cmp(entry->key, key) == 0)
        {
            memcpy(entry->value, value, ht->value_size);
            return 1;
        }
    }

    misc_htable_entry new_entry;
    new_entry.key = malloc(ht->key_size);
    new_entry.value = malloc(ht->value_size);
    
    if (new_entry.key == NULL || new_entry.value == NULL)
    {
        if (new_entry.key != NULL) free(new_entry.key);
        if (new_entry.value != NULL) free(new_entry.value);
        return 0;
    }

    memcpy(new_entry.key, key, ht->key_size);
    memcpy(new_entry.value, value, ht->value_size);

    if (!misc_list_pushback(*bucket, &new_entry))
    {
        free(new_entry.key);
        free(new_entry.value);
        return 0;
    }

    ht->size++;
    return 1;
}

int misc_htable_remove(misc_htable ht, const void *key)
{
    if (ht == NULL || key == NULL) return 0;

    size_t hash = ht->hash_func(key);
    size_t bucket_idx = hash % ht->capacity;

    misc_list *bucket = (misc_list*)misc_vector_get(ht->buckets, bucket_idx);
    if (*bucket == NULL) return 0;

    size_t list_size = misc_list_size(*bucket);
    for (size_t i = 0; i < list_size; i++)
    {
        misc_htable_entry *entry = (misc_htable_entry*)misc_list_get(*bucket, i);
        if (ht->key_cmp(entry->key, key) == 0)
        {
            free(entry->key);
            free(entry->value);
            misc_list_remove(*bucket, i, NULL);
            ht->size--;
            return 1;
        }
    }

    return 0;
}
