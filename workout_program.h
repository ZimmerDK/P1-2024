#pragma once

#include <stdio.h>
#include "exercises.h"

#include <math.h>

typedef struct workout_days_t {
    int compound[AMOUNT_COMPOUND];
    int secondary[AMOUNT_SECONDARY];
    int tertiary[AMOUNT_TERTIARY];

} workout_days_t;

typedef struct {
    int days;
    int time;
} UserPreferences_t;

workout_days_t* generate_workout_program(UserPreferences_t userprefs);

workout_days_t* allocate_workout_program(int number_of_days);

void user_input(int *amountOfDays, int *time);

void print_workout_program(workout_days_t* program_day, int number_of_days);

void print_function(exercise_t*, workout_days_t program_day[], int number_of_days);

