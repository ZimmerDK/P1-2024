#ifndef EXERCISES_H
#define EXERCISES_H
//#include "workout_program.h"
#include <stdio.h>
//
#define AMOUNT_EXERCISES 18

#define AMOUNT_COMPOUND 5
#define AMOUNT_SECONDARY 4
#define AMOUNT_TERTIARY 9
//
typedef enum exercise_type_e {
    COMPOUND, SECONDARY, ISOLATION
} exercise_type_e;

typedef struct user_exercise_data_t user_exercise_data_t;

// Exercise constant
typedef struct exercise_t {
    char name[30];
    double weight_step;  // eg 2 or 2.5 etc
    int min_reps, max_reps;
    exercise_type_e type;
    double est_intensity;  //
    user_exercise_data_t* user_exercise_data;
} exercise_t;


extern exercise_t exercises_c[AMOUNT_EXERCISES];

extern const int exercise_compound_c[AMOUNT_COMPOUND];
extern const int exercise_secondary_c[AMOUNT_SECONDARY];
extern const int exercise_tertiary_c[AMOUNT_TERTIARY];

// Exercises

#endif


