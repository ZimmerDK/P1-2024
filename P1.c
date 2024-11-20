// P1.cpp : Defines the entry point for the application.
//

#include "exercises.h"
#include "P1.h"
#include "workout_program.h"
//#include "UserData.h"

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>


void run_day(workout_days_t* workout_day);

typedef struct set_data_t {
	//const int estWorkoutIntensity;
	
	int intensity;

	//float score;
} set_data_t;

typedef struct workout_result_t {
	int repChange;
	double weightChange;
} workout_result_t;

int main(void) {

	UserPreferences_t* userPrefs = malloc(sizeof(UserPreferences_t));

	if (userPrefs == NULL) {
		printf("Memory allocation failed");
		return -1;
	};

	UserData_main(userPrefs);

	//workout_days_t* workout = (workout_days_t*)generate_workout_program(*userPrefs);


	FILE* userFILE = NULL;
	char filepath[MAX_LENGTH + 15];
	snprintf(filepath, sizeof(filepath), "%s/%s.dat", USER_FILES_DIR, username);
	userFILE = fopen(filepath, "rb+");
	ProfilePage(userFILE, userPrefs);

	//print_workout_program(workout, userPrefs->days);

	/*for (int i = 0; i < userPrefs->days; i++) {
		run_day(&workout[i]);
	};*/

	//printf("Rep Change : %d\n", result.repChange);
	//printf("Weight Change : %lf\n", result.weightChange);

	/*workout_data_t calibrationData = {8, 20.0};

	calibrate_workout_routine(&calibrationData);

	printf("Calibration Result: %lf kg", calibrationData.weight);
	*/
	//printf("Hello World!");

	return 0;
}

int ProfilePage(FILE* userFILE, UserPreferences_t* userPrefs) {
	int input;
	printf("Welcome!\n");
	UserOptions:
	printf("Please input an option below:\n");
	printf("1 = Print workout plan   |   2 = Edit user preferences   |   "
		   "3 = User Profile and Stats\n>");
	scanf(" %d", &input);

	switch (input) {

		case 1:
			*userPrefs = read_user_preferences(userFILE);
			workout_days_t* workout = (workout_days_t*)generate_workout_program(*userPrefs);
			print_workout_program(workout, userPrefs->days);
			char c_input;
			want_to_start_workout:
			printf("Do you want to start the workout? (y/n)\n");
			scanf(" %c", &c_input);
			if (c_input == 'n') {
				goto UserOptions;
			} else if (c_input == 'y') {
				// Run day workout
				for (int i = 0; i < userPrefs->days; i++) {
					run_day(&workout[i]);
				};
			} else {
				printf("ERROR: Please enter valid input ('y' or 'n')\n");
				goto want_to_start_workout;
			}
			break;

		case 2:
			user_setup(userFILE);
			goto UserOptions;
			break;

		case 3:
			UserProfileView:
			printf("USER PROFILE  |  Logged in as: %s\n", username);
			printf("Please input an option below:\n");
			printf("1 = View Progress Report   |   2 = Workout Streak\n>");
			scanf(" %d", &input);
			switch (input) {
				case 1:
					//Print Progress Report [TO DO]
				case 2:
					//Print Workout Streak [TO DO]
				default:
					if (input < 1 || input > 2) {
						printf("ERROR: Please input a valid option!\n");
						goto UserProfileView;
					}
					break;
			}

		default:
			if (input < 1 || input > 3) {
			printf("ERROR: Please input a valid option!\n");
			goto UserOptions;
			}
			break;
	}


	free(userPrefs);

	return 0;
}

/** @brief Function that calculates the workout
 *  @param data @in The data from the sets
 *  @param exercise_data @in The data from the exercise
 *  @param setCount @in The amount of sets
 *  @return The workout result
 */
workout_result_t calculate_workout(set_data_t* data, exercise_data_t* exercise_data, int setCount) {

	int maxScore = 0;
	set_data_t* max_result;

	const int minRep = exercise_data->exercise->min_reps;
	const int maxRep = exercise_data->exercise->max_reps;
	const double estIntensity = exercise_data->exercise->est_intensity;
	const int repDiff = maxRep - minRep;
	const double weight_step = exercise_data->exercise->weight_step;

	workout_result_t workoutResult = {0, 0};

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

void calibrate_workout_routine(exercise_data_t* calibration_data) {

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
	printf("Starting exercise: %s\n", exercise->name);
	printf("Estimated Intensity: %lf\n", exercise->est_intensity);
	printf("Current Weight: %lf\n", exercise->user_exercise_data->weight);
	printf("Current Reps: %d\n", exercise->user_exercise_data->reps);
	printf("#############################################\n\n");
	
	set_data_t* setData = malloc(sizeof(set_data_t) * 3);

	scanf("%d %d %d", &setData[0].intensity, &setData[1].intensity, &setData[2].intensity);

	workout_result_t workout_result = calculate_workout(setData, exercise->user_exercise_data, 3);

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
			run_exercise(&exercises_c[exercise_compound_c[i]]);
		}
	}

	printf("Starting secondary exercises:\n");
	for (int i = 0; i < AMOUNT_SECONDARY; i++) {
		if (workout_day->secondary[i] == 1) {
			run_exercise(&exercises_c[exercise_secondary_c[i]]);
		}
	}

	printf("Starting tertiary exercises:\n");
	for (int i = 0; i < AMOUNT_TERTIARY; i++) {
		if (workout_day->tertiary[i] == 1) {
			run_exercise(&exercises_c[exercise_tertiary_c[i]]);
		}
	}
}

struct user_meta_data {
	char username[16];
};
