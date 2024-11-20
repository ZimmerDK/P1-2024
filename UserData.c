#include "UserData.h"
#include "exercises.h"
#include "P1.h"

/**@brief Struct to track changes in workout performance */
typedef struct workout_result_t {
    int repChange;        // Change in number of reps
    double weightChange;  // Change in weight used
} workout_result_t;

int UserData_main(UserPreferences_t* userprefs) {
    // Ensure user directory exists before opening files
    if (!ensure_user_directory()) {
        printf("Error: Could not create user directory\n");
        return 1;
    }

    // Construct path to accounts file
    char accounts_path[MAX_LENGTH + 15];
    snprintf(accounts_path, sizeof(accounts_path), "%s/%s", USER_FILES_DIR, USER_ACCOUNTS_FILE);

    // Open accounts file in binary read/write mode
    FILE* accountsFILE = fopen(accounts_path, "rb+");
    if (!accountsFILE) {
        printf("Error: Could not open accounts file\n");
        return 1;
    }

    FILE* userFILE = NULL;
    char input[MAX_LENGTH];

    // Initialize username hashmap for quick lookups
    HashMap_t* map = createHashMap();
    if (!map) {
        printf("Error: Could not create hashmap\n");
        fclose(accountsFILE);
        return 1;
    }

    // Load existing accounts into hashmap
    int m = startHashMap(accountsFILE, map);

    // Main program loop
    int running = 1;
    while (running) {
        login_screen:
        printf("\nLogin or signup? (or 'exit' to quit)\n");
        printf("Type 'login' if you already have an account.\n");
        printf("Type 'signup' if you want to create a new account.\n");

        if (scanf("%s", input) != 1) {
            printf("Error reading input\n");
            continue;
        }

        // Convert input to lowercase
        for(int n = 0; input[n]; n++) {
            input[n] = tolower(input[n]);
        }

        // Handle signup process
        if (strcmp(input, "signup") == 0) {
            printf("\nEnter your desired username (Max %d characters): ", MAX_LENGTH-1);
            if (scanf("%s", input) != 1) {
                printf("Error reading username\n");
                continue;
            }
            // Convert username to lowercase
            for(int n = 0; input[n]; n++) {
                input[n] = tolower(input[n]);
            }

            // Validate username length
            if (strlen(input) >= MAX_LENGTH) {
                printf("\nUsername too long! Maximum %d characters.\n", MAX_LENGTH-1);
                continue;
            }

            // Check if username already exists
            if (userVerify(input, map)) {
                printf("\nUsername already exists!\n");
            } else {
                // Reopen file in append mode for writing new user
                fclose(accountsFILE);
                snprintf(accounts_path, sizeof(accounts_path), "%s/%s", USER_FILES_DIR, USER_ACCOUNTS_FILE);
                accountsFILE = fopen(accounts_path, "a");
                if (!accountsFILE) {
                    printf("Error reopening accounts file for writing\n");
                    break;
                }

                // Write new username to accounts file
                fprintf(accountsFILE, "%s\n", input);
                fflush(accountsFILE);

                // Create user's data file
                char filepath[MAX_LENGTH + 15];
                snprintf(filepath, sizeof(filepath), "%s/%s.dat", USER_FILES_DIR, input);
                userFILE = fopen(filepath, "w+");

                // Initialize user preferences and exercise data
                user_setup(userFILE);
                fill_user_data(userFILE);

                // Add new user to hashmap
                if (userFILE) {
                    set(map, input, m * MAX_LENGTH);
                    m++;
                    printf("\nAccount created successfully!\n");
                } else {
                    printf("\nError creating account!\n");
                }

                fflush(userFILE);

                // Reopen accounts file in read mode
                fclose(accountsFILE);
                accountsFILE = fopen(accounts_path, "r");
            }
        }
        // Handle login process
        else if (strcmp(input, "login") == 0) {
            printf("\nEnter your username: ");
            if (scanf(" %s", input) != 1) {
                printf("Error reading username\n");
                continue;
            }
            // Convert username to lowercase
            for(int n = 0; input[n]; n++) {
                input[n] = tolower(input[n]);
            }

            // Verify user exists and handle login
            if (userVerify(input, map)) {
                printf("\nLogged in as %s!\n", input);
                char filepath[MAX_LENGTH + 15];
                snprintf(filepath, sizeof(filepath), "%s/%s.dat", USER_FILES_DIR, input);
                userFILE = fopen(filepath, "rb+");

                // Read and display user preferences
                *userprefs = read_user_preferences(userFILE);

                // Load user data into exercise structures
                parse_user_data(exercises_c, userFILE);

                // Exercise data test loop
                /*
                do {
                    char input_exercise[MAX_LENGTH+10];
                    printf("Input desired exercise to test: ");
                    scanf(" %s", &input_exercise);
                    if (input_exercise == "exit") {
                        break;
                    }
                    // Read and modify exercise data (test functionality)
                    user_exercise_data_t input_exercise_data = read_user_data(userFILE, 1);
                    printf("%s\n", input_exercise);
                    printf("Weight: %.2f kg\n", input_exercise_data.weight);
                    printf("Reps: %d\n", input_exercise_data.reps);

                    input_exercise_data.reps += 10;
                    input_exercise_data.weight += 100;
                    write_user_data(userFILE, 1, input_exercise_data);
                    fflush(userFILE);

                } while (1); */

                fclose(userFILE);

                if (!userFILE) {
                    printf("Error opening user file\n");
                }
                break;  // Successfully logged in
            } else {
                printf("\nUsername does not exist!\n");
                goto login_screen;
            }
        }
        else if (strcmp(input, "exit") == 0) {
            break;
        } else {
            printf("\nInvalid input!\n");
            goto login_screen;
        }
    }

    // Cleanup resources
    if (userFILE) fclose(userFILE);
    if (accountsFILE) fclose(accountsFILE);
    freeHashMap(map);
    return 0;
}

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

