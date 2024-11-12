#ifndef USERDATA_H
#define USERDATA_H

#include "exercises.h"
#include<stdio.h>
#include<math.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#ifdef _WIN32
    #include <direct.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

// #define HASHMAP_SIZE 3072
/** @brief Initial capacity of the hashmap */
#define INITIAL_SIZE 16
/** @brief Maximum load factor before resizing (75%) */
#define MAX_LOAD_FACTOR 0.75

// For the userdata username and file
#define MAX_LENGTH 17  // 16 chars + '\0'
#define USER_ACCOUNTS_FILE "accounts.dat"
#define USER_FILES_DIR "userfiles"

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


#endif //USERDATA_H
