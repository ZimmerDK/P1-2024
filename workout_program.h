#include <stdio.h>
#include <stdlib.h>
#include "exercises.h"
#include <math.h>
#pragma once
#ifndef WORKOUT_PROGRAM_H
#define WORKOUT_PROGRAM_H

#endif //WORKOUT_PROGRAM_H


typedef struct workout_days_t {
    int compound[AMOUNT_COMPOUND];
    int secondary[AMOUNT_SECONDARY];
    int tertiary[AMOUNT_TERTIARY];

} workout_days_t;

void generate_workout_program(workout_days_t* workout_program, int amountOfDays, int timeMins);

workout_days_t* allocate_workout_program(int number_of_days);

void user_input(int *amountOfDays, int *time);

void print_function(exercise_t*, workout_days_t program_day[], int number_of_days);
