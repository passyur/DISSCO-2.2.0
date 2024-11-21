cmake_minimum_required(VERSION 3.25)

project(CMOD)
message(STATUS "== CMOD ==")

find_package(XercesC REQUIRED)
find_package(SndFile REQUIRED)

file(GLOB_RECURSE CMOD_SRC
    "${PROJECT_SOURCE_DIR}/*.cpp"
    "${PROJECT_SOURCE_DIR}/*.h"
)

add_executable(CMOD ${CMOD_SRC})

### libsndfile must be linked in just the same way as it is in LASS ###
target_link_libraries(CMOD PRIVATE ${SNDFILE_LIBRARY})
target_link_libraries(CMOD PRIVATE ${XercesC_LIBRARY})
target_link_libraries(CMOD PRIVATE LASS)
message(STATUS "libsndfile, libxerces-c, and LASS includes linked with target!")

### _external_ library checkers for informative errors ###
if(NOT SNDFILE_FOUND)
    message(SEND_ERROR "Couldn't find libsndfile, won't go well; check BUILDING docs")
else()
    message(STATUS "Found libsndfile install!")
endif()

if(NOT XercesC_FOUND)
    message(SEND_ERROR "Couldn't find libxerces-c, won't go well; check BUILDING docs")
else()
    message(STATUS "Found libxerces-c install!")
endif()