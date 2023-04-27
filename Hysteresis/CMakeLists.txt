cmake_minimum_required(VERSION 3.5)
project(Tester)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories("codeToTest")

file(GLOB_RECURSE SRC "test/*.c" "test/*.cpp" "src/*.c" "src/*.cpp")
  
# Create the exe.
add_executable(${PROJECT_NAME}
  ${SRC}
)
  
#All libraries this project depends on
target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} pthread)
