#include "UserData.h"
#include <sys/stat.h>

/**@brief Creates and initializes a new hashmap
 * @return HashMap* Pointer to the newly created hashmap, or NULL if allocation fails */
HashMap* createHashMap() {
    HashMap* map = malloc(sizeof(HashMap));
    if (!map) return NULL;

    map->capacity = INITIAL_SIZE;
    map->entries = calloc(INITIAL_SIZE, sizeof(HashMapEntry));
    if (!map->entries) {
        free(map);
        return NULL;
    }
    map->size = 0;
    return map;
}

/**@brief Computes the hash value for a given string key using DJB2 algorithm.
 *
 * @param key The string key to hash
 * @return unsigned int The computed hash value */
unsigned int hash(const char* key) {
    unsigned int hash = 5381;  // DJB2 algorithm starting value
    int c;
    while ((c = *key++)) {
        // hash * 33 + c: The "magic number" 33 has been found to work well in practice
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

/**@brief Resizes the hashmap when it becomes too full
 *
 * Doubles the capacity of the hashmap and rehashes all existing entries.
 * This maintains the hashmap's performance characteristics.
 * @param map Pointer to the hashmap to resize
 * @return int 1 on success, 0 on failure (memory allocation error) */
static int resizeHashMap(HashMap* map) {
    size_t newCapacity = map->capacity * 2;
    HashMapEntry* newEntries = calloc(newCapacity, sizeof(HashMapEntry));
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

/**@brief Inserts or updates a key-value pair in the hashmap.
 *
 * If the key already exists, its value is updated. If the key is new, it is inserted into the hashmap. The hashmap will automatically resize
 * if it becomes too full (load factor > MAX_LOAD_FACTOR).
 * @param map The hashmap to modify
 * @param key The key to insert/update
 * @param value The value to associate with the key */
void set(HashMap* map, char* key, int value) {
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
            // Free old key to prevent memory leak
            free(map->entries[index].key);
            map->entries[index].key = strdup(key);
            map->entries[index].value = value;
            return;
        }
        // Move to next position
        index = (index + 1) % map->capacity;
    } while (index != originalIndex);

    // If we get here, the hashmap is full (shouldn't happen due to resize)
    printf("Error: Hashmap is full\n");
}

/**@brief Retrieves the value associated with a key from the hashmap
 *
 * Uses linear probing to handle collisions. If the key is not found,
 * returns -1 as an error value.
 *
 * @param map Pointer to the hashmap
 * @param key The string key to look up
 * @return int The value associated with the key, or -1 if key not found */
int get(HashMap* map, char* key) {
    unsigned int index = hash(key) % map->capacity;
    unsigned int originalIndex = index;

    do {
        if (map->entries[index].key == NULL) {
            return -1;  // Empty slot means key doesn't exist
        }
        if (strcmp(map->entries[index].key, key) == 0) {
            return map->entries[index].value;  // Found the key
        }
        // Move to next position
        index = (index + 1) % map->capacity;
    } while (index != originalIndex);

    return -1;  // Key not found
}

/**@brief Frees all memory associated with the hashmap
 *
 * Properly frees all allocated strings and the hashmap structure itself.
 * After calling this function, the hashmap pointer should not be used.
 *
 * @param map Pointer to the hashmap to free */
void freeHashMap(HashMap* map) {
    // Iterate through all entries in the hashmap
    for (size_t i = 0; i < map->capacity; i++) {
        // Check if the current entry has a non-NULL key
        if (map->entries[i].key != NULL) {
            free(map->entries[i].key);  // Free the dynamically allocated key string
        }
    }
    free(map->entries); // Free the memory used for the entry array
    free(map);          // Finally, free the hashmap structure itself
}

/**@brief Verifies whether a username exists in the hashmap
 *
 * @param usernameInput The username to verify
 * @param map The hashmap containing valid usernames
 * @return int 1 if username exists, 0 if it doesn't exist */
int userVerify(char* usernameInput, HashMap* map) {
    int value = get(map, usernameInput);
    return (value != -1); // Return 1 if username exists, 0 if it does not
}

/**@brief Creates necessary directories for user files if they don't exist
 * @return int Returns 1 on success, 0 on failure */
static int ensure_user_directory() {
    struct stat st = {0};
    if (stat(USER_FILES_DIR, &st) == -1) {
        #ifdef _WIN32
        return mkdir(USER_FILES_DIR) == 0;
        #else
        return mkdir(USER_FILES_DIR, 0700) == 0;
        #endif
    }
    return 1;
}

/**@brief Creates a new user and adds them to the system.
 *
 * @param accountsFILE File pointer to the accounts database
 * @param username Username to create (max MAX_LENGTH characters)
 * @param map Hashmap containing existing usernames
 * @return FILE* Pointer to the newly created user file, NULL if creation fails */
FILE* create_new_user(FILE* accountsFILE, char username[MAX_LENGTH], HashMap* map) {
    // Ensure user directory exists
    if (!ensure_user_directory()) {
        printf("Error: Could not create user directory\n");
        return NULL;
    }

    // Get current position for the new user's index
    long position = ftell(accountsFILE);

    // Add username to accounts.dat
    fwrite(username, MAX_LENGTH, 1, accountsFILE);
    fflush(accountsFILE);  // Ensure data is written to disk

    // Add username to hashmap with its position as value
    set(map, username, position);

    // Create path for user's data file
    char filepath[MAX_LENGTH + 15];  // +15 for directory, separator, ".dat" and null terminator
    snprintf(filepath, sizeof(filepath), "%s/%s.dat", USER_FILES_DIR, username);
    FILE* userFILE = fopen(filepath, "w+");

    return userFILE;
}

/**
 * Writes workout data (number of days and workout time) to a user file.
 * @param userFILE The file pointer to write to
 * @param numDays Number of days worked out
 * @param workoutTime Time spent working out
 * @return 1 on success, 0 on failure
 */
int writeWorkoutData(FILE* userFILE, int value) {
    // Write each number on a separate line
    if (fprintf(userFILE, "%d\n", value) < 0) {
        printf("Error: Failed to write workout data\n");
        return 0;
    }

    // Ensure data is written to disk
    if (fflush(userFILE) != 0) {
        printf("Error: Failed to flush file buffer\n");
        return 0;
    }

    return 1;
}

int main() {
    // Ensure user directory exists before opening files
    if (!ensure_user_directory()) {
        printf("Error: Could not create user directory\n");
        return 1;
    }

    // Update accounts file path
    char accounts_path[MAX_LENGTH + 15];
    snprintf(accounts_path, sizeof(accounts_path), "%s/%s", USER_FILES_DIR, USER_ACCOUNTS_FILE);

    FILE* accountsFILE = fopen(accounts_path, "r+");
    if (!accountsFILE) {
        printf("Error: Could not open accounts file\n");
        return 1;
    }

    FILE* userFILE = NULL;
    char input[MAX_LENGTH];

    // Create username hashmap
    HashMap* map = createHashMap();
    if (!map) {
        printf("Error: Could not create hashmap\n");
        fclose(accountsFILE);
        return 1;
    }

    char tempUsername[MAX_LENGTH];
    int m = 0;

    // Read line by line instead of fixed-length records
    while (fgets(tempUsername, MAX_LENGTH, accountsFILE)) {
        // Remove newline character if present
        size_t len = strlen(tempUsername);
        if (len > 0 && (tempUsername[len-1] == '\n' || tempUsername[len-1] == '\r')) {
            tempUsername[len-1] = '\0';
        }
        // Remove second character of CRLF if present
        len = strlen(tempUsername);
        if (len > 0 && (tempUsername[len-1] == '\n' || tempUsername[len-1] == '\r')) {
            tempUsername[len-1] = '\0';
        }

        // Debug output
        printf("Read username: '%s'\n", tempUsername);

        // Store file position as value in hashmap
        set(map, tempUsername, m * MAX_LENGTH);
        m++;
    }
    printf("\nLoaded %d usernames into the hashmap\n", m);

    int running = 1;
    while (running) {
        login_screen:
        printf("\nLogin or signup? (or 'exit' to quit)\n");
        printf("Type 'login' if you already have an account.\n");
        printf("Type 'signup' if you want to create a new account.\n");
        // scanf("%s", input);

        if (scanf("%s", input) != 1) {
            printf("Error reading input\n");
            continue;
        }

        for(int n = 0; input[n]; n++) {
            input[n] = tolower(input[n]);
        }

        if (strcmp(input, "login") == 0) {
            printf("\nEnter your username: ");
            if (scanf(" %s", input) != 1) {
                printf("Error reading username\n");
                continue;
            }
            for(int n = 0; input[n]; n++) {
                input[n] = tolower(input[n]);
            }
            // Update the login section where userfile is opened:
            if (userVerify(input, map)) {
                printf("\nLogged in as %s!\n", input);
                char filepath[MAX_LENGTH + 15];
                snprintf(filepath, sizeof(filepath), "%s/%s.dat", USER_FILES_DIR, input);
                userFILE = fopen(filepath, "rb+");
                if (!userFILE) {
                    printf("Error opening user file\n");
                }
                break;  // Successfully logged in
            } else {
                printf("\nUsername does not exist!\n");
                goto login_screen;
            }
        } else if (strcmp(input, "signup") == 0) {
            printf("\nEnter your desired username (Max %d characters): ", MAX_LENGTH-1);
            if (scanf("%s", input) != 1) {
                printf("Error reading username\n");
                continue;
            }
            for(int n = 0; input[n]; n++) {
                input[n] = tolower(input[n]);
            }
            if (strlen(input) >= MAX_LENGTH) {
                printf("\nUsername too long! Maximum %d characters.\n", MAX_LENGTH-1);
                continue;
            }if (userVerify(input, map)) {
                printf("\nUsername already exists!\n");
            } else {
                // Reopen file in append mode for writing
                fclose(accountsFILE);
                snprintf(accounts_path, sizeof(accounts_path), "%s/%s", USER_FILES_DIR, USER_ACCOUNTS_FILE);
                accountsFILE = fopen(accounts_path, "a");
                if (!accountsFILE) {
                    printf("Error reopening accounts file for writing\n");
                    break;
                }

                // Write the new username with a newline
                fprintf(accountsFILE, "%s\n", input);
                fflush(accountsFILE);

                // Create user's data file
                char filepath[MAX_LENGTH + 15];
                snprintf(filepath, sizeof(filepath), "%s/%s.dat", USER_FILES_DIR, input);
                userFILE = fopen(filepath, "w+");

                if (userFILE) {
                    // Add to hashmap
                    set(map, input, m * MAX_LENGTH);
                    m++;
                    printf("\nAccount created successfully!\n");
                } else {
                    printf("\nError creating account!\n");
                }



                // Reopen in read mode for continued operation
                fclose(accountsFILE);
                accountsFILE = fopen(accounts_path, "r");
            }
        }
        else if (strcmp(input, "exit") == 0) {
            break;
        } else {
            printf("\nInvalid input!\n");
            goto login_screen;
        }
    }

    // These 'fclose' and 'free' should be at the very end of the main program
    // Cleanup
    if (userFILE) fclose(userFILE);
    if (accountsFILE) fclose(accountsFILE);
    freeHashMap(map);
    return 0;
}