/**@brief Verifies whether a username exists in the hashmap
 * @param usernameInput The username to verify
 * @param map The hashmap containing valid usernames
 * @return int 1 if username exists, 0 if it doesn't exist */
int userVerify(char* usernameInput, HashMap_t* map) {
    int value = get(map, usernameInput);
    return (value != -1);
}

/**@brief Reads usernames from a file and stores them in a hashmap
 * @param accountsFILE The open file containing the user accounts
 * @param map The hashmap to store the usernames and their file positions
 * @return int The number of usernames loaded into the hashmap */
int startHashMap(FILE* accountsFILE, HashMap_t* map) {
    char tempUsername[MAX_LENGTH];
    int m = 0;

    // Read line by line
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

        printf("Read username: '%s'\n", tempUsername);
        set(map, tempUsername, m * MAX_LENGTH);
        m++;
    }
    printf("\nLoaded %d usernames into the hashmap\n", m);
    return m;
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

/**@brief Creates a new user and adds them to the system
 * @param accountsFILE File pointer to the accounts database
 * @param username Username to create (max MAX_LENGTH characters)
 * @param map Hashmap containing existing usernames
 * @return FILE* Pointer to the newly created user file, NULL if creation fails */
FILE* create_new_user(FILE* accountsFILE, char username[MAX_LENGTH], HashMap_t* map) {
    if (!ensure_user_directory()) {
        printf("Error: Could not create user directory\n");
        return NULL;
    }

    long position = ftell(accountsFILE);
    fwrite(username, MAX_LENGTH, 1, accountsFILE);
    fflush(accountsFILE);

    set(map, username, position);

    char filepath[MAX_LENGTH + 15];
    snprintf(filepath, sizeof(filepath), "%s/%s.dat", USER_FILES_DIR, username);
    FILE* userFILE = fopen(filepath, "w+");

    return userFILE;
}

