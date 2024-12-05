#ifndef PROGRESS_REPORT_H
#define PROGRESS_REPORT_H
#include "P1.h"
#include "UserData.h"
#include "workout_program.h"
#include "exercises.h"

void inputs_progress(int* input_workout_progress, int amount_of_workouts, int* input_print_day, int amount_of_different_workouts);

void user_view_report(const user_context_t* user_context, workout_days_t* workout_day);

void print_progress_workout_day(const user_context_t* user_context, workout_days_t* workout_day, int input_print_day, int amount_of_workouts, int input_workout_progress);

//void sort_array_progress(user_exercise_data_t improved_exercise[]);
#endif //PROGRESS_REPORT_H