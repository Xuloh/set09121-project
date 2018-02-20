cmake_minimum_required(VERSION 3.9)
#Require modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Games_Engineering_Project)

### Setup Directories ###
# Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Output all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

### Add External Dependencies ###
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

### copy resources to build directory ###
add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
"${PROJECT_SOURCE_DIR}/res"
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res)

### Box2D lib ###
add_subdirectory("lib/box2d/Box2D")
set(BOX2D_INCS "lib/box2d/Box2D")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

### Box2D test ###
file(GLOB_RECURSE Box2DTestSources box2dtest/*.cpp box2dtest/*.h)
add_executable(Box2DTest ${Box2DTestSources})
target_include_directories(Box2DTest SYSTEM PRIVATE ${SFML_INCS} ${BOX2D_INCS})
target_link_libraries(Box2DTest sfml-graphics Box2D)
add_dependencies(Box2DTest copy_resources)

