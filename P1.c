// P1.cpp : Defines the entry point for the application.
//

#include "exercises.h"
#include "P1.h"
#include "workout_program.h"
#include "UserData.h"

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>


const char* USER_DATA_HEADER_PATH = "./UserData.dat";
const char* USER_DATA_HEADER_DEBUG_PATH = "C:\\Users\\kikoi\\Desktop\\P1\\P1\\UserData.dat";
const char* USER_DATA_DIR_PATH = "C:\\Users\\kikoi\\Desktop\\P1\\P1\\out\\Users\\";


typedef struct set_data_t {
	//const int estWorkoutIntensity;
	
	int intensity;

	//float score;
};

typedef struct workout_result_t {
	int repChange;
	double weightChange;
};



/** @brief Function that calculates the workout
 *  @param data @in The data from the sets
 *  @param exercise_data @in The data from the exercise
 *  @param setCount @in The amount of sets
 *  @return The workout result
 */
struct workout_result_t calculate_workout(struct set_data_t* data, exercise_data_t* exercise_data, int setCount) {

	int maxScore = 0;
	struct set_data_t* max_result;

	const int minRep = exercise_data->exercise->min_reps;
	const int maxRep = exercise_data->exercise->max_reps;
	const double estIntensity = exercise_data->exercise->est_intensity;
	const int repDiff = maxRep - minRep;
	const double weight_step = exercise_data->exercise->weight_step;

	struct workout_result_t workoutResult = {0, 0};

	// Find the set with the highest score
	for (int i = 0; i < setCount; i++) {
		if (data[i].intensity > maxScore) {
			max_result = &data[i];
			maxScore = data[i].intensity;
		}
	}

	// Calculate the collected score
	double collectedScore = 0;
	double maxPossibleScore = 0;
	if (max_result->intensity >= estIntensity) {
		for (int i = 0; i < setCount; i++) {
			double sqrtRes = (sqrt((i + 1) / (float)setCount));
			collectedScore += data[i].intensity * (-(sqrtRes / 1.4) + 1.5);
			maxPossibleScore += (-(sqrtRes / 1.4) + 1.5);
		}
		collectedScore /= maxPossibleScore;
	}

	printf("Collected Score : %lf \n", collectedScore);

	// Check if the collected score is higher or lower than the estimated intensity
	if (collectedScore > (estIntensity+0.5) + 0.0001) {
		if (exercise_data->reps == minRep) {
			workoutResult.weightChange = -weight_step;
			workoutResult.repChange = repDiff;
		}
		else if (exercise_data->reps > minRep) {
			workoutResult.repChange = -1;
		}
	}
	else {
		if (maxScore != 10 && collectedScore < (estIntensity - 0.5)) {
			if (maxScore <= estIntensity / 2) {
				workoutResult.weightChange = weight_step;
				workoutResult.repChange = -(exercise_data->reps - minRep);
			} else {
				if (exercise_data->reps == maxRep) {
					workoutResult.weightChange = weight_step;
					workoutResult.repChange = -repDiff;
				}
				else if (exercise_data->reps < maxRep) {
					workoutResult.repChange = 1;
				}
			}

		}
	}

	return workoutResult;
}

/** @brief Function that calibrates the workout routine
 *  @param calibration_data @in @out The calibration data
 */

void calibrate_workout_routine(struct exercise_data_t* calibration_data) {

	printf("6 Reps @ %lf kg\n", calibration_data->weight);
	printf("Enter Intensity: ");
	
	int intensity = 0;
	scanf("%d", &intensity);

	if (intensity >= 9) {
		return;
	} 
	else if (intensity >= 7)
	{
		calibration_data->weight += 2.5;
		calibrate_workout_routine(calibration_data);
	}
	else {
		calibration_data->weight += 5.0;
		calibrate_workout_routine(calibration_data);
	}
}

/** @brief Function that runs the exercise
 *  @param exercise @in The exercise
 */
void run_exercise(exercise_t* exercise) {

	printf("#############################################\n");
	printf("Starting exercise: %s\n", exercise.name);
	printf("Estimated Intensity: %lf\n", exercise.est_intensity);
	printf("Current Weight: %lf\n", /*exercise.user_exercise_data->weight*/ 40.0);
	printf("Current Reps: %d\n", /*exercise.user_exercise_data->reps*/ 10);
	printf("#############################################\n\n");
	
	struct set_data_t* setData = malloc(sizeof(struct set_data_t) * 3);
	exercise_data_t workoutData = { 10, 40.0, exercise };
	scanf("%d %d %d", &setData[0].intensity, &setData[1].intensity, &setData[2].intensity);

	struct workout_result_t workout_result = calculate_workout(setData, &workoutData, 3);

	printf("\nRep Change : %d\n", workout_result.repChange);
	printf("Weight Change : %lf\n", workout_result.weightChange);

	free(setData);
}


/** @brief Function that runs the day
 *  @param workout_day @in The workout day
 */
void run_day(workout_days_t* workout_day) {
	printf("Starting compound exercises:\n");
	for (int i = 0; i < AMOUNT_COMPOUND; i++) {
		if (workout_day->compound[i] == 1) {
			run_exercise(exercises_c[exercise_compound_c[i]]);
		}
	}

	printf("Starting secondary exercises:\n");
	for (int i = 0; i < AMOUNT_SECONDARY; i++) {
		if (workout_day->secondary[i] == 1) {
			run_exercise(exercises_c[exercise_secondary_c[i]]);
		}
	}

	printf("Starting tertiary exercises:\n");
	for (int i = 0; i < AMOUNT_TERTIARY; i++) {
		if (workout_day->tertiary[i] == 1) {
			run_exercise(exercises_c[exercise_tertiary_c[i]]);
		}
	}
}


int main(void) {


	workout_days_t testDay = { {1, 1, 0, 1, 0}, {1, 0, 1}, {0, 0, 1} };

	__main();




	//run_day(&testDay);

	//printf("Rep Change : %d\n", result.repChange);
	//printf("Weight Change : %lf\n", result.weightChange);

	/*struct workout_data_t calibrationData = {8, 20.0};

	calibrate_workout_routine(&calibrationData);

	printf("Calibration Result: %lf kg", calibrationData.weight);
	*/
	//printf("Hello World!");

	return 0;
}

/* FILE LAYOUT

HEADER:
int - user count
int - header end

*/

struct user_meta_data {
	char username[16];
};
/*
void create_new_user(FILE* file, char username[16]) {
	int user_count = 0;

	fseek(file, 0, SEEK_SET);

	fread(&user_count, sizeof(int), 1, file);

	//for(int i = 0; i <)

	fseek(file, 0, SEEK_END);
	
	fwrite(username, 16, 1, file);


}*/
/*
int main(void) {
	FILE* file = fopen(USER_DATA_HEADER_DEBUG_PATH, "rb+");

	if (file == NULL) {
		printf("File did not open :(");
		return -1;
	}

	create_new_user(file, "Kristoffer\0");
	create_new_user(file, "Magnus\0");
	create_new_user(file, "Gustav\0");



	fclose(file);
	return 0;
}*/