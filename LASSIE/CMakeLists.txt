cmake_minimum_required(VERSION 3.25)

project(LASSIE)
message(STATUS "== LASSIE ==")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Xml)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# qt_standard_project_setup()

### This will need to be updated as we add more windows/classes
qt_add_executable(LASSIE
    src/main.cpp
    src/windows/LASSIE.h
    src/inst.cpp src/inst.hpp
    src/windows/mainwindow.cpp src/windows/mainwindow.hpp src/UI/mainwindow.ui
    src/windows/MarkovWindow.cpp src/windows/MarkovWindow.hpp src/UI/MarkovWindow.ui
    src/windows/EnvelopeLibraryWindow.cpp src/windows/EnvelopeLibraryWindow.hpp
    src/windows/EnvLibDrawingArea.cpp src/windows/EnvLibDrawingArea.h
    src/windows/EnvelopeCommand.cpp src/windows/EnvelopeCommand.hpp
    src/windows/EnvelopeLibraryEntry.cpp src/windows/EnvelopeLibraryEntry.h
    src/windows/projectview.cpp src/windows/projectview.hpp
    src/windows/ProjectPropertiesDialog.cpp src/windows/ProjectPropertiesDialog.hpp src/UI/ProjectPropertiesDialog.ui
    src/windows/FunctionGenerator.cpp src/windows/FunctionGenerator.hpp src/UI/FunctionGenerator.ui
    src/core/project_struct.cpp src/core/project_struct.hpp
)

target_link_libraries(LASSIE PRIVATE 
    Qt6::Widgets
    Qt6::Core
    Qt6::Xml
)

target_link_libraries(LASSIE PRIVATE CMOD)

include(GNUInstallDirs)
target_include_directories(LASSIE PRIVATE
    ${XercesC_INCLUDE_DIR}
)

set_target_properties(LASSIE PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

install(TARGETS LASSIE
    BUNDLE  DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LASSIE)
endif()

message(STATUS "DONE!")