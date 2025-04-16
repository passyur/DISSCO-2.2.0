cmake_minimum_required(VERSION 3.16)
project(LASSIE VERSION 2.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    Charts
    Multimedia
    Network
    REQUIRED
)

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/EnvelopeLibraryEntry.cpp
    src/EnvelopeLibraryWindow.cpp
    src/EnvLibDrawingArea.cpp
)

# Header files
set(HEADERS
    src/mainwindow.hpp
    src/EnvelopeLibraryEntry.h
    src/EnvelopeLibraryWindow.h
    src/EnvLibDrawingArea.h
)

# UI files
set(UI_FILES
    src/mainwindow.ui
    src/EnvelopeLibraryWindow.ui
)

# Resource files
set(RESOURCE_FILES
    resources/resources.qrc
)

# Create executable
add_executable(LASSIE
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${RESOURCE_FILES}
)

# Link Qt libraries
target_link_libraries(LASSIE PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Charts
    Qt6::Multimedia
    Qt6::Network
)

# Include directories
target_include_directories(LASSIE PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Install rules
install(TARGETS LASSIE
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install resources
install(FILES ${RESOURCE_FILES}
    DESTINATION ${CMAKE_INSTALL_DATADIR}/LASSIE
)

# Set output directories
set_target_properties(LASSIE PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)