/**@brief Sets up user preferences for a new account
 * @param userFILE Pointer to the user's data file where preferences will be stored
 * @note Prompts user for days (1-7) and time (15-120 minutes) preferences
 * @note Validates input and writes preferences to file */
void user_setup(FILE* userFILE) {
    UserPreferences_t prefs;

    printf("\nWelcome! Let's set up your preferences.\n");

    // Get days input with input validation
    do {
        printf("Enter preferred number of days you would like to workout(1-7): ");
        if (scanf("%d", &prefs.days) != 1 || prefs.days < 1 || prefs.days > 7) {
            printf("\nInvalid input! Please enter a number between 1 and 7.\n");
            while (getchar() != '\n'); // Clear input buffer for retry
            continue;
        }
        break;
    } while (1);

    // Get time input with input validation
    do {
        printf("Enter preferred time in minutes (15-120): ");
        if (scanf("%d", &prefs.time) != 1 || prefs.time < 15 || prefs.time > 120) {
            printf("\nInvalid input! Please enter a number between 15 and 120.\n");
            while (getchar() != '\n'); // Clear input buffer for retry
            continue;
        }
        break;
    } while (1);

    // Write preferences to file with error checking
    if (fwrite(&prefs.days, sizeof(int), 1, userFILE) != 1) {
        printf("\nError writing preferences to file!\n");
        return;
    }

    if (fwrite(&prefs.time, sizeof(int), 1, userFILE) != 1) {
        printf("\nError writing preferences to file!\n");
        return;
    }

    fflush(userFILE); // Ensure data is written to disk
    printf("\nPreferences saved successfully!\n");
}

/**@brief Initializes exercise data for all exercises in the exercises_c array
 * @param userFILE Pointer to the file where exercise data will be written
 * @note Writes weight (double) and reps (int) for each exercise
 * @note Skips past user preferences section before writing exercise data */
void fill_user_data(FILE* userFILE) {
    const size_t RECORD_SIZE = sizeof(double) + sizeof(int);
    const size_t SKIP_PREFS = sizeof(UserPreferences_t);

    // Iterate through all exercises in exercises_c array
    for(int i = 0; i < sizeof(exercises_c) / sizeof(exercise_t); i++) {
        // Position file pointer after preferences section
        fseek(userFILE, SKIP_PREFS + i * RECORD_SIZE, SEEK_SET);

        // Create test workout result (currently using placeholder values)
        workout_result_t workoutResult = {i,i};

        // Initialize exercise data with base values
        exercise_data_t current_exercise_data = {
            .weight = 20.0, // Base weight for testing
            .reps = exercises_c[i].min_reps,
        };

        // Apply workout changes
        current_exercise_data.weight += workoutResult.weightChange;
        current_exercise_data.reps += workoutResult.repChange;

        printf("Writing data for %s at index %d\n", exercises_c[i].name, i);

        // Write weight and reps with error checking
        if (fwrite(&current_exercise_data.weight, sizeof(double), 1, userFILE) != 1) {
            printf("\nError writing weight to file!\n");
        }
        if (fwrite(&current_exercise_data.reps, sizeof(int), 1, userFILE) != 1) {
            printf("\nError writing reps to file!\n");
        }
    }

    fflush(userFILE);
    printf("\nUser Exercise Data saved successfully!\n");
}

/**@brief Parses user data from the user's file and assigns it to exercise structures
 * @param exercises Pointer to the exercise array to update with user data */
void parse_user_data(exercise_t exercises[], FILE* userFILE) {
    for (int i = 0; i < AMOUNT_EXERCISES; i++) {
        user_exercise_data_t data = read_user_data(userFILE, i);
        exercises[i].user_exercise_data = malloc(sizeof(exercise_data_t));
        exercises[i].user_exercise_data->weight = data.weight;
        exercises[i].user_exercise_data->reps = data.reps;
        exercises[i].user_exercise_data->exercise = &exercises[i];
    }
}

