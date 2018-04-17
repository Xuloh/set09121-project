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

### copy resources to build directory ###
add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
"${PROJECT_SOURCE_DIR}/res"
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res)

### SFML ###
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

### Box2D ###
add_subdirectory("lib/box2d/Box2D")
set(BOX2D_INCS "lib/box2d/Box2D")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

### Engine ###
file(GLOB_RECURSE SOURCES_ENGINE engine/*.cpp engine/*.h)
add_library(engine STATIC ${SOURCES_ENGINE})
target_include_directories(engine SYSTEM PUBLIC "${CMAKE_SOURCE_DIR}/engine" PRIVATE ${SFML_INCS} PRIVATE ${BOX2D_INCS})

### Box2D test ###
file(GLOB_RECURSE Box2DTestSources box2dtest/*.cpp box2dtest/*.h)
add_executable(Box2DTest ${Box2DTestSources})
target_include_directories(Box2DTest SYSTEM PRIVATE ${SFML_INCS} ${BOX2D_INCS})
target_link_libraries(Box2DTest sfml-graphics Box2D)
add_dependencies(Box2DTest copy_resources)

### Main project : MOIM ###
file(GLOB_RECURSE MOIM_SOURCES src/*.cpp src/*.h)
add_executable(moim ${MOIM_SOURCES})
target_include_directories(moim SYSTEM PRIVATE ${SFML_INCS} PRIVATE ${BOX2D_INCS})
target_link_libraries(moim sfml-graphics sfml-audio Box2D engine)
add_dependencies(moim copy_resources)
