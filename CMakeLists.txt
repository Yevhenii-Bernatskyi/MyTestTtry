# Specifies the minimum required version for CMake compilation
cmake_minimum_required(VERSION 3.14)
# Name the project
project(test1)

# Collect c/c + + files and assign them to the variable SRC_LIST_C
# ${CMAKE_CURRENT_SOURCE_DIR} represents the current item directory of CMakeLists.txt
#file(GLOB
	#    SRC_LIST_C
    #    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
    #)

# Specify the. h header file directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/gtest/include
)

# Specify the. dll link library file directory
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/gtest/lib)

add_library(libgtest STATIC IMPORTED) # or STATIC instead of SHARED
set_target_properties(libgtest PROPERTIES
	IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/gtest/lib/libgtest.so"
)

add_library(libgtest_main STATIC IMPORTED) # or STATIC instead of SHARED
set_target_properties(libgtest_main PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/gtest/lib/libgtest_main.so"
)


# Generate the c file into the executable sample1.exe
add_executable(test1 ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp)

# Specify the sample1 link library libgtest.dll and libgtest_main.dll
target_link_libraries(test1
                      libgtest
                      libgtest_main
)
