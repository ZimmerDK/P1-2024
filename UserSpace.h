/* OPTIONS
* Start workout -> Select workout
* Recalibrate
* User Preferences
* View report
*/
#pragma once

#include "workout_program.h"
#include "UserData.h"
#include "exercises.h"
#include "P1.h"
#include <stdio.h>
#include <stdlib.h>


enum UserSpace_options {
	PRINT_WORKOUT_PLAN = 1,
	RECALIBRATE = 2,
	CHANGE_PREFERENCES = 3,
	VIEW_REPORT = 4
};

enum Recalibrate_options {
	RECALIBRATE_ALL = 1,
	RECALIBRATE_SPECIFIC = 2,
	MANUALLY_EDIT_SPECIFIC = 3,
};

void user_space_main(user_file_header_prefs* userPrefs, workout_days_t* workout_plan, FILE* user_file);

void user_start_workout(user_file_header_prefs* userPrefs, workout_days_t* workout_plan);

void user_change_preferences();

void user_recalibrate();

void recalibrate_specific_exercise(exercise_t exercises[], int index);

void manual_exercise_edit(exercise_t exercises[], int chosen_index);

void user_view_report(workout_days_t* workout_days);


