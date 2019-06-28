cmake_minimum_required(VERSION 3.14)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(rhythbox_cleaner)

find_package(Threads REQUIRED)
find_package( MXML REQUIRED )

add_subdirectory(sources)
add_subdirectory(includes)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_INC_DIRECTORYS})
target_link_libraries(${PROJECT_NAME} PRIVATE MXML Threads::Threads)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME} PUBLIC -Wunused-parameter -Wextra -Wall)

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    target_compile_options(${PROJECT_NAME} PUBLIC -O0)
else ()
    target_compile_options(${PROJECT_NAME} PUBLIC -O3)
endif ()
