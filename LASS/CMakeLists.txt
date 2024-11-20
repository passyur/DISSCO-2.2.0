cmake_minimum_required(VERSION 3.25)

# include_directories()
# set(SNDFILE_FOUND FALSE)
# set(SNDFILE_INCLUDE_DIR "")
# set(SNDFILE_LIBRARIES "")
project(LASS)
message(STATUS "== LASS ==")

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

set(PROJECT_SOURCE_DIR src)

find_package(SndFile REQUIRED)
# message(STATUS "Found libsndfile!")

include_directories(${SNDFILE_INCLUDE_DIR})
message(STATUS "Added libsndfile includes!")

file(GLOB_RECURSE LASS_SRC
    "${PROJECT_SOURCE_DIR}/*.cpp"
    "${PROJECT_SOURCE_DIR}/*.h"
)

### not exactly sure how we want to deal with these, so disabled for now -Jacob ###
# file(GLOB_RECURSE EXAMPLES_SRC
#     "${EXAMPLES_DIR}/*.cpp"
#     "${EXAMPLES_DIR}/*.cxx"
# )

add_library(LASS STATIC ${LASS_SRC})

target_link_libraries(LASS PRIVATE ${SNDFILE_LIBRARY})
message(STATUS "libsndfile linked with target!")

target_include_directories(LASS PUBLIC
    "${SRC_DIR}"
    ${SNDFILE_INCLUDE_DIR}
)
message(STATUS "libsndfile included in target!")

# get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

if(NOT SNDFILE_FOUND)
    message(SEND_ERROR "Couldn't find libsndfile, won't go well; check BUILDING docs")
else()
    message(STATUS "Found libsndfile install!")
endif()