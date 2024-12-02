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


typedef struct user_exercise_data_t {
    int index_number;
    double weight;
    int reps;
    exercise_t* exercise;
} user_exercise_data_t;

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



int UserData_main();

int handle_signup(HashMap_t* map, char* accountsPath, FILE* accountsFILE, char* input);
int handle_login(HashMap_t* map, FILE* userFILE, char* input);

int userVerify(char* usernameInput, HashMap_t* map);
int startHashMap(FILE* accountsFILE, HashMap_t* map);

static int ensure_user_directory();

FILE* create_new_user(FILE* accountsFILE, char username[MAX_LENGTH], HashMap_t* map);

void user_setup(FILE* userFILE, int* days, int* time);

void parse_user_data(exercise_t* exercises);
void fill_user_data(FILE* userFILE, int days, int time);
void read_single_exercise_data(FILE* userFILE, int exercise_index, user_exercise_data_t* data);
user_file_header_prefs* read_user_preferences(FILE* userFILE);
workout_days_t* read_user_workout_data();

int update_user_exercise_data();

void read_previous_user_workout_data(user_file_exercise_data* data, int index);

int update_user_workout_data(workout_days_t *workout);

int write_user_data_post_workout();

void update_user_preferences(user_file_header_prefs* user_prefs);

// username malloc thing
extern char userprofile_path[MAX_LENGTH+15];  // Declare the variable as external

extern int userfile_workout_counter;

extern FILE* local_userFILE;
extern user_file_header_prefs* local_userPrefs;

// Function declarations
//void init_username(const char* input);
//void cleanup_username(void);


#endif //USERDATA_H
