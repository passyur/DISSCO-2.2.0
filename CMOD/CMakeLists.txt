cmake_minimum_required(VERSION 3.25)

project(CMOD)
message(STATUS "== CMOD ==")

find_package(XercesC REQUIRED)
find_package(SndFile REQUIRED)

file(GLOB_RECURSE CMOD_SRC
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.h"
)
# list(REMOVE_ITEM CMOD_SRC "${PROJECT_SOURCE_DIR}/src/Main.cpp")

# add_library(LCMOD OBJECT ${CMOD_SRC})

# if(EXISTS "${PROJECT_SOURCE_DIR}/src/Main.cpp")
#     list(INSERT CMOD_SRC 0 "${PROJECT_SOURCE_DIR}/src/Main.cpp")
# endif()

add_executable(CMOD ${CMOD_SRC})
# add_executable(CMOD "${PROJECT_SOURCE_DIR}/src/Main.cpp")
# target_link_libraries(CMOD PUBLIC LCMOD)

### libsndfile must be linked in just the same way as it is in LASS ###
target_link_libraries(CMOD PUBLIC ${SNDFILE_LIBRARY})
target_link_libraries(CMOD PRIVATE ${XercesC_LIBRARY})
# add_subdirectory(${CMAKE_SOURCE_DIR}/external-libs/muParser)
target_link_libraries(CMOD PRIVATE MUPARSER)
target_link_libraries(CMOD PUBLIC LASS)
message(STATUS "libsndfile, libxerces-c, muParser, and LASS includes linked with target!")

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

# set_property(TARGET objlib PROPERTY POSITION_INDEPENDENT_CODE 1)
