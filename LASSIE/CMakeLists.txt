cmake_minimum_required(VERSION 3.16)
project(LASSIE VERSION 2.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 packages
find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    Charts
    Multimedia
    Network
    Xml
    REQUIRED
)

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/EnvelopeLibraryEntry.cpp
    src/EnvelopeLibraryWindow.cpp
    src/EnvLibDrawingArea.cpp
    src/ProjectViewController.cpp
    src/MarkovModelLibraryWindow.cpp
)

# Header files
set(HEADERS
    src/mainwindow.hpp
    src/EnvelopeLibraryEntry.hpp
    src/EnvelopeLibraryWindow.hpp
    src/EnvLibDrawingArea.hpp
    src/ProjectViewController.hpp
    src/MarkovModelLibraryWindow.hpp
)

# UI files
set(UI_FILES
    src/mainwindow.ui
    src/EnvelopeLibraryWindow.ui
    src/MarkovModelLibraryWindow.ui
    src/ProjectViewController.ui
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
    Qt6::Xml
)

# Include directories
target_include_directories(LASSIE PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Gui_INCLUDE_DIRS}
    ${Qt6Widgets_INCLUDE_DIRS}
    ${Qt6Charts_INCLUDE_DIRS}
    ${Qt6Multimedia_INCLUDE_DIRS}
    ${Qt6Network_INCLUDE_DIRS}
    ${Qt6Xml_INCLUDE_DIRS}
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