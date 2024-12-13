#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "Algorithms.h"
#include "workout_program.h"

#define BUFFER_SIZE 0xFFFF

typedef struct {
    int cases;
    int failed;
} test_result_t;

typedef struct {
    char* name;
    test_result_t(*fnPtr)();
} test_t;

void run_tests();

int written_bytes = 0;
char* log_buffer;

void _writelog(char* string, ...) {
	va_list args;
	va_start(args, string);
	written_bytes += vsprintf(log_buffer + written_bytes, string, args);
	va_end(args);
}

void _resetlog() {
	written_bytes = 0;
	log_buffer[0] = '\0';
}

int main(void) {
   run_tests();
}




typedef struct {
    user_file_header_prefs userPrefs; // Mock user preferences
    workout_days_t* expectedResult; // Expected result 
} workout_generation_testcase_t;

workout_generation_testcase_t workout_generation_testcases[] = {
	{
		.userPrefs = {
			.prefered_days = 4,
			.prefered_time = 100,
			.workout_counter = 0
		},
		.expectedResult = (workout_days_t[]) {
			{
				.compound = {1, 0, 1, 0, 1},
				.secondary = {0, 1, 0, 1},
				.tertiary = {0, 1, 0, 1, 0, 0, 0, 0, 0}
			},
            {
                .compound = {0, 1, 0, 1, 0},
                .secondary = {1, 0, 1, 0},
                .tertiary = {1, 0, 1, 0, 1, 0, 0, 0, 0}
            },
            {
                .compound = {1, 0, 1, 0, 1},
                .secondary = {0, 1, 0, 1},
                .tertiary = {0, 1, 0, 1, 0, 0, 0, 0, 0}
            },
            {
                .compound = {0, 1, 0, 1, 0},
                .secondary = {1, 0, 1, 0},
                .tertiary = {1, 0, 1, 0, 1, 0, 0, 0, 0}
            },
		}
	},
    {
		.userPrefs = {
			.prefered_days = 3,
			.prefered_time = 30,
			.workout_counter = 0
		},
		.expectedResult = (workout_days_t[]) {
			{
				.compound = {1, 0, 0, 0, 0},
				.secondary = {0, 0, 0, 0},
				.tertiary = {1, 0, 0, 0, 0, 0, 0, 0, 0}
			},
            {
                .compound = {0, 1, 0, 0, 0},
                .secondary = {0, 0, 0, 0},
                .tertiary = {0, 1, 0, 0, 0, 0, 0, 0, 0}
            },
            {
                .compound = {0, 0, 1, 0, 0},
                .secondary = {0, 0, 0, 0},
                .tertiary = {0, 0, 1, 0, 0, 0, 0, 0, 0}
            },
		}
	},
    {
        .userPrefs = {
            .prefered_days = 7,
            .prefered_time = 120,
            .workout_counter = 0
        },
        .expectedResult = (workout_days_t[]) {
            {
                .compound = {1, 0, 0, 1, 0},
                .secondary = {0, 1, 0, 0},
                .tertiary = {1, 0, 0, 1, 0, 0, 1, 0, 0}
            },
            {
                .compound = {0, 1, 0, 0, 1},
                .secondary = {0, 0, 1, 0},
                .tertiary = {0, 1, 0, 0, 1, 0, 0, 1, 0}
            },
            {
                .compound = {0, 0, 1, 0, 0},
                .secondary = {1, 0, 0, 1},
                .tertiary = {0, 0, 1, 0, 0, 1, 0, 0, 1}
            },
                {
                .compound = {1, 0, 0, 1, 0},
                .secondary = {0, 1, 0, 0},
                .tertiary = {1, 0, 0, 1, 0, 0, 1, 0, 0}
            },
            {
                .compound = {0, 1, 0, 0, 1},
                .secondary = {0, 0, 1, 0},
                .tertiary = {0, 1, 0, 0, 1, 0, 0, 1, 0}
            },
            {
                .compound = {0, 0, 1, 0, 0},
                .secondary = {1, 0, 0, 1},
                .tertiary = {0, 0, 1, 0, 0, 1, 0, 0, 1}
            },
            {
                .compound = {1, 0, 0, 1, 0},
                .secondary = {0, 1, 0, 0},
                .tertiary = {1, 0, 0, 1, 0, 0, 1, 0, 0}
            }
        }
    },
    {
        .userPrefs = {
            .prefered_days = 1,
            .prefered_time = 15,
            .workout_counter = 0
        },
        .expectedResult = (workout_days_t[]) {
            {
                .compound = {0, 0, 0, 0, 0},
                .secondary = {1, 0, 0, 0},
                .tertiary = {0, 0, 0, 0, 0, 0, 0, 0, 0}
            }
        }
    },
    {
        .userPrefs = {
            .prefered_days = 3,
            .prefered_time = 15,
            .workout_counter = 0
        },
        .expectedResult = (workout_days_t[]) {
            {
                .compound = {0, 0, 0, 0, 0},
                .secondary = {1, 0, 0, 0},
                .tertiary = {0, 0, 0, 0, 0, 0, 0, 0, 0}
            },
			{
				.compound = {0, 0, 0, 0, 0},
				.secondary = {0, 1, 0, 0},
				.tertiary = {0, 0, 0, 0, 0, 0, 0, 0, 0}
			},
            {
                .compound = {0, 0, 0, 0, 0},
                .secondary = {0, 0, 1, 0},
                .tertiary = {0, 0, 0, 0, 0, 0, 0, 0, 0}
            }
        }
    }
};

