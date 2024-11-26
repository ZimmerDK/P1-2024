#include "workout_program.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void set_program_day(workout_days_t* program_day, int amountOfDays, int time );

/** @brief Function that generates the workout program
 *  @param userPrefs @in The user preferences
 *  @return A pointer to the workout program
 */
workout_days_t* generate_workout_program(user_file_header_prefs userPrefs) {

    int amountOfDays = userPrefs.prefered_days;
	int time = userPrefs.perfered_time;
	workout_days_t* program_day = allocate_workout_program(amountOfDays);

	set_program_day(program_day, amountOfDays, time );

	return program_day;
}

/** @brief Function that allocates memory for the workout program
 *  @param amountOfDays @in The amount of days the program should be
 *  @return A pointer to the workout program
 */
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


void _print_add_spacing(char c, int amountDays) {
	for (int i = 0; i <= 30*amountDays; i++) {
        printf("%c", c);
	}
	printf("\n");
}

void _print_add_spacing_with_ends(char spacing, char ends, int amountDays) {
	printf("%c", ends);
	for (int i = 0; i < 30*amountDays-1; i++) {
		printf("%c", spacing);
	}
	printf("%c\n", ends);
}

void _print_add_seperation(char seperator, char inbetween, int amountDays) {

    for (int i = 0; i < amountDays; i++) {
		printf("%c", seperator);
		for (int j = 0; j < 29; j++) {
			printf("%c", inbetween);
		}
	}
	printf("%c\n", seperator);

}

void print_workout_program(workout_days_t* program, int amountofdays) {

    // Print some kind of table
    int* e_counter = (int*)malloc(sizeof(int) * amountofdays);
    if(e_counter == NULL) {
        printf("Not allocated memory in e_counter!");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < amountofdays; i++) {
		e_counter[i] = 0;
	}
    /*
    ##############################################
    #    DAY 1     #    DAY 2     #    DAY 3     #
    ##############################################
    # Compound     # Compound     # Compound     #
    # ...		  # ...          # ...          #
    # Secondary    # Secondary    # Secondary    #
    # ...          # ...          # ...          #
    # Tertiary     # Tertiary     # Tertiary     #
    # ...          # ...          # ...          #
    ##############################################
    */

    _print_add_spacing('#', amountofdays);

    _print_add_seperation('#', ' ', amountofdays);


    for (int i = 0; i < amountofdays; i++) {
		printf("#            DAY %d            ", i+1);
	}
    printf("#\n");


    _print_add_seperation('#', ' ', amountofdays);

    _print_add_spacing('#', amountofdays);

    _print_add_seperation('#', '(', amountofdays);

    printf("#");
    for (int i = 0; i < (amountofdays/2 * 30)-5; i++) {
        printf(" ");
    }
    printf("COMPOUND");
    for (int i = 0; i < (amountofdays/2 * 30)-5; i++) {
		printf(" ");
	}
    printf(" #\n");

    _print_add_seperation('#', ')', amountofdays);

    _print_add_spacing('#', amountofdays);

    for(int i = 0; i < AMOUNT_COMPOUND; i++) {
        for (int j = 0; j < amountofdays; j++) {
            // get the next exercise using e_counter
            while(e_counter[j] < AMOUNT_COMPOUND && program[j].compound[e_counter[j]] == 0) {
				e_counter[j]++;
			}
            if (e_counter[j] < AMOUNT_COMPOUND) {

				//printf("# %s", exercises_c[exercise_compound_c[e_counter[j]]].name);

                // Pad to 30 characters
                printf("# %s", exercises_c[exercise_compound_c[e_counter[j]]].name);
				for (int k = 0; k < 28 - strlen(exercises_c[exercise_compound_c[e_counter[j]]].name); k++) {
                    printf(" ");
				}
                e_counter[j]++;
			}
			else {
                // Pad to 30 characters
                printf("#");
				for (int k = 0; k <=28; k++) {
					printf(" ");
				}
			}
        }
        printf("#\n");
	}

    _print_add_spacing('#', amountofdays);

    _print_add_seperation('#', '/', amountofdays);

    printf("#");
    for (int i = 0; i < (amountofdays / 2 * 30) - 6; i++) {
        printf(" ");
    }
    printf("SECONDARY");
    for (int i = 0; i < (amountofdays / 2 * 30) - 5; i++) {
        printf(" ");
    }
    printf(" #\n");

    _print_add_seperation('#', '\\', amountofdays);

    _print_add_spacing('#', amountofdays);

    for (int i = 0; i < amountofdays; i++) {
        e_counter[i] = 0;
    }

    for (int i = 0; i < AMOUNT_SECONDARY; i++) {
        for (int j = 0; j < amountofdays; j++) {
            // get the next exercise using e_counter
            while (e_counter[j] < AMOUNT_SECONDARY && program[j].secondary[e_counter[j]] == 0) {
                e_counter[j]++;
            }
            if (e_counter[j] < AMOUNT_SECONDARY) {

                //printf("# %s", exercises_c[exercise_compound_c[e_counter[j]]].name);

                // Pad to 30 characters
                printf("# %s", exercises_c[exercise_secondary_c[e_counter[j]]].name);
                for (int k = 0; k < 28 - strlen(exercises_c[exercise_secondary_c[e_counter[j]]].name); k++) {
                    printf(" ");
                }
                e_counter[j]++;
            }
            else {
                // Pad to 30 characters
                printf("#");
                for (int k = 0; k <= 28; k++) {
                    printf(" ");
                }
            }
        }
        printf("#\n");
    }

    _print_add_spacing('#', amountofdays);

    _print_add_seperation('#', '<', amountofdays);

    printf("#");
    for (int i = 0; i < (amountofdays / 2 * 30) - 5; i++) {
        printf(" ");
    }
    printf("TERTIARY");
    for (int i = 0; i < (amountofdays / 2 * 30) - 5; i++) {
        printf(" ");
    }
    printf(" #\n");

    _print_add_seperation('#', '>', amountofdays);

    _print_add_spacing('#', amountofdays);

    for (int i = 0; i < amountofdays; i++) {
        e_counter[i] = 0;
    }

    for (int i = 0; i < AMOUNT_TERTIARY; i++) {
        for (int j = 0; j < amountofdays; j++) {
            // get the next exercise using e_counter
            while (e_counter[j] < AMOUNT_TERTIARY && program[j].tertiary[e_counter[j]] == 0) {
                e_counter[j]++;
            }
            if (e_counter[j] < AMOUNT_TERTIARY) {

                //printf("# %s", exercises_c[exercise_compound_c[e_counter[j]]].name);

                // Pad to 30 characters
                printf("# %s", exercises_c[exercise_tertiary_c[e_counter[j]]].name);
                for (int k = 0; k < 28 - strlen(exercises_c[exercise_tertiary_c[e_counter[j]]].name); k++) {
                    printf(" ");
                }
                e_counter[j]++;
            }
            else {
                // Pad to 30 characters
                printf("#");
                for (int k = 0; k <= 28; k++) {
                    printf(" ");
                }
            }
        }
        printf("#\n");
    }

    _print_add_spacing('#', amountofdays);
}


