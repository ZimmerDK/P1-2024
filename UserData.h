#ifndef USERDATA_H
#define USERDATA_H

#include "exercises.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <time.h>

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
#define USER_FILES_DIR "./userfiles"

typedef struct account {
    char username[20];
    exercise_data_t exercise_user_data;
} account_t;

typedef struct {
    char* key;
    int value; // In this case, the value will be the index of the word in the wordlist
} HashMapEntry_t;

typedef struct {
    HashMapEntry_t* entries;
    size_t capacity;  // Current size of entries array
    size_t size;     // Number of items stored
} HashMap_t;

typedef struct {
    int days;
    int time;
} UserPreferences_t;

typedef struct user_exercise_data_t {
    int index_number;
    double weight;
    int reps;
} user_exercise_data_t;

int UserData_main(UserPreferences_t* userprefs);

HashMap_t* createHashMap();
unsigned int hash(const char* key);
static int resizeHashMap(HashMap_t* map);
void set(HashMap_t* map, char* key, int value);
int get(HashMap_t* map, char* key);
void freeHashMap(HashMap_t* map);
int userVerify(char* usernameInput, HashMap_t* map);
int startHashMap(FILE* accountsFILE, HashMap_t* map);
static int ensure_user_directory();
FILE* create_new_user(FILE* accountsFILE, char username[MAX_LENGTH], HashMap_t* map);
int writeWorkoutData(FILE* userFILE, int value);
void user_setup(FILE* userFILE);

void parse_user_data(exercise_t exercises[], FILE* userFILE);
void fill_user_data(FILE* userFILE);
user_exercise_data_t read_user_data(FILE* userFILE, int exercise_index);
UserPreferences_t read_user_preferences(FILE* userFILE);
int write_user_data(FILE* userFILE, int exercise_index, user_exercise_data_t new_data);

void calibrate_workout_routine(struct exercise_data_t* calibration_data);


// username malloc thing
extern char* username;  // Declare the variable as external

// Function declarations
void init_username(const char* input);
void cleanup_username(void);


#endif //USERDATA_H
