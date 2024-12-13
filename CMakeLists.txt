# CMakeList.txt : CMake project for P1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Flag for testing, if 0 a testing executable will not be created, if any other value a testing executable will be created
set(TESTING 1)

# _CRT_SECURE_NO_WARNINGS
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("P1")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/userfiles/accounts.dat ${CMAKE_CURRENT_BINARY_DIR}/userfiles/accounts.dat COPYONLY)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/testfiles DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# Add source to this project's executable.
add_executable (P1 "P1.c" "P1.h" "workout_program.c" "workout_program.h" "exercises.c" "exercises.h" "UserData.c" "UserData.h" "UserSpace.c" "UserSpace.h" "hashmap.c" "hashmap.h" "progress_report.c" "progress_report.h"
        exercises.c
        exercises.h
        workout_program.c
        workout_program.h
        UserData.c
        UserData.h
        UserSpace.c
        UserSpace.h
        hashmap.c
        hashmap.h
        progress_report.c
        progress_report.h
 "Algorithms.c" "Algorithms.h")


# only create a testing executable if TESTING is not 0 and the target os is windows
if (TESTING AND WIN32)
  # Add source to this project's executable.
  add_executable (TEST_P1 "workout_program.c" "workout_program.h" "exercises.c" "exercises.h" "UserData.c" "UserData.h" "UserSpace.c" "UserSpace.h" "hashmap.c" "hashmap.h" "progress_report.c" "progress_report.h"
        exercises.c
        exercises.h
        workout_program.c
        workout_program.h
        UserData.c
        UserData.h
        UserSpace.c
        UserSpace.h
        hashmap.c
        hashmap.h
        progress_report.c
        progress_report.h
 "Algorithms.c" "Algorithms.h" "testcases.c")
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET P1 PROPERTY CXX_STANDARD 20)
endif()

#add_executable(workout_program workout_program.c workout_program.h exercises.c exercises.h)

# TODO: Add tests and install targets if needed.