#ifndef USERDATA_H
#define USERDATA_H

#include "workout_program.h"
#include "exercises.h"
#include "hashmap.h"
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



// For the userdata username and file
#define MAX_LENGTH 17  // 16 chars + '\0'
#define USER_ACCOUNTS_FILE "accounts.dat"
#define USER_FILES_DIR "./userfiles"
#define DEFAULT_VALUE 0

#define USERDATA_SUCCESS 0x0
#define USERDATA_FAILURE 0x1

#define USERDATA_CONTEXT_VALID (char)0x0
#define USERDATA_CONTEXT_INVALID (char)0x1

typedef struct {
    FILE* userFILE;
    user_file_header_prefs* userPrefs;
    char contextExists;
} user_context_t;


typedef struct {
    int reps;
    double weight;
} user_file_exercise_data;


typedef struct {
    workout_days_t* workout;
    user_file_exercise_data exercise_data[AMOUNT_EXERCISES];
} user_file_header_data;

typedef struct {
	user_file_header_prefs prefs;
	user_file_header_data data;
} user_file_header;



int establish_userdata_context();

int handle_signup(HashMap_t* map, char* accountsPath, FILE* accountsFILE, char* input);
int handle_login(HashMap_t* map, FILE* userFILE, char* input);

int verify_user_existence(char* usernameInput, HashMap_t* map);
int startHashMap(FILE* accountsFILE, HashMap_t* map);

static int ensure_user_directory();

int validate_user_context(user_context_t* user_context);

FILE* create_new_user(FILE* accountsFILE, char username[MAX_LENGTH], HashMap_t* map);

void user_preferences_prompt(int* days, int* time);

void parse_user_data_into_array(exercise_t* exercises);

void fill_user_data(user_context_t* userContext, int days, int time);

void read_single_exercise_data(user_context_t* user_context, int exercise_index, user_exercise_data_t* data);

user_file_header_prefs* read_user_preferences(FILE* userFILE);

workout_days_t* read_user_workout_data(const user_context_t* user_context);

int update_user_exercise_data(const user_context_t* user_context);

int read_previous_user_workout_data(const user_context_t* user_context, user_file_exercise_data* data, int index);

int update_user_workout_data(const user_context_t* user_context, workout_days_t* workout);

int write_user_data_post_workout(const user_context_t* user_context);

int update_user_preferences(const user_context_t* user_context);

// username malloc thing
extern char userprofile_path[MAX_LENGTH+15];  // Declare the variable as external

extern int userfile_workout_counter;

extern user_context_t local_userContext;

// Function declarations
//void init_username(const char* input);
//void cleanup_username(void);


#endif //USERDATA_H
