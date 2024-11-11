
#ifndef USERDATA_H
#define USERDATA_H

#define HASHMAP_SIZE 3072
#include <stdio.h>

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
