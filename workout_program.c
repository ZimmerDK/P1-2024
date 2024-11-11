#include "workout_program.h"

#include <math.h>

#include "exercises.h"

void set_program_day(workout_days_t* program_day, int amountOfDays, int time,
    exercise_t exercise_compound_c[3], exercise_t exercise_secondary_c[3], exercise_t exercise_tertiary_c[3],
    int amountOfCompound, int amountOfSecondary, int amountOfTertiary);

int main(void) {
    int amountOfDays = 0;
    int time = 0;
    int amountOfCompound = 10;
    int amountOfSecondary = 3;
    int amountOfTertiary = 3;

    user_input(&amountOfDays, &time);
    workout_days_t* program_day = allocate_workout_program(amountOfDays);

    set_program_day(program_day, amountOfDays, time,
        exercise_compound_c, exercise_secondary_c, exercise_tertiary_c,
        amountOfCompound, amountOfSecondary, amountOfTertiary);

    print_function(program_day, amountOfDays, amountOfCompound, amountOfSecondary, amountOfTertiary);

    free(program_day);

    return 0;
}

workout_days_t* allocate_workout_program(int amountOfDays) {
    workout_days_t* program_day = (workout_days_t*)malloc(amountOfDays* sizeof(workout_days_t));
    if (program_day == NULL) {
        // Hukommelsen kunne ikke allokeres
        return NULL;
    }
    return program_day;
}

void user_input(int *amountOfDays, int *time) {
    printf("Days?\n");
    scanf(" %d", amountOfDays);
    printf("Time?\n");
    scanf(" %d", time);
}

void print_function(workout_days_t* program_day, int number_of_days,
    int amountOfCompound, int amountOfSecondary, int amountOfTertiary) {

    for(int i = 0; i < number_of_days; i++) {
        printf("\n");
        printf("Day: %d\n", i+1);
        for(int j = 0; j < amountOfCompound; j++) {
            printf("compound %d: %d\n", j+1, program_day[i].compound[j]);
        }
    }

    for(int i = 0; i < number_of_days; i++) {
        printf("\n");
        printf("Day: %d\n", i+1);
        for(int j = 0; j < amountOfSecondary; j++) {
            printf("secondary %d: %d\n", j+1, program_day[i].secondary[j]);
        }
    }

    for(int i = 0; i < number_of_days; i++) {
        printf("\n");
        printf("Day: %d\n", i+1);
        for(int j = 0; j < amountOfTertiary; j++) {
            printf("tertiary %d: %d\n", j+1, program_day[i].tertiary[j%3]);
        }
    }
}

void set_program_day(workout_days_t* program_day, int amountOfDays, int time,
    exercise_t exercise_compound_c[3], exercise_t exercise_secondary_c[3], exercise_t exercise_tertiary_c[3],
    int amountOfCompound, int amountOfSecondary, int amountOfTertiary) {
    for(int i = 0; i < amountOfDays; i++) {
        for(int j = 0; j < amountOfCompound; j++) {
            program_day[i].compound[j] = 0;
        }
        for(int j = 0; j < amountOfSecondary; j++) {
            program_day[i].secondary[j] = 0;
        }
        for(int j = 0; j < amountOfTertiary; j++) {
            program_day[i].tertiary[j] = 0;
        }
    }

    int maxTime = amountOfDays * time;

    int i = 0;
    int day = 0;

    while((amountOfCompound > i) && (maxTime > 0))
    {
        program_day[day%amountOfDays].compound[i%amountOfCompound] = 1;
        i++;
        day++;
        maxTime -= 15;
    }

    i=0;
    while((amountOfSecondary > i) && (maxTime > 0))
    {
        program_day[day%amountOfDays].secondary[i%amountOfSecondary] = 1;
        i++;
        day++;
        maxTime -= 12;
    }

    i=0;
    while((amountOfTertiary > i) && (maxTime > 0))
    {
        program_day[day%amountOfDays].tertiary[i%amountOfTertiary] = 1;
        i++;
        day++;
        maxTime -= 9;
    }
}