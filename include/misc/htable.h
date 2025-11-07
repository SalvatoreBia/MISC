#pragma once
#ifndef HTABLE_H
#define HTABLE_H

#include <stddef.h>


/**
 * @brief Opaque handle to a hash table instance.
 */
typedef struct misc_generic_hashtable* misc_htable;

/**
 * @brief Hash function type.
 * @param key Pointer to the key to hash
 * @return Hash value for the key
 */
typedef size_t (*misc_hash_fn)(const void *key);

/**
 * @brief Key comparison function type.
 * @param key1 Pointer to first key
 * @param key2 Pointer to second key
 * @return 0 if keys are equal, non-zero otherwise
 */
typedef int (*misc_key_cmp_fn)(const void *key1, const void *key2);

/**
 * @brief Creates a new hash table with custom hash and key comparison functions.
 * @param value_size Size in bytes of each value
 * @param key_size Size in bytes of each key
 * @param hash_func Hash function to use for keys
 * @param key_cmp Key comparison function
 * @return Pointer to the new hash table, or NULL on allocation failure
 * @note The caller must provide both a hash function and a comparison function.
 */
misc_htable misc_htable_create(size_t value_size, size_t key_size, 
                               misc_hash_fn hash_func, 
                               misc_key_cmp_fn key_cmp);

/**
 * @brief Creates a new hash table with string keys.
 * @param value_size Size in bytes of each value
 * @return Pointer to the new hash table, or NULL on allocation failure
 * @note Uses FNV-1a hash function and strcmp for key comparison.
 */
misc_htable misc_htable_create_str(size_t value_size);

/**
 * @brief Creates a new hash table with integer keys.
 * @param value_size Size in bytes of each value
 * @return Pointer to the new hash table, or NULL on allocation failure
 * @note Uses a specialized integer hash function and integer comparison.
 */
misc_htable misc_htable_create_int(size_t value_size);

/**
 * @brief Destroys the hash table and frees all associated memory.
 * @param ht Hash table to destroy
 * @note All key-value pairs are removed and their memory is freed.
 * @note The hash table handle becomes invalid after this call.
 */
void misc_htable_destroy(misc_htable ht);

/**
 * @brief Removes all key-value pairs from the hash table.
 * @param ht Hash table to clear
 * @note The hash table's size is set to zero but capacity remains unchanged.
 */
void misc_htable_clear(misc_htable ht);

/**
 * @brief Returns the number of key-value pairs currently stored in the hash table.
 * @param ht Hash table to query
 * @return Number of key-value pairs
 */
size_t misc_htable_size(const misc_htable ht);

/**
 * @brief Checks if a key exists in the hash table.
 * @param ht Hash table to query
 * @param key Pointer to the key to search for
 * @return 1 if the key is found, 0 otherwise
 */
int misc_htable_contains(const misc_htable ht, const void *key);

/**
 * @brief Retrieves the value associated with a key.
 * @param ht Hash table to query
 * @param key Pointer to the key to search for
 * @return Pointer to the value if the key exists, NULL otherwise
 * @warning The pointer remains valid until the hash table is modified.
 */
void* misc_htable_get(const misc_htable ht, const void *key);

/**
 * @brief Inserts or updates a key-value pair in the hash table.
 * @param ht Hash table to modify
 * @param key Pointer to the key
 * @param value Pointer to the value
 * @return 1 on success, 0 on allocation failure
 * @note If the key already exists, its value is updated.
 * @note If the key does not exist, a new entry is created.
 * @note Automatically rehashes when the load factor exceeds 0.75.
 */
int misc_htable_put(misc_htable ht, const void *key, const void *value);

/**
 * @brief Removes a key-value pair from the hash table.
 * @param ht Hash table to modify
 * @param key Pointer to the key to remove
 * @return 1 if the key was found and removed, 0 otherwise
 */
int misc_htable_remove(misc_htable ht, const void *key);

#endif /* HTABLE_H */