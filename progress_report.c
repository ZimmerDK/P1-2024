#include "progress_report.h"


void user_view_report(const user_context_t* user_context, workout_days_t* workout_day) {

    int amount_of_workouts = user_context->userPrefs->workout_counter-2;
    int amount_of_different_workouts = user_context->userPrefs->prefered_days;
    int input_workout_progress;
    int input_print_day;

    inputs_progress(&input_workout_progress, amount_of_workouts, &input_print_day, amount_of_different_workouts);
    input_print_day--;
    input_workout_progress--;


    print_progress_workout_day(user_context, workout_day, input_print_day, amount_of_workouts, input_workout_progress);
}


void inputs_progress(int* input_workout_progress, int amount_of_workouts, int* input_print_day, int amount_of_different_workouts) {
    do {
        printf("Enter a workout to see progress from 1 - %d. \n", amount_of_different_workouts);
        scanf(" %d", input_print_day);

        if (*input_print_day > amount_of_different_workouts || *input_print_day <= 0) {
            printf("Your total number of different workouts is: %d.\n"
                   "Therefore enter an amount from 1 - %d:\n",
                   amount_of_different_workouts, amount_of_different_workouts);
        }
    } while (*input_print_day > amount_of_different_workouts || *input_print_day <= 0);

    do {
        printf("You have trained a total of %d times! \n", amount_of_workouts+1);
        printf("Enter amount of workouts you want progress for: \n");
        scanf(" %d", input_workout_progress);

        if (*input_workout_progress > amount_of_workouts + 1 || *input_workout_progress <= 0) {
            printf("Your total number of workouts is: %d.\n"
                   "Therefore enter amount of workouts you want progress for, less than %d:\n", (amount_of_workouts+1),
                   (amount_of_workouts+1));
        }
    } while (*input_workout_progress > amount_of_workouts + 1 || *input_workout_progress <= 0);
}


int sort_array_progress_weight(const user_file_exercise_data* a, const user_file_exercise_data* b) {
    return(int)-(a->weight - b->weight);
}


void print_progress_workout_day(const user_context_t* user_context, workout_days_t* workout_day, int input_print_day, int amount_of_workouts, int input_workout_progress) {

	user_exercise_data_t* improved_exercise = (user_exercise_data_t*)malloc(sizeof(user_exercise_data_t) * AMOUNT_EXERCISES);

    user_file_exercise_data* previous_exercise_data = (user_file_exercise_data*)malloc(sizeof(user_file_exercise_data) * AMOUNT_EXERCISES);

    read_previous_user_workout_data(user_context, previous_exercise_data, (amount_of_workouts - input_workout_progress));

    // Look through all the exercises and compare the weight reps

    for (int i = 0; i < AMOUNT_EXERCISES; i++) {
		improved_exercise[i].weight = exercises_c[i].user_exercise_data->weight - previous_exercise_data[i].weight;
		improved_exercise[i].reps = exercises_c[i].user_exercise_data->reps - previous_exercise_data[i].reps;
    }


    // Sort by weight
    qsort(improved_exercise, AMOUNT_EXERCISES, sizeof(user_exercise_data_t), sort_array_progress_weight);

	// DEBUG: Print the current data, prev data and sorted array
    
    /*for (int i = 0; i < AMOUNT_EXERCISES; i++) {
		printf("Exercise: %s %lf kg\n", exercises_c[i].name, improved_exercise[i].weight);
    }*/

	// Print the exercises
	// only if the weight or reps have changed then print
	
    for (int i = 0; i < AMOUNT_EXERCISES; i++) {
		if ((improved_exercise[i].weight > 0 + 0.00001f || improved_exercise[i].weight < 0 - 0.00001f)) {
			
			// print: Exercise: {exercise_name} +/- {weight}kg

            // setup string
			
			char sign = '+';
			if (improved_exercise[i].weight < 0) {
				sign = '-';
			}

            printf("Exercise: %s %c %.2lf kg\n", exercises_c[i].name, sign, fabs(improved_exercise[i].weight));
		}
	}
    


    /*
    int counter = 0;
    int exc_counter = 0;
    for (int i = 0; i < AMOUNT_COMPOUND; i++) {
        if (workout_day[input_print_day].compound[i] == 1) {
            if (current_exercise_data[counter].weight != previous_exercise_data[counter].weight){
                improved_exercise[exc_counter].weight =
                    current_exercise_data[counter].weight - previous_exercise_data[counter].weight;

                improved_exercise[exc_counter].index_number = counter;
                exc_counter++;
            }
            else {
                improved_exercise[exc_counter].reps =
                    current_exercise_data[counter].reps - previous_exercise_data[counter].reps;

                improved_exercise[exc_counter].index_number = counter;
                exc_counter++;
            }
        }
        counter++;
    }
    for (int i = 0; i < AMOUNT_SECONDARY; i++) {
        if (workout_day[input_print_day].secondary[i] == 1) {
            if (current_exercise_data[counter].weight != previous_exercise_data[counter].weight){
                improved_exercise[exc_counter].weight =
                    current_exercise_data[counter].weight - previous_exercise_data[counter].weight;

                improved_exercise[exc_counter].index_number = counter;
                exc_counter++;
            }
            else {
                improved_exercise[exc_counter].reps =
                    current_exercise_data[counter].reps - previous_exercise_data[counter].reps;

                improved_exercise[exc_counter].index_number = counter;
                exc_counter++;
            }
        }
        counter++;
    }
    for (int i = 0; i < AMOUNT_TERTIARY; i++) {
        if (workout_day[input_print_day].tertiary[i] == 1) {
            if (current_exercise_data[counter].weight != previous_exercise_data[counter].weight){
                improved_exercise[exc_counter].weight =
                    current_exercise_data[counter].weight - previous_exercise_data[counter].weight;

                improved_exercise[exc_counter].index_number = counter;
                exc_counter++;
            }
            else {
                improved_exercise[exc_counter].reps =
                    current_exercise_data[counter].reps - previous_exercise_data[counter].reps;

                improved_exercise[exc_counter].index_number = counter;
                exc_counter++;
            }
        }
        counter++;
    }
    */
}
