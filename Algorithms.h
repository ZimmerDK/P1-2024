#pragma once

#include "workout_program.h"
#include "UserData.h"
#include "UserSpace.h"

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>

typedef struct set_data_t {
	//const int estWorkoutIntensity;

	int intensity;

	//float score;
} set_data_t;

typedef struct workout_result_t {
	int repChange;
	double weightChange;
} workout_result_t;

workout_result_t calculate_workout(set_data_t* data, user_exercise_data_t* exercise_data, int setCount);
void calibrate_workout_routine(exercise_t exercises[]);
void run_day(workout_days_t* workout_day);