test_result_t _test_workout_generation() {

    const int count = sizeof(workout_generation_testcases) / sizeof(workout_generation_testcases[0]);
	int failed_tests = 0;

	for (int i = 0; i < count; i++) {
		_writelog("Running test case %d\n", i);
		workout_days_t* result = generate_workout_program(workout_generation_testcases[i].userPrefs);
        char failed = 0;
        for (int j = 0; j < workout_generation_testcases[i].userPrefs.prefered_days; j++) {
            for (int k = 0; k < AMOUNT_COMPOUND; k++) {
				if (result[j].compound[k] != workout_generation_testcases[i].expectedResult[j].compound[k]) {
					_writelog("FAILURE: Compound exercise %d on day %d\n", k, j);
                    failed = 1;
                    //goto failed;
				}
			}
			for (int k = 0; k < AMOUNT_SECONDARY; k++) {
				if (result[j].secondary[k] != workout_generation_testcases[i].expectedResult[j].secondary[k]) {
					_writelog("FAILURE: Secondary exercise %d on day %d\n", k, j);
					failed = 1;
                    //goto failed;
				}
			}
			for (int k = 0; k < AMOUNT_TERTIARY; k++) {
				if (result[j].tertiary[k] != workout_generation_testcases[i].expectedResult[j].tertiary[k]) {
					_writelog("FAILURE: Tertiary exercise %d on day %d\n", k, j);
					failed = 1;
                    //goto failed;
				}
			}
		}

	failed:
        if (failed) {
            failed_tests++;
        }
	}

    return (test_result_t){ count, failed_tests };
}

typedef struct {
    set_data_t *data; //mock set data
    user_exercise_data_t exercise_data;//moch user_exercise_data
    workout_result_t workout_result; //expected data
} calculate_workout_testcase_t;

