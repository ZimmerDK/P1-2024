#ifndef EXERCISES_H
#define EXERCISES_H
//#include "workout_program.h"
#include <stdio.h>
//
#define AMOUNT_COMPOUND 5
#define AMOUNT_SECONDARY 6
#define AMOUNT_TERTIARY 9
//
typedef enum exercise_type_e {
    COMPOUND, SECONDARY, ISOLATION
} exercise_type_e;

typedef struct exercise_data_t exercise_data_t;

// Exercise constant
typedef struct exercise_t {
    char name[30];
    double weight_step;  // eg 2 or 2.5 etc
    int min_reps, max_reps;
    exercise_type_e type;
    double est_intensity;  //
    exercise_data_t* user_exercise_data;
} exercise_t;

// Exercise User Data
struct exercise_data_t {
    int reps;
    double weight;
    exercise_t* exercise;
 };

extern const exercise_t exercise_compound_c[AMOUNT_COMPOUND];
extern const exercise_t exercise_secondary_c[AMOUNT_SECONDARY];
extern const exercise_t exercise_tertiary_c[AMOUNT_TERTIARY];

// Exercises

#endif


