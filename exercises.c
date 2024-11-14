#include "exercises.h"

const exercise_t exercise_compound_c[5] = {
   {"Bench Press", 2.5, 6, 12, COMPOUND, 9.0, NULL},
   {"Squat", 2.5, 8, 10, COMPOUND, 9.0, NULL},
   {"Deadlift", 2.5, 8, 12, COMPOUND, 8.5, NULL},
   {"Military Press", 2.5, 6, 10, COMPOUND, 8.0, NULL},
   {"Barbell Rows", 2.5, 8, 12, COMPOUND, 8.5, NULL}
};

const exercise_t exercise_secondary_c[5] = {
   {"Romanian Deadlifts", 2.5, 6, 12, SECONDARY, 9.0, NULL},
   {"Lat Pull-Down", 2.5, 6, 12, SECONDARY, 9.0, NULL},
   {"Seated Cablerow", 2.5, 6, 12, SECONDARY, 9.0, NULL}
   };

const exercise_t exercise_tertiary_c[5] = {
   {"Kickbacks", 2.5, 6, 12, ISOLATION, 9.0, NULL},
   {"Seated Dumbell Curls", 2.5, 6, 12, ISOLATION, 9.0, NULL},
   {"Lateral Raises", 2.5, 6, 12, ISOLATION, 9.0, NULL},
};