# bash .\run_test.sh
cmake_minimum_required(VERSION 3.14.0)
project(AdventOfCode VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(
	${PROJECT_NAME}
  	main.cpp
	day01.h
	day02.h
	day03.h
	day04.h
	day05.h
	day06.h
	day07.h
	day08.h
	day09.h
	day10.h
	day11.h
	day12.h
	#[[
	day13.h
	day14.h
	day15.h
	day16.h
	day17.h
	day18.h
	day19.h
	day20.h
	day21.h
	day22.h
	day23.h
	day24.h
	day25.h
	#]]
)
	
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../Utils)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
