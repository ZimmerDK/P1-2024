#include "workout_program.h"
#include <math.h>
#include "exercises.h"

void set_program_day(workout_days_t* program_day, int amountOfDays, int time );

/*int main(void) {
    int amountOfDays = 0;
    int time = 0;

    user_input(&amountOfDays, &time);
    workout_days_t* program_day = allocate_workout_program(amountOfDays);

    set_program_day(program_day, amountOfDays, time );

    print_function(exercise_compound_c, program_day, amountOfDays);

    free(program_day);

    return 0;
}*/

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

void print_function(exercise_t* exercise_compound, workout_days_t* program_day, int number_of_days) {

    for(int i = 0; i < number_of_days; i++) {
        printf("\n");
        printf("Day: %d\n", i+1);
        for(int j = 0; j < AMOUNT_COMPOUND; j++) {
            if ( program_day[i].compound[j] == 1 ) {
                printf("compound %d: %s\n", j+1, exercises_c[exercise_compound_c[j]].name);
            }
        }
        for(int j = 0; j < AMOUNT_SECONDARY; j++) {
            if ( program_day[i].secondary[j] == 1 ) {
                printf("secondary %d: %s\n", j+1, exercises_c[exercise_secondary_c[j]].name);
            }
        }
        for(int j = 0; j < AMOUNT_TERTIARY; j++) {
            if ( program_day[i].tertiary[j] == 1 ) {
                printf("tertiary %d: %s\n", j+1, exercises_c[exercise_tertiary_c[j]].name);
            }
        }
    }
}

void set_program_day(workout_days_t* program_day, int amountOfDays, int time) {
    for(int i = 0; i < amountOfDays; i++) {
        for(int j = 0; j < AMOUNT_COMPOUND; j++) {
            program_day[i].compound[j] = 0;
        }
        for(int j = 0; j < AMOUNT_SECONDARY; j++) {
            program_day[i].secondary[j] = 0;
        }
        for(int j = 0; j < AMOUNT_TERTIARY; j++) {
            program_day[i].tertiary[j] = 0;
        }
    }

    int i = 0;
    int day = 0;
    int counter = 0;

    int* timePerDay = (int*)malloc(sizeof(int)*amountOfDays);
    for(int i = 0; i < amountOfDays; i++) {
        timePerDay[i] = time;
    }


    while((AMOUNT_COMPOUND > i) && counter <= amountOfDays)
    {
        if(timePerDay[day%amountOfDays] >= 15) {
            program_day[day%amountOfDays].compound[i] = 1;
            i++;
            timePerDay[day%amountOfDays] -= 15;
        }
        else {
            counter++;
        }
        day++;
    }

    i = 0;
    counter = 0;
    while((AMOUNT_SECONDARY > i) && counter <= amountOfDays)
    {
        if(timePerDay[day%amountOfDays] >= 12) {
            program_day[day%amountOfDays].secondary[i] = 1;
            i++;
            timePerDay[day%amountOfDays] -= 12;
        }
        else {
            counter++;
        }
        day++;
    }
    counter = 0;
    i = 0;

    while((AMOUNT_TERTIARY > i) && counter <= amountOfDays)
    {
        if(timePerDay[day % amountOfDays] >= 9) {
            program_day[day%amountOfDays].tertiary[i] = 1;
            i++;
            timePerDay[day%amountOfDays] -= 9;
        }
        else {
            counter++;
        }
        day++;
    }
}
/* Bør man ikke tage det hvis man ikke kan kører hele programmet igennem? compound / secondary / tertiary */