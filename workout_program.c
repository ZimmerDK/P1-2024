#include "workout_program.h"
#include "exercises.h"

void set_program_day(workout_days_t* program_day, int amountOfDays, int time );

int main(void) {
    int amountOfDays = 0;
    int time = 0;

    user_input(&amountOfDays, &time);
    workout_days_t* program_day = allocate_workout_program(amountOfDays);

    set_program_day(program_day, amountOfDays, time );

    print_function(exercise_compound_c, program_day, amountOfDays);

    free(program_day);

    return 0;
}

//Function that allocates enough memory for all the structs of workout days. enough space for each day is alocated.
workout_days_t* allocate_workout_program(int amountOfDays) {
    workout_days_t* program_day = (workout_days_t*)malloc(amountOfDays* sizeof(workout_days_t));
    if (program_day == NULL) {
        // Hukommelsen kunne ikke allokeres
        printf("Not allocated memory in Workout Days!");
        exit(EXIT_FAILURE);
    }
    return program_day;
}

/*Function that promts the user for input of amout of days per week and time per day. It is saved in the variables
amountOfDays and time */

void user_input(int *amountOfDays, int *time) {
    printf("How many days per week?\n");
    scanf(" %d", amountOfDays);
    printf("How much time per day?\n");
    scanf(" %d", time);
}

/*Fuction that prints which exercises per day.
It uses the struct Exercise_t, the structs program_day and number_of_days */
void print_function(exercise_t*, workout_days_t* program_day, int number_of_days) {

    for(int i = 0; i < number_of_days; i++) {
        printf("\n");
        printf("Day: %d\n", i+1);

        //Prints the compound for the day
        for(int j = 0; j < AMOUNT_COMPOUND; j++) {
            if ( program_day[i].compound[j] == 1 ) {
                printf("compound %d: %s\n", j+1, exercise_compound_c[j].name);
            }
        }

        //Prints the secondary for the day
        for(int j = 0; j < AMOUNT_SECONDARY; j++) {
            if ( program_day[i].secondary[j] == 1 ) {
                printf("secondary %d: %s\n", j+1, exercise_secondary_c[j].name);
            }
        }

        //Prints the tertiary for the day
        for(int j = 0; j < AMOUNT_TERTIARY; j++) {
            if ( program_day[i].tertiary[j] == 1 ) {
                printf("tertiary %d: %s\n", j+1, exercise_tertiary_c[j].name);
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

    int DifferentProgramDays = 0;
    switch(amountOfDays) {
        case 1: DifferentProgramDays = 1; break;
        case 2: DifferentProgramDays = 2; break;
        case 3: DifferentProgramDays = 3; break;
        case 4: DifferentProgramDays = 2; break;
        case 5: DifferentProgramDays = 3; break;
        case 6: DifferentProgramDays = 3; break;
        case 7: DifferentProgramDays = 3; break;
        default: printf("input amount between 1 and 7");
    }

    int timePerDay[amountOfDays];
    for(int i = 0; i < DifferentProgramDays; i++) {
        timePerDay[i] = time;
    }

    const int maxAmountOfCompound = 3*DifferentProgramDays;
    int CompoundCounter = 0;
    int i = 0;
    int day = 0;
    int counter = 0;

    while((AMOUNT_COMPOUND > i) && counter <= DifferentProgramDays )
    {
        if(timePerDay[day%DifferentProgramDays] >= 17) {
            program_day[day%DifferentProgramDays].compound[i] = 1;
            i++;
            timePerDay[day%DifferentProgramDays] -= 17;
            CompoundCounter++;
        }
        else {
            counter++;
        }
        day++;

    }

    i = 0;
    counter = 0;
    while(AMOUNT_SECONDARY > i && counter <= DifferentProgramDays && CompoundCounter < maxAmountOfCompound)
    {
        if(timePerDay[day%DifferentProgramDays] >= 14) {
            program_day[day%DifferentProgramDays].secondary[i] = 1;
            i++;
            timePerDay[day%DifferentProgramDays] -= 14;
            CompoundCounter++;
        }
        else {
            counter++;
        }
        day++;
    }
    counter = 0;
    i = 0;

    while((AMOUNT_TERTIARY > i) && counter <= DifferentProgramDays)
    {
        if(timePerDay[day % DifferentProgramDays] >= 10) {
            program_day[day%DifferentProgramDays].tertiary[i] = 1;
            i++;
            timePerDay[day%DifferentProgramDays] -= 10;
        }
        else {
            counter++;
        }
        day++;
    }
    if ( amountOfDays > DifferentProgramDays ) {
        switch (amountOfDays) {
            case 4: program_day[2] = program_day[0];
                    program_day[3] = program_day[1];
                    break;
            case 5: program_day[3] = program_day[0];
                    program_day[4] = program_day[1];
                    break;
            case 6: program_day[3] = program_day[0];
                    program_day[4] = program_day[1];
                    program_day[5] = program_day[2];
                    break;
            case 7: program_day[3] = program_day[0];
                    program_day[4] = program_day[1];
                    program_day[5] = program_day[2];
                    program_day[6] = program_day[3];
                    break;
        }
    }
}