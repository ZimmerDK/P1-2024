#include "UserSpace.h"
#include "progress_report.h"

#include <float.h>

/**@brief Main user interaction loop for the workout application
 *
 * Provides a menu-driven interface for users to interact with their workout plan,
 * including options to print workout plan, recalibrate exercises, change preferences,
 * view reports, and exit the application.
 * @param userPrefs Pointer to user preference settings
 * @param workout_plan Pointer to the generated workout plan
 * @param userFILE File pointer for user data storage*/
void user_space_main(user_file_header_prefs* userPrefs, workout_days_t* workout_plan, FILE* userFILE) {
	int running = 1;    // Flag to control the main program loop
	int _days = 0; int _time = 0;  // Temporary variables for user preferences

	while (running) {
		char input[17];  // Buffer for user menu choice input

		// Display menu options
		printf("\nSelect from the following menu:");
		printf("\n1. Print Workout Plan");
		printf("\n2. Recalibrate");
		printf("\n3. Change preferences");
		printf("\n4. View report");
		printf("\n5. Exit");
		printf("\n\nEnter your choice: ");
		scanf(" %s", input); printf("\n");

		int choice = atoi(input);  // Convert input to integer

		switch (choice) {
		case PRINT_WORKOUT_PLAN:
			// Display current workout plan
			print_workout_program(workout_plan, userPrefs->prefered_days);
			char c_input;
			want_to_start_workout:  // Label for goto error handling
			printf("Do you want to start the workout? (y/n)\n");
			scanf(" %c", &c_input);
			if (c_input == 'n') {
				continue;  // Return to main menu
			} else if (c_input == 'y') {
				user_start_workout(userPrefs, workout_plan);  // Begin selected workout
			} else {
				printf("ERROR: Please enter valid input ('y' or 'n')\n");
				goto want_to_start_workout;  // Retry input
			}
			continue;

		case RECALIBRATE:
			user_recalibrate();  // Open exercise recalibration submenu
			continue;

		case CHANGE_PREFERENCES:
			// Update user preferences
			user_preferences_prompt(local_userFILE, &_days, &_time);
			userPrefs->prefered_days = _days;
			userPrefs->perfered_time = _time;
			update_user_preferences(userPrefs);

			// Regenerate workout plan based on new preferences
			workout_plan = generate_workout_program(*userPrefs);
			update_user_workout_data(workout_plan);
			continue;

		case VIEW_REPORT:
			user_view_statusreport();  // Placeholder for workout report
			continue;

		case 5:
			exit(69420);  // Exit the program with a custom exit code
			break;
		default:
			printf("ERROR: Please enter valid choice\n");
		}

		// Clear console for different operating systems
		#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
			system("cls");  // Clear screen on Windows
		#else
			system("clear");  // Clear screen on Unix-like systems
		#endif
	}
}

/**@brief Starts a user-selected workout
 *
 * Prompts the user to select a specific day from their workout plan,
 * validates the selection, and runs the chosen workout.
 * @param user_prefs Pointer to user preference settings
 * @param workout_plan Pointer to the generated workout plan*/
void user_start_workout(user_file_header_prefs* user_prefs, workout_days_t* workout_plan) {
	int selected_index = 0;
	do {
		// Prompt user to select a workout day
		printf("Select workout (%d-%d): ", 1, user_prefs->prefered_days);
		scanf(" %d", &selected_index); printf("\n");

		// Validate user input
		if (selected_index < 1 || selected_index > user_prefs->prefered_days) {
			printf("\nERROR: Invalid selection  |  Please input valid day\n");
		}
	} while (selected_index < 1 || selected_index > user_prefs->prefered_days);

	// Allocate memory for the selected workout
	workout_days_t** user_selected_workout = (workout_days_t**)malloc(sizeof(workout_days_t*) * 1);

	// Point to the selected workout day
	*user_selected_workout = &(workout_plan[selected_index - 1]);

	// Log pre-workout data
	write_user_data_post_workout();

	// Execute the selected workout
	run_day(*user_selected_workout);

	// Update exercise data after workout
	update_user_exercise_data();
}

/**@brief Provides a submenu for exercise recalibration
 *
 * Offers options to:
 * - Recalibrate all exercises
 * - Recalibrate a specific exercise
 * - Manually edit weight for a specific exercise*/
void user_recalibrate() {
	char recalibrate_input[17];  // Buffer for user input

	// Display recalibration menu
	printf("\nSelect from the following menu:");
	printf("\n1. Recalibrate ALL exercises");
	printf("\n2. Recalibrate SPECIFIC exercise");
	printf("\n3. Manually edit weight for specific exercise");
	printf("\n4. Return to Main Menu");
	printf("\n\nEnter your choice: ");
	scanf(" %s", recalibrate_input); printf("\n");

	int choice = atoi(recalibrate_input);  // Convert input to integer
	int chosen_index = 0;

	switch (choice) {
		case RECALIBRATE_ALL:
			// Recalibrate entire workout routine
			calibrate_workout_routine(exercises_c);
			break;
		case RECALIBRATE_SPECIFIC:
			// Display list of exercises
			printf("EXERCISES LIST:\n");
			for (int i = 0; i < AMOUNT_EXERCISES; i++) {
				printf("%d = %s\n", i, exercises_c[i].name);
			}

			// Prompt for specific exercise to recalibrate
			printf("Input desired exercise to recalibrate: ");
			scanf(" %d", &chosen_index);
			recalibrate_specific_exercise(exercises_c, chosen_index);
			break;
		case MANUALLY_EDIT_SPECIFIC:
			// Display list of exercises
			printf("EXERCISES LIST:\n");
			for (int i = 0; i < AMOUNT_EXERCISES; i++) {
				printf("%d = %s\n", i, exercises_c[i].name);
			}

			// Prompt for specific exercise to manually edit
			printf("Input desired exercise to manually edit: ");
			scanf(" %d", &chosen_index);
			manual_exercise_edit(exercises_c, chosen_index);
			break;
		case 4:
			break;  // Return to main menu
		default:
			printf("ERROR: Please enter valid choice\n");
	}
}

