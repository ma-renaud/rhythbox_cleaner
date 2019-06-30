# Set local variables
set(localDirectory
        ${${PROJECT_NAME}_INC_DIRECTORYS}
        )
#---------------

# Add specific directory to the list
list(APPEND localDirectory
        ${CMAKE_CURRENT_SOURCE_DIR}
        )
#---------------

# Update the project lists
set(${PROJECT_NAME}_INC_DIRECTORYS ${localDirectory} CACHE INTERNAL "")
#---------------