calculate_workout_testcase_t calculate_workout_testcases[] = {
    {
        //Bench test 1
        .data = (set_data_t[]){
            {8}, {8}, {9}
        },
        .exercise_data = {
            .index_number = 1,
            .reps = 10,
            .weight = 10.0,
            .exercise = &(exercises_c[0])
        },

        //calculated score: 8,28. est_intensity 9
        .workout_result = {
            .repChange = -4,
            .weightChange = 2.5
        }
    },
    {
        //squat test 2
        .data = (set_data_t[]){
            {7}, {8}, {10}
        },
        .exercise_data = {
            .index_number = 0,
            .reps = 6,
            .weight = 10.0,
            .exercise = &(exercises_c[1])
        },

        //calculated score: 8,17. est_intesity 9. One is 10 therefore stay
        .workout_result = {
            .repChange = 0,
            .weightChange = 0
        }
    },
    {
        //Test 3 triceps pushdown
        .data = (set_data_t[]){
            {9}, {9}, {9}
        },
        .exercise_data = {
            .index_number = 0,
            .reps = 6,
            .weight = 10.0,
            .exercise = &(exercises_c[10])
        },

        //Calculated score: 9, est_intesity 9. It should stay
        .workout_result = {
            .repChange = 0,
            .weightChange = 0
        }
    },
    {
        //Test 4 Lat Pull-Down
        .data = (set_data_t[]){
            {9}, {10}, {10}
        },
        .exercise_data = {
            .index_number = 0,
            .reps = 6,
            .weight = 10.0,
            .exercise = &(exercises_c[6])
        },

        //calculated score 9,61. est_intesity 9, user should go down
        .workout_result = {
            .repChange = 6,
            .weightChange = -2.5
        }
    },
    {
        //Test 5 Face Pulls
        .data = (set_data_t[]){
            {3}, {7}, {9}
        },
        .exercise_data = {
            .index_number = 0,
            .reps = 12,
            .weight = 10.0,
            .exercise = &(exercises_c[17])
        },
        //Calculated score 6, est_intensity 9, user should go up
        .workout_result = {
            .repChange = -6,
            .weightChange = 2.5
        }
    },
    {
        //Incline Dumbbell-Press test 6
        .data = (set_data_t[]){
            {7}, {7}, {9}
        },
        .exercise_data = {
            .index_number = 0,
            .reps = 12,
            .weight = 10.0,
            .exercise = &(exercises_c[7])
        },
    //calculated score 7,56, est_intensity 9, user should go up 2.0
        .workout_result = {
            .repChange = -6,
            .weightChange = 2.0
        }
    },
    {
        //test 7 Deadlift
        .data = (set_data_t[]){
            {8}, {8}, {9}
        },
        .exercise_data = {
            .index_number = 0,
            .reps = 9,
            .weight = 10.0,
            .exercise = &(exercises_c[2])
        },
        //calculated score 8,28, est_intensity 8,5. No weightchange
        .workout_result = {
            .repChange = 0,
            .weightChange = 0
        }
    },
    {
        //test 8 Leg Extensions
        .data = (set_data_t[]){
            {10}, {9}, {10}
        },
        .exercise_data = {
            .index_number = 0,
            .reps = 10,
            .weight = 10.0,
            .exercise = &(exercises_c[12])
        },
        //calculated score: 9,67. Should go up 4 reps, down -2.5 kg
        .workout_result = {
            .repChange = 4,
            .weightChange = -2.5
        }
    }
};

test_result_t _test_calculate_workout() {
   //workout_result_t calculate_workout(set_data_t * data, user_exercise_data_t * exercise_data, int setCount)
    const int count = sizeof(calculate_workout_testcases) / sizeof(calculate_workout_testcases[0]);
    int failed_tests = 0;


    for (int i = 0; i < count; i++) {
        _writelog("Running test case %d\n", i+1);
        workout_result_t result = calculate_workout(calculate_workout_testcases[i].data, &calculate_workout_testcases[i].exercise_data, 3);
      
        if (result.weightChange != calculate_workout_testcases[i].workout_result.weightChange || result.repChange != calculate_workout_testcases[i].workout_result.repChange) {
            failed_tests++;
            _writelog("Test %d, calculated result: weightchange: %lf, repchange: %d\n", i+1, result.weightChange, result.repChange);
            _writelog("Expected result: weightchange: %lf, repchange: %d\n", calculate_workout_testcases[i].workout_result.weightChange, calculate_workout_testcases[i].workout_result.repChange);
        }
    }
    return (test_result_t) { count, failed_tests };
}

typedef struct {
    char* pathToTest;
    char* username;
    int days;
    int time;
} user_generation_testcase_t;

user_generation_testcase_t user_generation_testcases[] = {
    {
        .days = 3,
        .time = 30,
        .pathToTest = "testfiles/test_3_30.dat",
        .username = "test_3_30"
    },
    {
        .days = 4,
        .time = 100,
        .pathToTest = "testfiles/test_4_100.dat",
        .username = "test_4_100"
    },
    {
        .days = 7,
        .time = 120,
        .pathToTest = "testfiles/test_7_120.dat",
        .username = "test_7_120"
    }
};

