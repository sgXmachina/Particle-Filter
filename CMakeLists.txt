cmake_minimum_required(VERSION 2.8)

project(str_hw3)
message("project root is: ${str_hw3_SOURCE_DIR}")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${str_hw3_SOURCE_DIR}/bin)
message("Executables saved to: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

find_package(Boost REQUIRED COMPONENTS 
	system
	filesystem
	iostreams)
message("Boost found: ${Boost_FOUND}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${str_hw3_SOURCE_DIR}/cmake")
find_package(LibConfig++ REQUIRED)
message("Libconfig++ found: ${LIBCONFIG_FOUND}")

find_package(SFML 2 REQUIRED system window graphics)

include_directories(
	${str_hw3_SOURCE_DIR}/include
	${Boost_INCLUDE_DIRS}
	${LIBCONFIGPP_INCLUDE_DIRS}
	${SFML_INCLUDE_DIRS})

add_library(str_libs 
	src/bee-map.cpp
	src/helper_functions.cpp
	src/particle_filter.cpp
	src/str_io.cpp
	src/str_motion_model.cpp
	src/str_sensor_model.cpp	
	src/str_observation_model.cpp
	src/str_ray_tracer.cpp	
	)

################################################
## Main executable 
################################################

add_executable(run_particle_filter
	src/run_particle_filter.cpp 
	src/grapher.cpp
	
	)
target_link_libraries(run_particle_filter
	${Boost_LIBRARIES}
	${LIBCONFIGPP_LIBRARIES}
	${SFML_LIBRARIES}
	str_libs

	)



################################################
## Tests 
################################################

#add_executable(robot_viz_test
#	src/test/robot_viz_test.cpp 
#	src/grapher.cpp
#	)
#target_link_libraries(robot_viz_test
#	${Boost_LIBRARIES}
#	str_libs
#	)
#
#
#add_executable(test_ray_tracer
#	src/test/test_ray_tracer.cpp
#	src/grapher.cpp
#	)
#target_link_libraries(test_ray_tracer
#	${Boost_LIBRARIES}
#	str_libs
#	)
#
add_executable(test_observation_model
	src/test/test_observation_model.cpp
	src/grapher.cpp
	)
target_link_libraries(test_observation_model
	${Boost_LIBRARIES}
	${SFML_LIBRARIES}
	${LIBCONFIGPP_LIBRARIES}
	str_libs

	)
#
#
##add_executable(libconfig_example
##	src/libconfig_example.cpp 
##	src/str_sensor_model.cpp
##	src/bee-map.cpp
##	src/str_io.cpp
##	)
##target_link_libraries(libconfig_example
##	${Boost_LIBRARIES}
##	${LIBCONFIGPP_LIBRARIES}
##	)
#
#add_executable(sfml_example
#	src/test/sfml_example.cpp
#	src/grapher.cpp
#	)
#target_link_libraries(sfml_example
#	${Boost_LIBRARIES}
#	${LIBCONFIGPP_LIBRARIES}
#	${SFML_LIBRARIES}
#	str_libs
#	)
#
#add_executable(test_motion_model
#	src/test/test_motion_model.cpp
#	src/grapher.cpp
#	)
#
#target_link_libraries(test_motion_model
#	${LIBCONFIGPP_LIBRARIES}
#	${SFML_LIBRARIES}
#	str_libs
#	)