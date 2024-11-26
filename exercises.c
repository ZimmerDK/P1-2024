#include "exercises.h"
#include <stdio.h>
#include "workout_program.h"

/* Assigning different structs for Compound, secondary and tertiary exercises.with all the different compound exercises.
The exercises are formed with name, weight step, minimum reps, maximum reps, type, intensity and a struct of user
exercise data */

exercise_t exercises_c[AMOUNT_EXERCISES] = {
	{"Bench Press", 2.5, 6, 10, COMPOUND, 9.0, CHEST, NULL},
	{"Squat", 2.5, 8, 10, COMPOUND, 9.0, UPP_THIGH, NULL},
	{"Deadlift", 2.5, 8, 12, COMPOUND, 8.5, BACK, NULL},
	{"Dumbbell Military Press", 2, 6, 10, COMPOUND, 8.0, SHOULDER, NULL},
	{"Barbell Rows", 2.5, 8, 12, COMPOUND, 8.5, BACK, NULL},
	{"Romanian Deadlifts", 2.5, 6, 12, SECONDARY, 9.0, LOW_THIGH, NULL},
	{"Lat Pull-Down", 2.5, 6, 12, SECONDARY, 9.0, BACK, NULL},
	{"Incline Dumbbell-Press", 2, 6, 12, SECONDARY, 9.0, CHEST, NULL},
	{"Hip-Thrusts", 2.5, 6, 12, SECONDARY, 9.0, LOW_THIGH, NULL},
	{"Triceps Pushdown", 2.5, 6, 12, ISOLATION, 9.0, TRICEPS, NULL},
	{"Seated Dumbbell Curls", 2.5, 6, 12, ISOLATION, 9.0, BICEPS, NULL},
	{"Lateral Raises", 2.5, 6, 12, ISOLATION, 9.0, SHOULDER, NULL},
	{"Leg Extensions", 2.5, 10, 14, ISOLATION, 9.0, UPP_THIGH, NULL},
	{"Skull Crushers", 2.5, 6, 12, ISOLATION, 9.0, TRICEPS, NULL},
	{"Flies", 2.5, 6, 12, ISOLATION, 9.0, CHEST, NULL},
	{"Seated Hammer Curls", 2.5, 6, 12, ISOLATION, 9.0, BICEPS, NULL},
	{"Reverse Flies", 2.5, 6, 12, ISOLATION, 9.0, SHOULDER, NULL},
	{"Face Pulls", 2.5, 6, 12, ISOLATION, 9.0, SHOULDER, NULL}
};

//mapping the different compound exercises
const int exercise_compound_c[AMOUNT_COMPOUND] = {
	0, 1, 2, 3, 4
};

//mapping the different compound secondary
const int exercise_secondary_c[AMOUNT_SECONDARY] = {
	5, 6, 7, 8
};

//mapping the different compound tertiary
const int exercise_tertiary_c[AMOUNT_TERTIARY] = {
	9, 10, 11, 12, 13, 14, 15, 16, 17
};