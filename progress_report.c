#include "progress_report.h"
#include "P1.h"
#include "UserData.h"

void print_progress_report() {
    user_file_header_prefs* user_prefs = read_user_preferences(local_userFILE);
    int amount_of_workouts = user_prefs->workout_counter-1;
    user_file_exercise_data* exercise_data = (user_file_exercise_data*)malloc(sizeof(user_file_exercise_data)*AMOUNT_EXERCISES);
    read_previous_user_workout_data(exercise_data, amount_of_workouts);

    printf("Weight exercise 1: %lf\n", exercise_data[0].weight);
    printf("Reps exercise 1: %d\n", exercise_data[0].reps);
    //printf("Weight: %lf\n", current_workout.weight);
    //printf("Reps: %d\n", current_workout.reps);
}