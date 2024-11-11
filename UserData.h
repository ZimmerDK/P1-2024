//
// Created by magnu on 11/11/2024.
//

#ifndef USERDATA_H
#define USERDATA_H

#define HASHMAP_SIZE 3072

typedef struct {
    char* key;
    int value; // In this case, the value will be the index of the word in the wordlist
} HashMapEntry;

typedef struct {
    HashMapEntry* entries;
    int size;
} HashMap;

#endif //USERDATA_H