test_result_t _test_user_generation() {
    
    const int count = sizeof(user_generation_testcases) / sizeof(user_generation_testcases[0]);
    int failed_tests = 0;
    
    HANDLE hChildStdoutRd, hChildStdoutWr;
    HANDLE hChildStdinRd, hChildStdinWr;
    SECURITY_ATTRIBUTES sa;
    DWORD dwMode;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    // Create pipes
    CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &sa, 0);
    CreatePipe(&hChildStdinRd, &hChildStdinWr, &sa, 0);

    SetHandleInformation(hChildStdoutRd, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(hChildStdinWr, HANDLE_FLAG_INHERIT, 0);

    // Create child process
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.hStdOutput = hChildStdoutWr;
    si.hStdInput = hChildStdinRd;
    si.dwFlags |= STARTF_USESTDHANDLES;

    if (!CreateProcess(NULL, "P1.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "Failed to create process.\n");
        return;
    }

    CloseHandle(hChildStdoutWr);
    CloseHandle(hChildStdinRd);

    // Switch pipe to non-blocking mode
    dwMode = PIPE_READMODE_BYTE | PIPE_NOWAIT;
    SetNamedPipeHandleState(hChildStdoutRd, &dwMode, NULL, NULL);

    char buffer[128];
    DWORD bytesRead, bytesWritten;

    char command[1024];
    int bytes_written = 0;
    for (int i = 0; i < count; i++) {
        bytes_written += sprintf(command + bytes_written, "signup\n%s\n%d\n%d\n", user_generation_testcases[i].username, user_generation_testcases[i].days, user_generation_testcases[i].time);
    }

    bytes_written += sprintf(command + bytes_written, "exit\n");


    // Example command
    WriteFile(hChildStdinWr, command, bytes_written, &bytesWritten, NULL);
    FlushFileBuffers(hChildStdinWr);

    // Continuous read loop
    /*while (1) {
        if (ReadFile(hChildStdoutRd, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
            buffer[bytesRead] = '\0';
            printf("Child output: %s", buffer);
        }
        else {
            DWORD err = GetLastError();
            if (err == ERROR_BROKEN_PIPE) {
                // Child process exited
                break;
            }
            else if (err != ERROR_NO_DATA) {
                fprintf(stderr, "Error reading pipe: %ld\n", err);
                break;
            }
            Sleep(10); // Avoid busy looping
        }

        //WriteFile(hChildStdinWr, input, strlen(input), &bytesWritten, NULL);
        //FlushFileBuffers(hChildStdinWr);
    }*/

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(hChildStdoutRd);
    CloseHandle(hChildStdinWr);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);


    for (int i = 0; i < count; i++) {
		_writelog("Running test case %d\n", i + 1);

        FILE* file = fopen(user_generation_testcases[i].pathToTest, "r");

        char userfilepath[128];
        sprintf(userfilepath, "userfiles/%s.dat", user_generation_testcases[i].username);
        FILE* file2 = fopen(userfilepath, "r");

        if (file == NULL || file2 == NULL) {
			_writelog("Failed to open file %s\n", user_generation_testcases[i].pathToTest);
			failed_tests++;
			continue;
		}

        char c1, c2;
		while ((c1 = fgetc(file)) != EOF && (c2 = fgetc(file2)) != EOF) {
			if (c1 != c2) {
				_writelog("File %s and %s differ\n", user_generation_testcases[i].pathToTest, userfilepath);
				failed_tests++;
				break;
			}
		}

		fclose(file);
		fclose(file2);
	}

    return (test_result_t) { count, failed_tests };
}

// An array that stores function pointers to tests, they return int (amount of failed tests) and take a output buffer as argument
test_t tests[] = {
    {
        .name = "Workout generation",
        .fnPtr = _test_workout_generation
    },
    {
        .name = "Calculate workout",
        .fnPtr = _test_calculate_workout
    },
	{
		.name = "User generation",
		.fnPtr = _test_user_generation
	}
};


void run_tests() {
    
    log_buffer = (char*)malloc(BUFFER_SIZE);

	for (int i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
		printf("Running test: %s......", tests[i].name);
        test_result_t test_result = tests[i].fnPtr();
		if (test_result.failed == 0) {
			printf("%d/%d  [PASSED]\n", test_result.cases-test_result.failed, test_result.cases);
		}
		else {
			printf("%d/%d  [FAILED]\n", test_result.cases-test_result.failed, test_result.cases);
			
			printf("---------------------LOG TRACE---------------------\n\n%s\n\n", log_buffer);
			printf("---------------------------------------------------\n\n");
		}
        _resetlog();
	}
}
