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

void user_space_main(const user_context_t* user_context);

void user_start_workout(const user_context_t* user_context, workout_days_t* workout_plan);

void user_recalibrate(const user_context_t* user_context);

void recalibrate_specific_exercise(const user_context_t* user_context, exercise_t exercises[], int index);

void manual_exercise_edit(const user_context_t* user_context, exercise_t exercises[], int chosen_index);

void user_view_statusreport();

void _clear_console();


