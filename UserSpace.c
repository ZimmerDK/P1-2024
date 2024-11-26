#include "UserSpace.h"
#include "progress_report.h"


void user_space_main(user_file_header_prefs* userPrefs, workout_days_t* workout_plan, FILE* userFILE) {
	int running = 1;
	int _days = 0;
	int _time = 0;
	while (running) {
		char input[17];

		printf("\nSelect from the following menu:");
		printf("\n1. Print Workout Plan");
		printf("\n2. Recalibrate");
		printf("\n3. Change preferences");
		printf("\n4. View report");
		printf("\n5. Exit");
		printf("\n\nEnter your choice: ");
		scanf(" %s", input); printf("\n");

		int choice = atoi(input);

		switch (choice) {
		case PRINT_WORKOUT_PLAN:
			//*userPrefs = read_user_preferences(userFILE);
			//workout_days_t* workout = (workout_days_t*)generate_workout_program(*userPrefs);
			print_workout_program(workout_plan, userPrefs->prefered_days);
			char c_input;
			want_to_start_workout:
			printf("Do you want to start the workout? (y/n)\n");
			scanf(" %c", &c_input);
			if (c_input == 'n') {
				continue;
			} else if (c_input == 'y') {
				user_start_workout(userPrefs, workout_plan);
			} else {
				printf("ERROR: Please enter valid input ('y' or 'n')\n");
				goto want_to_start_workout;
			}
			continue;

		case RECALIBRATE:
			user_recalibrate();
			continue;

		case CHANGE_PREFERENCES:
			user_setup(local_userFILE, &_days, &_time);
			userPrefs->prefered_days = _days;
			userPrefs->perfered_time = _time;
			update_user_preferences(userPrefs);
			workout_plan = generate_workout_program(*userPrefs);
			update_user_workout_data(workout_plan);
			continue;

		case VIEW_REPORT:
			user_view_report();
			continue;

		case 5:
			exit(69420);
			break;
		default:
			printf("ERROR: Please enter valid choice\n");
		}

		// Clear console
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
		system("cls");
#else
		system("clear");
#endif

	}
}


void user_start_workout(user_file_header_prefs* user_prefs, workout_days_t* workout_plan) {

	//print_workout_program(workout_plan, user_prefs->prefered_days);
	
	int selected_index = 0;
	do {
		printf("Select workout (%d-%d): ", 1, user_prefs->prefered_days);
		scanf(" %d", &selected_index); printf("\n");
		if (selected_index < 1 || selected_index > user_prefs->prefered_days) {
			printf("\nERROR: Invalid selection  |  Please input valid day\n");
		}
	} while (selected_index < 1 || selected_index > user_prefs->prefered_days);

	workout_days_t** user_selected_workout = (workout_days_t**)malloc(sizeof(workout_days_t*) * 1);

	*user_selected_workout = &(workout_plan[selected_index - 1]);
	
	write_user_data_post_workout();

	run_day(*user_selected_workout);

	update_user_exercise_data();

	print_progress_report();

}

void user_recalibrate() {
	// TODO:
	calibrate_workout_routine(exercises_c);
}

void user_view_report() {
	// TODO: USER REPORT
}
