# Set local variables
set(localSources
        ${${PROJECT_NAME}_SOURCES}
        )
#---------------

# Add specific source to the list
list(APPEND localSources
        ${CMAKE_CURRENT_SOURCE_DIR}/menucli.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/appcli.cpp
        )
#---------------

# Update the project lists
set(${PROJECT_NAME}_SOURCES ${localSources} CACHE INTERNAL "")
#---------------