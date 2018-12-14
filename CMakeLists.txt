cmake_minimum_required(VERSION 2.6)
project(Mastermind)

include_directories(include/)
set(CMAKE_CXX_COMPILER "mpic++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -Wall -Wextra -pedantic")

add_executable(${PROJECT_NAME}
  # List all source files here (only .cpp, not headers)
  "src/main.cpp"
  "src/Master.cpp"
  "src/mastermind.cpp"
  "src/Challenger.cpp"
  )

# Copy the job (.pbs) file to build dir
set(JOB_FILE "mastermind.pbs")
add_custom_target(${JOB_FILE} ALL DEPENDS ${PROJECT_NAME} COMMAND cp -r "../${JOB_FILE}" "${CMAKE_BINARY_DIR}")

add_custom_target(transfer COMMAND scp -r ../src ../include ../build.sh ../${JOB_FILE} ../CMakeLists.txt hydra:mastermind)

add_custom_target(run COMMAND mpirun ${PROJECT_NAME} DEPENDS ${PROJECT_NAME})

add_custom_target(submit COMMAND qsub ${JOB_FILE} DEPENDS ${PROJECT_NAME} ${JOB_FILE})

add_custom_target(display_executable_size ALL COMMAND du -sh ${PROJECT_NAME} | sed 's/${PROJECT_NAME}//' DEPENDS ${PROJECT_NAME})
