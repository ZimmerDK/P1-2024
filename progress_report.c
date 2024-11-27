#include "progress_report.h"
void user_view_report(workout_days_t* workout_day) {

    user_file_header_prefs* user_prefs = read_user_preferences(local_userFILE);
    int amount_of_workouts = user_prefs->workout_counter-2;
    int amount_of_different_workouts = user_prefs->prefered_days;
    int input_workout_progress;
    int input_print_day;

    inputs_progress(&input_workout_progress, amount_of_workouts, &input_print_day, amount_of_different_workouts);
    input_print_day--;


    print_progress_workout_day(workout_day, input_print_day, amount_of_workouts, input_workout_progress);
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

int sort_array_progress_weight(const void* a, const void* b) {
    return(int)(-((user_file_exercise_data *)a)->weight - ((user_file_exercise_data*)b)->weight);
}

void print_progress_workout_day(workout_days_t* workout_day, int input_print_day, int amount_of_workouts,
    int input_workout_progress) {

    int amountOfExercises = 0;
    for (int i = 0; i < AMOUNT_COMPOUND; i++) {
        if (workout_day[input_print_day].compound[i] == 1) {
            amountOfExercises++;
        }
    }
    for (int i = 0; i < AMOUNT_SECONDARY; i++) {
        if (workout_day[input_print_day].compound[i] == 1) {
            amountOfExercises++;
        }
    }
    for (int i = 0; i < AMOUNT_TERTIARY; i++) {
        if (workout_day[input_print_day].compound[i] == 1) {
            amountOfExercises++;
        }
    }

    user_exercise_data_t improved_exercise[amountOfExercises];

    user_file_exercise_data* current_exercise_data =
     (user_file_exercise_data*)malloc(sizeof(user_file_exercise_data)*AMOUNT_EXERCISES);

    user_file_exercise_data* previous_exercise_data =
        (user_file_exercise_data*)malloc(sizeof(user_file_exercise_data)*AMOUNT_EXERCISES);

    read_previous_user_workout_data(previous_exercise_data, amount_of_workouts - input_workout_progress);
    read_previous_user_workout_data(current_exercise_data, (amount_of_workouts));

    for (int i = 0; i < AMOUNT_EXERCISES; i++) {
        printf("Weight: %lf\n", current_exercise_data[i].weight);
    }

    printf("---------------------\n");
    for (int i = 0; i < AMOUNT_EXERCISES; i++) {
        printf("Weight: %lf\n", previous_exercise_data[i].weight);
    }
    printf("---------------------\n");

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
    qsort(improved_exercise, amountOfExercises, sizeof(user_exercise_data_t), sort_array_progress_weight);

    for (int i = 0; i < amountOfExercises; i++) {
        printf("Weight : %lf \n", improved_exercise[i].weight);
    }
}
