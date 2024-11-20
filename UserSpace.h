/* OPTIONS
* Start workout -> Select workout
* Recalibrate
* User Prefrences
* View report
*/
#pragma once

#include "workout_program.h"
#include "UserData.h"
#include "exercises.h"
#include "P1.h"
#include <stdio.h>
#include <stdlib.h>

extern const void* modes[4];


enum Userspace_options {
	START_WORKOUT = 1,
	RECALIBRATE = 2,
	CHANGE_PREFERENCES = 3,
	VIEW_REPORT = 4
};

void user_space_main(UserPreferences_t* user_prefs, workout_days_t* workout_plan);

void user_start_workout(UserPreferences_t* user_prefs, workout_days_t* workout_plan);

void user_recalibrate();

void user_change_preferences();

void user_view_report();


