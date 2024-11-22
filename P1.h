// P1.h : Include file for standard system include files,
// or project specific include files.
#pragma once


#ifndef P1_H
#define P1_h

#include "UserData.h"
#include "workout_program.h"
#include "UserData.h"
#include "UserSpace.h"

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>

void calibrate_workout_routine(user_exercise_data_t* calibration_data, exercise_t exercises[]);
void run_day(workout_days_t* workout_day);

#endif // P1_H


// User Profile stuff:
int ProfilePage(FILE* userFILE, UserPreferences_t* userPrefs);

// TODO: Reference additional headers your program requires here.
