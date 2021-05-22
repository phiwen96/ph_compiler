cmake_minimum_required (VERSION 3.19.4)
get_filename_component (self ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project (
	${self}
	VERSION 1.0.0
	DESCRIPTION "Very nice project"
	LANGUAGES CXX
)


add_subdirectory (cmake/phake)


if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	
	add_compile_definitions (RUN_CATCH)
	# Let's ensure -std=g++xx instead of -std=c++xx
	set (CMAKE_CXX_STANDARD 20)
	set (CMAKE_CXX_STANDARD_REQUIRED ON)
	set (CMAKE_CXX_EXTENSIONS ON)

	# Let's nicely support folders in IDEs
	set_property (GLOBAL PROPERTY USE_FOLDERS ON)



	# Testing only available if this is the main app
	# Note this needs to be done in the main CMakeLists
	# since it calls enable_testing, which must be in the
	# main CMakeLists.
	include (CTest)




endif()

find_package (Boost 1.74 REQUIRED)

add_subdirectory (src)


# if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
	add_subdirectory (tests)
# endif()