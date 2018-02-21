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

### ECM Library ###
file(GLOB_RECURSE SOURCES_ECM lib/ecm/*.cpp lib/ecm/*.h)
add_library(lib-ecm STATIC ${SOURCES_ECM})
target_include_directories(lib-ecm INTERFACE "${CMAKE_SOURCE_DIR}/lib/ecm/" SYSTEM PRIVATE ${SFML_INCS})

### Renderer System ###
file(GLOB_RECURSE SOURCES_RENDERER lib/renderer/*.cpp lib/renderer/*.h)
add_library(renderer-system STATIC ${SOURCES_RENDERER})
target_include_directories(renderer-system INTERFACE "${CMAKE_SOURCE_DIR}/lib/renderer" SYSTEM PRIVATE ${SFML_INCS})

### Input System ###
file(GLOB_RECURSE SOURCES_INPUT lib/input/*.cpp lib/input/*.h)
add_library(input-system STATIC ${SOURCES_INPUT})
target_include_directories(input-system INTERFACE "${CMAKE_SOURCE_DIR}/lib/input" SYSTEM PRIVATE ${SFML_INCS})
target_link_libraries(input-system renderer-system)

### Box2D test ###
file(GLOB_RECURSE Box2DTestSources box2dtest/*.cpp box2dtest/*.h)
add_executable(Box2DTest ${Box2DTestSources})
target_include_directories(Box2DTest SYSTEM PRIVATE ${SFML_INCS} ${BOX2D_INCS})
target_link_libraries(Box2DTest sfml-graphics Box2D)
add_dependencies(Box2DTest copy_resources)
