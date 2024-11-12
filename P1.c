// P1.cpp : Defines the entry point for the application.
//

#include "exercises.h"
#include "P1.h"

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

struct workout_result_t calculate_workout(struct set_data_t* data, exercise_data_t* exercise_data, int setCount) {

	int maxScore = 0;
	struct set_data_t* max_result;

	const int minRep = exercise_data->exercise->min_reps;
	const int maxRep = exercise_data->exercise->max_reps;
	const double estIntensity = exercise_data->exercise->est_intensity;
	const int repDiff = maxRep - minRep;
	const double weight_step = exercise_data->exercise->weight_step;

	struct workout_result_t workoutResult = {0, 0};

	// Finder max score i set data
	for (int i = 0; i < setCount; i++) {
		if (data[i].intensity > maxScore) {
			max_result = &data[i];
			maxScore = data[i].intensity;
		}
	}

	// Hvis vi har en score på 9 eller højere i ét af setsne, kig på resten og vurdere en score
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


int main(void) {

	struct set_data_t* setData = malloc(sizeof(struct set_data_t) * 3);
	exercise_data_t workoutData = { 10, 40.0, &exercise_c[0]};

	setData[0].intensity = 1;
	setData[1].intensity = 2;
	setData[2].intensity = 3;

	struct workout_result_t result = calculate_workout(setData, &workoutData, 3);


	printf("Rep Change : %d\n", result.repChange);
	printf("Weight Change : %lf\n", result.weightChange);

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

void create_new_user(FILE* file, char username[16]) {
	int user_count = 0;

	fseek(file, 0, SEEK_SET);

	fread(&user_count, sizeof(int), 1, file);

	//for(int i = 0; i <)

	fseek(file, 0, SEEK_END);
	
	fwrite(username, 16, 1, file);


}
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