# Set local variables
set(localSources
        ${${PROJECT_NAME}_SOURCES}
        )
#---------------

# Add specific source to the list
list(APPEND localSources
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/rhythmdbxml.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/rhythmdb.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/entry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/song.cpp
        )
#---------------

# Update the project lists
set(${PROJECT_NAME}_SOURCES ${localSources} CACHE INTERNAL "")
#---------------