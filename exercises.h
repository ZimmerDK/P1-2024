
#ifndef EXERCISES_H
#define EXERCISES_H
#include <stdio.h>

#endif //EXERCISES_H
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

// Exercises
const exercise_t exercise_c[3] = {
    {" Benchpress", 2.5, 8, 12, COMPOUND, 9.0, NULL},
    {" Squat", 2.5, 8, 12, COMPOUND, 9.0, NULL},
    {" Deadlift", 2.5, 8, 12, COMPOUND, 9.0, NULL},
};

