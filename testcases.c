#include <windows.h>
#include <stdio.h>
#include <string.h>

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
			.prefered_days = 7,
			.prefered_time = 60,
			.workout_counter = 0
		},
		.expectedResult = (workout_days_t[]) {
			{
				.compound = {0, 0, 1, 1, 1},
				.secondary = {0, 1, 1, 1},
				.tertiary = {0, 0, 0, 0, 0, 0, 1, 1, 1}
			},
            {
                .compound = {0, 0, 1, 1, 1},
                .secondary = {0, 1, 1, 1},
                .tertiary = {0, 0, 0, 0, 0, 0, 1, 1, 1}
            },
            {
                .compound = {0, 0, 1, 1, 1},
                .secondary = {0, 1, 1, 1},
                .tertiary = {0, 0, 0, 0, 0, 0, 1, 1, 1}
            },
		}
	},
    {
		.userPrefs = {
			.prefered_days = 5,
			.prefered_time = 30,
			.workout_counter = 0
		},
		.expectedResult = (workout_days_t[]) {
			{
				.compound = {1, 0, 1, 0, 1},
				.secondary = {0, 1, 0, 0},
				.tertiary = {1, 0, 1, 0, 1, 0, 1, 0, 1}
			},
            {
                .compound = {1, 0, 1, 0, 1},
                .secondary = {0, 1, 1, 1},
                .tertiary = {0, 0, 1, 0, 0, 0, 1, 0, 1}
            },
            {
                .compound = {0, 1, 1, 0, 1},
                .secondary = {0, 1, 0, 1},
                .tertiary = {0, 0, 1, 0, 1, 0, 1, 0, 1}
            },
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
                    goto failed;
				}
			}
			for (int k = 0; k < AMOUNT_SECONDARY; k++) {
				if (result[j].secondary[k] != workout_generation_testcases[i].expectedResult[j].secondary[k]) {
					_writelog("FAILURE: Secondary exercise %d on day %d\n", k, j);
					failed = 1;
                    goto failed;
				}
			}
			for (int k = 0; k < AMOUNT_TERTIARY; k++) {
				if (result[j].tertiary[k] != workout_generation_testcases[i].expectedResult[j].tertiary[k]) {
					_writelog("FAILURE: Tertiary exercise %d on day %d\n", k, j);
					failed = 1;
                    goto failed;
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

// An array that stores function pointers to tests, they return int (amount of failed tests) and take a output buffer as argument

test_t tests[] = {
	{
        .name = "Workout generation",
		.fnPtr = _test_workout_generation
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






// LATER STUFFS

/*
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
        return 1;
    }

    CloseHandle(hChildStdoutWr);
    CloseHandle(hChildStdinRd);

    // Switch pipe to non-blocking mode
    dwMode = PIPE_READMODE_BYTE | PIPE_NOWAIT;
    SetNamedPipeHandleState(hChildStdoutRd, &dwMode, NULL, NULL);

    char buffer[128];
    DWORD bytesRead, bytesWritten;

    // Example command
    const char* input = "yes\n";
    WriteFile(hChildStdinWr, input, strlen(input), &bytesWritten, NULL);
    FlushFileBuffers(hChildStdinWr);

    // Continuous read loop
    while (1) {
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

        WriteFile(hChildStdinWr, input, strlen(input), &bytesWritten, NULL);
        FlushFileBuffers(hChildStdinWr);
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(hChildStdoutRd);
    CloseHandle(hChildStdinWr);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
*/