/**@brief Retrieves exercise data for a specific exercise from the user's file
 * @param userFILE Pointer to the file containing exercise data
 * @param exercise_index Index of the desired exercise in exercises_c array
 * @return user_exercise_data_t structure containing weight, reps, and index
 * @note Returns zeroed structure if any error occurs */
user_exercise_data_t read_user_data(FILE* userFILE, int exercise_index) {
    user_exercise_data_t data = {0};
    const size_t RECORD_SIZE = sizeof(double) + sizeof(int);
    const size_t SKIP_PREFS = sizeof(UserPreferences_t);

    // Validate file pointer
    if (userFILE == NULL) {
        printf("Error: Invalid file pointer\n");
        return data;
    }

    // Calculate file position for desired exercise
    long position = SKIP_PREFS + exercise_index * RECORD_SIZE;

    // Seek to exercise position
    if (fseek(userFILE, position, SEEK_SET) != 0) {
        printf("Error: Could not seek to exercise position\n");
        return data;
    }

    // Read weight and reps with error checking
    if (fread(&data.weight, sizeof(double), 1, userFILE) != 1) {
        printf("Error reading weight for exercise %d (%s)\n",
               exercise_index, exercises_c[exercise_index].name);
        return data;
    }

    if (fread(&data.reps, sizeof(int), 1, userFILE) != 1) {
        printf("Error reading reps for exercise %d (%s)\n",
               exercise_index, exercises_c[exercise_index].name);
        return data;
    }

    data.index_number = exercise_index;
    return data;
}

/**@brief Reads user preferences from the data file
 * @param userFILE Pointer to the file containing user preferences
 * @return UserPreferences_t structure containing days and time preferences
 * @note Returns zeroed structure if any error occurs */
UserPreferences_t read_user_preferences(FILE* userFILE) {
    UserPreferences_t data = {0};

    // Seek to start of file where preferences are stored
    long position = fseek(userFILE, 0, SEEK_SET);

    if (fseek(userFILE, position, SEEK_SET) != 0) {
        printf("Error: Could not seek to exercise position\n");
        return data;
    }

    // Read days and time preferences with error checking
    if (fread(&data.days, sizeof(int), 1, userFILE) != 1) {
        printf("Error reading days for User Prefs\n");
        return data;
    }

    if (fread(&data.time, sizeof(int), 1, userFILE) != 1) {
        printf("Error reading time for User Prefs\n");
        return data;
    }

    return data;
}

/**@brief Updates exercise data for a specific exercise in the user's file
 * @param userFILE Pointer to the file containing exercise data
 * @param exercise_index Index of the exercise to update
 * @param new_data New exercise data to write (weight and reps)
 * @return 0 on success, -1 on error */
int write_user_data(FILE* userFILE, int exercise_index, user_exercise_data_t new_data) {
    const size_t RECORD_SIZE = sizeof(double) + sizeof(int);
    const size_t SKIP_PREFS = sizeof(UserPreferences_t);

    // Validate file pointer
    if (userFILE == NULL) {
        printf("Error: Invalid file pointer\n");
        return -1;
    }

    // Calculate position for target exercise
    long position = SKIP_PREFS + exercise_index * RECORD_SIZE;

    // Seek to exercise position
    if (fseek(userFILE, position, SEEK_SET) != 0) {
        printf("Error: Could not seek to exercise position\n");
        return -1;
    }

    // Write weight and reps with error checking
    if (fwrite(&new_data.weight, sizeof(double), 1, userFILE) != 1) {
        printf("Error writing weight for exercise %d (%s)\n",
               exercise_index, exercises_c[exercise_index].name);
        return -1;
    }

    if (fwrite(&new_data.reps, sizeof(int), 1, userFILE) != 1) {
        printf("Error writing reps for exercise %d (%s)\n",
               exercise_index, exercises_c[exercise_index].name);
        return -1;
    }

    fflush(userFILE); // Ensure data is written to disk
    return 0; // Success
}