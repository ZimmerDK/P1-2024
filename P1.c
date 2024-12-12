// P1.cpp : Defines the entry point for the application.
//
#include "P1.h"

#include "exercises.h"

#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>

int main(void) {

	//char* buffer = malloc(4096);
	//int what = setvbuf(stdout, NULL, _IOFBF, 4096);
	
	setvbuf(stdout, NULL, _IONBF, 0);

	printf("TEST?????????");
	//fflush(stdout);

	establish_userdata_context();

	fflush(stdout);

	if (local_userContext.userPrefs->workout_counter == -1) {
		calibrate_workout_routine(exercises_c);
		// Set the workout counter to 0
		local_userContext.userPrefs->workout_counter = 0;
		update_user_preferences(&local_userContext);
	}

	workout_days_t* workout = read_user_workout_data(&local_userContext);
	
	user_space_main(&local_userContext);
	

	return 0;
}
