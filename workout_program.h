#include "P1.h"
#include <stdio.h>
#include <stdlib.h>
#ifndef WORKOUT_PROGRAM_H
#define WORKOUT_PROGRAM_H

#endif //WORKOUT_PROGRAM_H

const int amountOfCompound = 10;
const int amountOfSecondary = 10;
const int amountOfTertiary = 10;

typedef struct workout_days_t {
    int compound[amountOfCompound];
    int secondary[amountOfSecondary];
    int tertiary[amountOfTertiary];

} workout_days_t;

workout_days_t* allocate_workout_program(int number_of_days);

void user_input(int *amountOfDays, int *time);

void print_function(workout_days_t program_day[], int number_of_days,
    int amountOfCompound, int amountOfSecondary, int amountOfTertiary);