/**@brief Recalibrates a specific exercise based on user-reported intensity
 *
 * Interactively adjusts the exercise weight based on the user's perceived
 * intensity, with the goal of matching the estimated exercise intensity.
 * @param exercises Array of exercise structures
 * @param index Index of the exercise to recalibrate*/
void recalibrate_specific_exercise(exercise_t exercises[], int index) {
	const int reps = 5;  // Default number of repetitions

	// Initialize variables for intensity and weight adjustment
	double estIntensity = exercises->est_intensity;
	double weight_step = exercises->weight_step;
	int start_weight = 0;

	// Use global exercise data for specific exercise
	estIntensity = exercises_c[index].est_intensity;
	weight_step = exercises_c[index].weight_step;
	start_weight = 2;
	exercises[index].user_exercise_data->weight = start_weight;

	int current_intensity;
	printf("Calibrating for %s exercises\n", exercises[index].name);
	do {
		// Display current exercise details
		printf("#############################################\n");
		printf("Exercise: %s\n", exercises[index].name);
		printf("Current Weight: %lf\n", exercises[index].user_exercise_data->weight);
		printf("Current Reps: %d\n", reps);
		printf("#############################################\n\n");

		// Prompt for user-perceived intensity
		printf("Input Intensity: ");
		scanf(" %d", &current_intensity); printf("\n");

		// Adjust weight based on intensity
		if (current_intensity <= 3) {
			exercises[index].user_exercise_data->weight += weight_step*3;
		} else if (current_intensity >= 4 && current_intensity <= 6) {
			exercises[index].user_exercise_data->weight += weight_step*2;
		} else if (current_intensity >= 7 && current_intensity <= 8) {
			exercises[index].user_exercise_data->weight += weight_step;
		} else if (current_intensity == 10) {
			exercises[index].user_exercise_data->weight -= weight_step;
		}
	} while (current_intensity < estIntensity);

	// Display final calibration result
	printf("Final result for %s = %.2lf kg\n\n", exercises[index].name, exercises[index].user_exercise_data->weight);

	// Set repetitions to minimum recommended
	exercises[index].user_exercise_data->reps = exercises[index].min_reps;

	// Update exercise data in file
	if (update_user_exercise_data() != 0) {
		printf("ERROR: Could not update userFILE calibration\n");
		return;
	}
	printf("Calibration finished!\n");
}

/**@brief Allows manual editing of an exercise's weight and repetitions
 *
 * Provides an interface for the user to manually adjust the weight and
 * repetition count for a specific exercise, with input validation.
 * @param exercises Array of exercise structures
 * @param chosen_index Index of the exercise to manually edit*/
void manual_exercise_edit(exercise_t exercises[], int chosen_index) {
	// Validate exercise index
	if (chosen_index < 0 || chosen_index > AMOUNT_EXERCISES-1) {
		printf("\nERROR: Invalid selection  |  Please input valid exercise index\n");
		return;
	}

	// Display current exercise details
	printf("#############################################\n");
	printf("Exercise: %s\n", exercises_c[chosen_index].name);
	printf("Current Weight: %lf\n", exercises_c[chosen_index].user_exercise_data->weight);
	printf("Current Reps: %d\n", exercises_c[chosen_index].user_exercise_data->reps);
	printf("#############################################\n\n");

	// Prompt for new weight with validation
	printf("Input new weight: ");
	do {
		scanf(" %lf", &exercises_c[chosen_index].user_exercise_data->weight);

		// Check weight is within reasonable limits
		if (exercises_c[chosen_index].user_exercise_data->weight > 9999.0 ||
			exercises_c[chosen_index].user_exercise_data->weight < 0.0) {
				printf("\nERROR: Invalid weight |  Please input valid weight: ");
				continue;
		} break;
	} while (1);

	// Prompt for new rep count with validation
	printf("\nInput new rep amount: ");
	do {
		scanf(" %d", &exercises_c[chosen_index].user_exercise_data->reps);

		// Check reps are within exercise-specific limits
		if (exercises_c[chosen_index].user_exercise_data->reps > exercises_c[chosen_index].max_reps ||
			exercises_c[chosen_index].user_exercise_data->reps < exercises_c[chosen_index].min_reps) {
				printf("\nERROR: Invalid reps |  Please input valid reps: ");
			continue;
		} break;
	} while (1);
	printf("\n");

	// Update exercise data in file
	if (update_user_exercise_data() != 0) {
		printf("ERROR: Could not update userFILE calibration\n");
		return;
	}
	printf("Manual edit finished!\n");
}

/**@brief Placeholder function for generating a user workout report
 *
 * Currently unimplemented. Intended to display a summary or report
 * of the user's workout progress and statistics.*/
void user_view_statusreport() {
	// TODO: Implement comprehensive workout report generation
	// Potential features:
	// - Total workouts completed
	// - Progress tracking
	// - Performance metrics
	// - Graphical representation of fitness improvements
}
