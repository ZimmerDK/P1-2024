#include "hashmap.h"

/**@brief Creates and initializes a new hashmap for storing usernames
 * @return HashMap_t* Pointer to the newly created hashmap, NULL if allocation fails */
HashMap_t* createHashMap() {
    HashMap_t* map = malloc(sizeof(HashMap_t));
    if (!map) return NULL;

    map->capacity = INITIAL_SIZE;
    map->entries = calloc(INITIAL_SIZE, sizeof(HashMapEntry_t));
    if (!map->entries) {
        free(map);
        return NULL;
    }
    map->size = 0;
    return map;
}

/**@brief Computes the hash value for a given string key using DJB2 algorithm
 * @param key The string key to hash
 * @return unsigned int The computed hash value */
unsigned int hash(const char* key) {
    unsigned int hash = 5381;  // DJB2 algorithm starting value
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

/**@brief Resizes the hashmap when it becomes too full
 * @param map Pointer to the hashmap to resize
 * @return int 1 on success, 0 on failure (memory allocation error) */
static int resizeHashMap(HashMap_t* map) {
    size_t newCapacity = map->capacity * 2;
    HashMapEntry_t* newEntries = calloc(newCapacity, sizeof(HashMapEntry_t));
    if (!newEntries) return 0;

    // Rehash all existing entries
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->entries[i].key) {
            unsigned int index = hash(map->entries[i].key) % newCapacity;

            // Find new position using linear probing
            while (newEntries[index].key != NULL) {
                index = (index + 1) % newCapacity;
            }

            newEntries[index].key = map->entries[i].key;
            newEntries[index].value = map->entries[i].value;
        }
    }

    free(map->entries);
    map->entries = newEntries;
    map->capacity = newCapacity;
    return 1;
}

/**@brief Inserts or updates a key-value pair in the hashmap
 * @param map The hashmap to modify
 * @param key The key to insert/update
 * @param value The value to associate with the key */
void set(HashMap_t* map, char* key, int value) {
    // Check if resize is needed
    if ((float)map->size / map->capacity >= MAX_LOAD_FACTOR) {
        if (!resizeHashMap(map)) {
            printf("Error: Failed to resize hashmap\n");
            return;
        }
    }

    unsigned int index = hash(key) % map->capacity;
    unsigned int originalIndex = index;

    do {
        // Empty slot found
        if (map->entries[index].key == NULL) {
            map->entries[index].key = strdup(key);
            map->entries[index].value = value;
            map->size++;
            return;
        }
        // Key already exists
        if (strcmp(map->entries[index].key, key) == 0) {
            free(map->entries[index].key);
            map->entries[index].key = strdup(key);
            map->entries[index].value = value;
            return;
        }
        // Move to next position
        index = (index + 1) % map->capacity;
    } while (index != originalIndex);

    printf("Error: Hashmap is full\n");
}

/**@brief Retrieves the value associated with a key from the hashmap
 * @param map Pointer to the hashmap
 * @param key The string key to look up
 * @return int The value associated with the key, or -1 if key not found */
int get(HashMap_t* map, char* key) {
    unsigned int index = hash(key) % map->capacity;
    unsigned int originalIndex = index;

    do {
        if (map->entries[index].key == NULL) {
            return -1;  // Empty slot means key doesn't exist
        }
        if (strcmp(map->entries[index].key, key) == 0) {
            return map->entries[index].value;  // Found the key
        }
        index = (index + 1) % map->capacity;
    } while (index != originalIndex);

    return -1;  // Key not found
}

/**@brief Frees all memory associated with the hashmap
 * @param map Pointer to the hashmap to free */
void freeHashMap(HashMap_t* map) {
    for (size_t i = 0; i < map->capacity; i++) {
        if (map->entries[i].key != NULL) {
            free(map->entries[i].key);
        }
    }
    free(map->entries);
    free(map);
}