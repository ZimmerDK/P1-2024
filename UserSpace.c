#include "UserSpace.h"


void user_space_main(UserPreferences_t* user_prefs, workout_days_t* workout_plan, FILE* userfile) {
	int running = 1;
	while (running) {
		char input[17];

		printf("\nSelect from the following menu:");
		printf("\n1. Start workout");
		printf("\n2. Recalibrate");
		printf("\n3. Change preferences");
		printf("\n4. View report");
		printf("\n5. Exit");
		printf("\n\nEnter your choice: ");
		scanf("%s", input);

		int choice = atoi(input);

		switch (choice) {
		case START_WORKOUT:
			user_start_workout(user_prefs, workout_plan);
			break;

		case RECALIBRATE:
			user_recalibrate();
			break;

		case CHANGE_PREFERENCES:
			user_change_preferences();
			break;

		case VIEW_REPORT:
			user_view_report();
			break;

		case 5:
			exit(69420);
			break;
		}

		// Clear console
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
		system("cls");
#else
		system("clear");
#endif

	}
}

void user_start_workout(UserPreferences_t* user_prefs, workout_days_t* workout_plan) {

	print_workout_program(workout_plan, user_prefs->days);

	printf("Select workout (%d-%d): ", 1, user_prefs->days);
	
	int selected_index = 0;
	scanf("%d", &selected_index);
	
	workout_days_t** user_selected_workout = (workout_days_t**)malloc(sizeof(workout_days_t*) * 1);

	*user_selected_workout = &(workout_plan[selected_index - 1]);

	run_day(*user_selected_workout);

}

void user_recalibrate() {
	//calibrate_workout_routine();
}

void user_view_report() {

}

void user_change_preferences(FILE* userFILE) {
	user_setup(userFILE);
}