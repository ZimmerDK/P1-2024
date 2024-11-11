
#ifndef USERDATA_H
#define USERDATA_H

#include "exercises.h"
#include<stdio.h>
#include<math.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

#define HASHMAP_SIZE 3072
#define MAX_LENGTH 20

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
    int size;
} HashMap;

const char* USER_DATA_FILE = "./userfiles/accounts.dat";

FILE* accountsFILE = fopen(USER_DATA_FILE, "r+");


#endif //USERDATA_H