/** @brief Function that prints the workout program
 *  @param exercises @in An array of exercises
 *  @param program_day @in The workout program
 *  @param number_of_days @in The amount of days in the program
 */
void print_function(exercise_t* exercises, workout_days_t* program_day, int number_of_days) {

    for(int i = 0; i < number_of_days; i++) {
        printf("\n");
        printf("Day: %d\n", i+1);

        //Prints the compound for the day
        for(int j = 0; j < AMOUNT_COMPOUND; j++) {
            if ( program_day[i].compound[j] == 1 ) {
                printf("compound %d: %s\n", j+1, exercises[exercise_compound_c[j]].name);
            }
        }

        //Prints the secondary for the day
        for(int j = 0; j < AMOUNT_SECONDARY; j++) {
            if ( program_day[i].secondary[j] == 1 ) {
                printf("secondary %d: %s\n", j+1, exercises[exercise_secondary_c[j]].name);
            }
        }

        //Prints the tertiary for the day
        for(int j = 0; j < AMOUNT_TERTIARY; j++) {
            if ( program_day[i].tertiary[j] == 1 ) {
                printf("tertiary %d: %s\n", j+1, exercises[exercise_tertiary_c[j]].name);
            }
        }
    }
}


/** @brief Function that sets the workout program
 *  @param program_day @out The workout program
 *  @param amountOfDays @in The amount of days in the program
 *  @param time @in The time per day
 */
void set_program_day(workout_days_t* program_day, int amountOfDays, int time) {

    // Initialize all values to 0
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

    // Determine how many different days the program should have
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


    int* timePerDay = (int*)malloc(sizeof(int) * amountOfDays);

    if(timePerDay == NULL) {
		printf("Not allocated memory in timePerDay!");
		exit(EXIT_FAILURE);
	}

    // Set the time per day
    for(int i = 0; i < DifferentProgramDays; i++) {
        timePerDay[i] = time;
    }

    const int maxAmountOfCompound = 3*DifferentProgramDays;
    int CompoundCounter = 0;
    int i = 0;
    int day = 0;
    int counter = 0;

    // Sets the compound exercises, if there is time (17 minutes)
    while((AMOUNT_COMPOUND > i) && (counter <= DifferentProgramDays) && (CompoundCounter < maxAmountOfCompound))
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

    i = 0; counter = 0;

    // Sets the secondary exercises, if there is time (14 minutes)
    while(AMOUNT_SECONDARY > i && counter <= DifferentProgramDays)
    {
        if(timePerDay[day%DifferentProgramDays] >= 14) {
            program_day[day%DifferentProgramDays].secondary[i] = 1;
            i++;
            timePerDay[day%DifferentProgramDays] -= 14;
        }
        else {
            counter++;
        }
        day++;
    }
    counter = 0; i = 0;

    // Sets the tertiary exercises, if there is time (10 minutes)
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

    // Free the allocated memory
    free(timePerDay);

	// If there are more days in the program than the different days, the program will be repeated
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
/* Bør man ikke tage det hvis man ikke kan kører hele programmet igennem? compound / secondary / tertiary */