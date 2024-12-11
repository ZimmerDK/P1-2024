#pragma once

#include "workout_program.h"
#include "UserData.h"
#include "UserSpace.h"

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>

void calibrate_workout_routine(exercise_t exercises[]);
void run_day(workout_days_t* workout_day);