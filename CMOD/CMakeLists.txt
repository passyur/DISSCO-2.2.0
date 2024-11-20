cmake_minimum_required(VERSION 3.25)

project(CMOD)
message(STATUS "== CMOD ==")

find_package(XercesC REQUIRED)
find_package(SndFile REQUIRED)

include_directories(${XercesC_INCLUDE_DIR})
message(STATUS "Added libxerces-c includes!")

include_directories(${SNDFILE_INCLUDE_DIR})
message(STATUS "Added libsndfile includes!")

file(GLOB_RECURSE CMOD_SRC
    "${PROJECT_SOURCE_DIR}/*.cpp"
    "${PROJECT_SOURCE_DIR}/*.h"
)

add_executable(CMOD ${CMOD_SRC})

target_link_libraries(CMOD PRIVATE ${SNDFILE_LIBRARY})
target_link_libraries(CMOD PRIVATE ${XercesC_LIBRARY})
target_link_libraries(CMOD PUBLIC LASS)
message(STATUS "libsndfile, libxerces-c, and LASS includes linked with target!")

target_include_directories(CMOD PUBLIC
    ${SRC_DIR}
    ${SNDFILE_INCLUDE_DIR}
    ${XercesC_INCLUDE_DIR}
    ${LASS_INCLUDE_DIR}
)
message(STATUS "libsndfile, libxerces-c, and LASS included in target!")

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

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