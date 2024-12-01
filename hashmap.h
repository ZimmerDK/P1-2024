#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// #define HASHMAP_SIZE 3072
/** @brief Initial capacity of the hashmap */
#define INITIAL_SIZE 16
/** @brief Maximum load factor before resizing (75%) */
#define MAX_LOAD_FACTOR 0.75

typedef struct {
    char* key;
    int value; // In this case, the value will be the index of the word in the wordlist
} HashMapEntry_t;

typedef struct {
    HashMapEntry_t* entries;
    size_t capacity;  // Current size of entries array
    size_t size;     // Number of items stored
} HashMap_t;


HashMap_t* createHashMap();
unsigned int hash(const char* key);
static int resizeHashMap(HashMap_t* map);
void set(HashMap_t* map, char* key, int value);
int get(HashMap_t* map, char* key);
void freeHashMap(HashMap_t* map);

