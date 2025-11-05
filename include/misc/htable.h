#pragma once
#ifndef HTABLE_H
#define HTABLE_H

#include <stddef.h>


/*
 * Opaque handle to a hash table instance.
 */
typedef struct misc_generic_hashtable* misc_htable;

/*
 * Hash function type.
 * Takes a pointer to a key and returns its hash value.
 */
typedef size_t (*misc_hash_fn)(const void *key);

/*
 * Key comparison function type.
 * Takes two key pointers and returns 0 if they are equal, non-zero otherwise.
 */
typedef int (*misc_key_cmp_fn)(const void *key1, const void *key2);

/*
 * Creates a new hash table with custom hash and key comparison functions.
 * The caller must provide both a hash function and a comparison function.
 * Returns NULL on allocation failure.
 */
misc_htable misc_htable_create(size_t value_size, size_t key_size, 
                               misc_hash_fn hash_func, 
                               misc_key_cmp_fn key_cmp);

/*
 * Creates a new hash table with string keys.
 * Uses FNV-1a hash function and strcmp for key comparison.
 * Returns NULL on allocation failure.
 */
misc_htable misc_htable_create_str(size_t value_size);

/*
 * Creates a new hash table with integer keys.
 * Uses a specialized integer hash function and integer comparison.
 * Returns NULL on allocation failure.
 */
misc_htable misc_htable_create_int(size_t value_size);

/*
 * Destroys the hash table and frees all associated memory.
 * All key-value pairs are removed and their memory is freed.
 * The hash table handle becomes invalid after this call.
 */
void misc_htable_destroy(misc_htable ht);

/*
 * Removes all key-value pairs from the hash table.
 * The hash table's size is set to zero but capacity remains unchanged.
 */
void misc_htable_clear(misc_htable ht);

/*
 * Returns the number of key-value pairs currently stored in the hash table.
 */
size_t misc_htable_size(const misc_htable ht);

/*
 * Checks if a key exists in the hash table.
 * Returns 1 if the key is found, 0 otherwise.
 */
int misc_htable_contains(const misc_htable ht, const void *key);

/*
 * Retrieves the value associated with a key.
 * Returns a pointer to the value if the key exists, NULL otherwise.
 * The pointer remains valid until the hash table is modified.
 */
void* misc_htable_get(const misc_htable ht, const void *key);

/*
 * Inserts or updates a key-value pair in the hash table.
 * If the key already exists, its value is updated.
 * If the key does not exist, a new entry is created.
 * Automatically rehashes when the load factor exceeds 0.75.
 * Returns 1 on success, 0 on allocation failure.
 */
int misc_htable_put(misc_htable ht, const void *key, const void *value);

/*
 * Removes a key-value pair from the hash table.
 * Returns 1 if the key was found and removed, 0 otherwise.
 */
int misc_htable_remove(misc_htable ht, const void *key);

#endif /* HTABLE_H */