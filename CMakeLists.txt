cmake_minimum_required(VERSION 3.25)

string(ASCII 27 Esc)
set(TextReset "${Esc}[m")
set(TextBold "${Esc}[1m")

### VERY IMPORTANT. USE OUT OF SOURCE BUILDS ONLY! ###
if (${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR}) 
    message(FATAL_ERROR "In-source builds are prohibited.\n"
            "Create a subdirectory 'build/' and use 'cmake ..' inside it.\n"
            "${TextBold}Delete 'CMakeCache.txt' and 'CMakeFiles/' before you continue.${TextReset}")
    
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")

# will define constants related to the version info
include(DISSCOVersions.txt)

project(DISSCO VERSION ${DISSCO_VERSION})

# Qt setup for LASSIE
message(STATUS "Looking for Qt6...")
set(Qt6_DIR "/opt/homebrew/opt/qt/lib/cmake/Qt6" CACHE PATH "Path to Qt6 CMake config")
message(STATUS "Qt6_DIR is set to: ${Qt6_DIR}")

find_package(Qt6 COMPONENTS Core REQUIRED)
message(STATUS "Found Qt6 Core version: ${Qt6Core_VERSION}")

find_package(Qt6 COMPONENTS Gui REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS Charts REQUIRED)
find_package(Qt6 COMPONENTS Xml REQUIRED)
find_package(XercesC REQUIRED)

message(STATUS "Qt6 found at: ${Qt6_DIR}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

option(BUILD_LASS_EXAMPLES "Build LASS examples" OFF)

# see Cross Compiling with CMake -- Mastering CMake for more on the eponymous subject
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/toolchains/windows.cmake")
elseif(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-deprecated")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/toolchains/macos.cmake")
elseif(LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-deprecated")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/toolchains/linux.cmake")
endif()

message(STATUS "== LASSIE ==")

# LASSIE source files
file(GLOB_RECURSE LASSIE_SOURCES
    "${CMAKE_SOURCE_DIR}/LASSIE/src/*.cpp"
)

file(GLOB_RECURSE LASSIE_HEADERS
    "${CMAKE_SOURCE_DIR}/LASSIE/src/*.hpp"
    "${CMAKE_SOURCE_DIR}/LASSIE/src/*.h"
)

file(GLOB_RECURSE LASSIE_UI_FILES
    "${CMAKE_SOURCE_DIR}/LASSIE/src/*.ui"
)

file(GLOB_RECURSE LASSIE_RESOURCE_FILES
    "${CMAKE_SOURCE_DIR}/LASSIE/resources/*.qrc"
)

message(STATUS "LASSIE sources: ${LASSIE_SOURCES}")
message(STATUS "LASSIE headers: ${LASSIE_HEADERS}")
message(STATUS "LASSIE UI files: ${LASSIE_UI_FILES}")
message(STATUS "LASSIE resource files: ${LASSIE_RESOURCE_FILES}")

# Create LASSIE executable
qt_add_executable(LASSIE
    MANUAL_FINALIZATION
    ${LASSIE_SOURCES}
    ${LASSIE_HEADERS}
    ${LASSIE_UI_FILES}
    ${LASSIE_RESOURCE_FILES}
)

# Link Qt libraries for LASSIE
target_link_libraries(LASSIE PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Charts
    Qt6::Xml
)
target_link_libraries(LASSIE PRIVATE ${XercesC_LIBRARY})
message(STATUS "libxerces-c linked with target!!")

# Set properties for Windows and macOS
set_target_properties(LASSIE PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

# Include directories for LASSIE
target_include_directories(LASSIE PRIVATE
    ${CMAKE_SOURCE_DIR}/LASSIE/src
)
target_include_directories(LASSIE PRIVATE
    ${CMAKE_SOURCE_DIR}/LASS/src
)
target_include_directories(LASSIE PRIVATE ${XercesC_INCLUDE_DIRS})

# Other components
add_subdirectory(${CMAKE_SOURCE_DIR}/LASS)
add_subdirectory(${CMAKE_SOURCE_DIR}/external-libs)
add_subdirectory(${CMAKE_SOURCE_DIR}/CMOD)

# Install rules for LASSIE
include(GNUInstallDirs)
install(TARGETS LASSIE
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LASSIE)
endif()

if(NOT XercesC_FOUND)
    message(SEND_ERROR "Couldn't find libxerces-c, won't go well; check BUILDING docs")
else()
    message(STATUS "Found libxerces-c install!")
endif()

message(STATUS "DONE!")