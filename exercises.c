#include "exercises.h"
#include <stdio.h>
#include "workout_program.h"

/* Assigning different structs for Compound, secondary and tertiary exercises.with all the different compound exercises.
The exercises are formed with name, weight step, minimum reps, maximum reps, type, intensity and a struct of user
exercise data */

//assigning the different compound exercises
const exercise_t exercise_compound_c[AMOUNT_COMPOUND] = {
   {"Bench Press", 2.5, 6, 12, COMPOUND, 9.0, NULL},
   {"Squat", 2.5, 8, 10, COMPOUND, 9.0, NULL},
   {"Deadlift", 2.5, 8, 12, COMPOUND, 8.5, NULL},
   {"Military Press", 2.5, 6, 10, COMPOUND, 8.0, NULL},
   {"Barbell Rows", 2.5, 8, 12, COMPOUND, 8.5, NULL}
};

//assigning the different compound secondary
const exercise_t exercise_secondary_c[AMOUNT_SECONDARY] = {
   {"Romanian Deadlifts", 2.5, 6, 12, SECONDARY, 9.0, NULL},
   {"Lat Pull-Down", 2.5, 6, 12, SECONDARY, 9.0, NULL},
   {"Incline Dumbell-Press", 2.5, 6, 12, SECONDARY, 9.0, NULL},
   {"Hip-Thrusts", 2.5, 6, 12, SECONDARY, 9.0, NULL}
   };

//assigning the different compound tertiary
const exercise_t exercise_tertiary_c[AMOUNT_TERTIARY] = {
   {"Tricep Pushdown", 2.5, 6, 12, ISOLATION, 9.0, NULL},
   {"Seated Dumbell Curls", 2.5, 6, 12, ISOLATION, 9.0, NULL},
   {"Lateral Raises", 2.5, 6, 12, ISOLATION, 9.0, NULL},
   {"Leg Extensions", 2.5, 6, 12, ISOLATION, 9.0, NULL},
   {"Skull Crushers", 2.5, 6, 12, ISOLATION, 9.0, NULL},
   {"Flies", 2.5, 6, 12, ISOLATION, 9.0, NULL},
   {"Suspinated Bicep Curls", 2.5, 6, 12, ISOLATION, 9.0, NULL},
   {"Reverse Flies", 2.5, 6, 12, ISOLATION, 9.0, NULL},
   {"Face Pulls", 2.5, 6, 12, ISOLATION, 9.0, NULL},
};