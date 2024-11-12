#ifndef USERDATA_H
#define USERDATA_H

#include "exercises.h"
#include<stdio.h>
#include<math.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

// #define HASHMAP_SIZE 3072
/** @brief Initial capacity of the hashmap */
#define INITIAL_SIZE 16
/** @brief Maximum load factor before resizing (75%) */
#define MAX_LOAD_FACTOR 0.75

// For the userdata username and file
#define MAX_LENGTH 17  // 16 chars + '\0'
/* #define USER_DATA_FILE "accounts.dat" */

typedef struct account {
    char username[20];
    exercise_data_t exercise_user_data;
} account;

typedef struct {
    char* key;
    int value; // In this case, the value will be the index of the word in the wordlist
} HashMapEntry;

typedef struct {
    HashMapEntry* entries;
    size_t capacity;  // Current size of entries array
    size_t size;     // Number of items stored
} HashMap;

const char* USER_DATA_FILE = "./userfiles/accounts.dat";



#endif //USERDATA_H
