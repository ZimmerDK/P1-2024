#include "UserData.h"

/**
 * A simple hashmap implementation using linear probing for collision resolution.
 * The hashmap stores string keys mapped to integer values.
 */

/**
 * Creates and initializes a new hashmap.
 * @return A pointer to the newly created hashmap
 */
HashMap* createHashMap() {
    HashMap* map = malloc(sizeof(HashMap));
    map->entries = calloc(HASHMAP_SIZE, sizeof(HashMapEntry));  // Initialize all entries to 0/NULL
    map->size = 0;
    return map;
}

/**
 * Implements the DJB2 hash algorithm for strings.
 * @param key The string to hash
 * @return A hash value modulo the hashmap size
 */
unsigned int hash(char* key) {
    unsigned int hash = 5381;  // DJB2 algorithm starting value
    int c;

    while ((c = *key++)) {
        // hash * 33 + c: The "magic number" 33 has been found to work well in practice
        hash = ((hash << 5) + hash) + c;
    }

    return hash % HASHMAP_SIZE;
}

/**
 * Inserts or updates a key-value pair in the hashmap.
 * Uses linear probing to handle collisions.
 * @param map The hashmap to modify
 * @param key The key to insert/update
 * @param value The value to associate with the key
 */
void set(HashMap* map, char* key, int value) {
    unsigned int index = hash(key);

    // Case 1: Empty slot found
    if (map->entries[index].key == NULL) {
        map->entries[index].key = strdup(key);  // Make a copy of the key
        map->entries[index].value = value;
        map->size++;
    }
    // Case 2: Key already exists at this position
    else if (strcmp(map->entries[index].key, key) == 0) {
        map->entries[index].value = value;  // Update existing value
    }
    // Case 3: Collision occurred, use linear probing
    else {
        // Start at next position and wrap around if necessary
        for (int i = (index + 1) % HASHMAP_SIZE; i != index; i = (i + 1) % HASHMAP_SIZE) {
            if (map->entries[i].key == NULL) {
                // Found an empty slot
                map->entries[i].key = strdup(key);
                map->entries[i].value = value;
                map->size++;
                return;
            }
            else if (strcmp(map->entries[i].key, key) == 0) {
                // Found existing key
                map->entries[i].value = value;
                return;
            }
        }
        // If we get here, we've checked all positions and found no space
        printf("Error: Hashmap is full\n");
    }
}

/**
 * Retrieves the value associated with a key from the hashmap.
 * @param map The hashmap to search
 * @param key The key to look up
 * @return The value associated with the key, or -1 if the key is not found
 */
int get(HashMap* map, char* key) {
    unsigned int index = hash(key);

    // Case 1: Empty slot found (key doesn't exist)
    if (map->entries[index].key == NULL) {
        return -1;
    }
    // Case 2: Key found at hashed position
    else if (strcmp(map->entries[index].key, key) == 0) {
        return map->entries[index].value;
    }
    // Case 3: Key might be in another position due to collision
    else {
        // Linear probe until we find the key or an empty slot
        for (int i = (index + 1) % HASHMAP_SIZE; i != index; i = (i + 1) % HASHMAP_SIZE) {
            if (map->entries[i].key == NULL) {
                return -1;  // Empty slot means key doesn't exist
            }
            else if (strcmp(map->entries[i].key, key) == 0) {
                return map->entries[i].value;  // Found the key
            }
        }
        return -1;  // Searched entire table, key not found
    }
}
/**
 * Frees all memory used by a hashmap.
 * @param map The hashmap to free
 */
void freeHashMap(HashMap* map) {
    // Iterate through all entries in the hashmap
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        // Check if the current entry has a non-NULL key
        if (map->entries[i].key != NULL) {
            // Free the dynamically allocated key string
            free(map->entries[i].key);
        }
    }

    // Free the memory used for the entry array
    free(map->entries);

    // Finally, free the hashmap structure itself
    free(map);
}

/**
 * Verifies if a username exists in the hashmap.
 * @param usernameInput The username to verify
 * @param map The hashmap containing valid usernames
 * @return 1 if username is valid, 0 otherwise
 */
int userVerify(char* usernameInput, HashMap* map) {
    int index = get(map, usernameInput);
    if (index == -1) {
        printf("Invalid input.\n");
    } else {
        return 1;
    }
    return 0;
}

FILE* create_new_user(FILE* accountsFILE, char username[MAX_LENGTH], HashMap* map) {

        // Add username to accounts.dat
        fseek(accountsFILE, 0, SEEK_END);

        fwrite(username, MAX_LENGTH, 1, accountsFILE);

        // Add username to hashmap
        set(map, username, SEEK_END);

        // Make new file
        strcat(username, ".dat");
        FILE* userFILE = fopen(username, "w+");

        return userFILE;
    }

int main() {

    FILE* accountsFILE = fopen(USER_DATA_FILE, "r+");
    FILE* userFILE;

    char input[MAX_LENGTH];

    // Create username hashmap
    HashMap* map = createHashMap();
    char tempUsername[128];
    int m = 0;
    while (fscanf(accountsFILE, "%s", tempUsername) != EOF) {
        set(map, tempUsername, m);
        m++;
    }
    printf("\nLoaded %d usernames into the hashmap\n", m);

    login:
    printf("\nLogin or signup?");
    printf("\nType 'login' if you already have an account.");
    printf("\nType 'signup' if you want to create a new account.");
    scanf("%s", input);

    for(int n = 0; input[n]; n++) {
        input[n] = tolower(input[n]);
    }
    if (strcmp(input, "login") == 0) {
        printf("\nEnter your username:");
        scanf("%s", input);
        for(int n = 0; input[n]; n++) {
            input[n] = tolower(input[n]);
        }
        if (userVerify(input, map) == 1) {
            printf("\nLogged in as %s!", input);
            strcat(input, ".dat");
            FILE* userFILE = fopen(input, "rb+");
        } else {
            printf("\nUsername does not exist!");
            goto login;
        }
    } else if (strcmp(input, "signup") == 0) {
        printf("\nEnter your desired username (Max 16 characters):");
        scanf("%s", input);
        for(int n = 0; input[n]; n++) {
            input[n] = tolower(input[n]);
        }
        if (strlen(input) <= 16) {
            if (userVerify(input, map) == 0) {
                FILE* userFILE = create_new_user(accountsFILE, input, map);
                goto login;
            } else {
                printf("\nUsername already exists!");
                goto login;
            }
        }
        printf("\nError in signup case!\n");
    } else if (strcmp(input, "exit") == 0) {
        exit(EXIT_SUCCESS);
    } else {
        printf("\nInvalid input!\n");
        goto login;
    }

    // These fclose and free need to be in the end of the program
    //fclose(userFILE);
    fclose(accountsFILE);
    freeHashMap(map);
    return 0;
}
