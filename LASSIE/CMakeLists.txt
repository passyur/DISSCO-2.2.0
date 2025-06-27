cmake_minimum_required(VERSION 3.25)

project(LASSIE)
message(STATUS "== LASSIE ==")

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Xml)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# qt_standard_project_setup()

### This will need to be updated as we add more windows/classes
qt_add_executable(LASSIE
    src/main.cpp
    src/windows/mainwindow.cpp src/windows/mainwindow.h src/UI/mainwindow.ui
    # src/windows/markovwindow.cpp src/windows/markovwindow.hpp
    src/windows/EnvelopeLibraryWindow.cpp src/windows/EnvelopeLibraryWindow.h
    src/windows/MarkovModelLibraryWindow.cpp src/windows/MarkovModelLibraryWindow.h
    src/windows/EnvLibDrawingArea.cpp src/windows/EnvLibDrawingArea.h 
    src/windows/EnvelopeLibraryEntry.cpp src/windows/EnvelopeLibraryEntry.h
    src/windows/FunctionGenerator.cpp src/windows/FunctionGenerator.h
    src/windows/ProjectPropertiesDialog.cpp src/windows/ProjectPropertiesDialog.h
    src/windows/ProjectViewController.cpp src/windows/ProjectViewController.h
    # src/windows/EventAttributesViewController.cpp src/windows/EventAttributesViewController.h
    src/inst.hpp
    src/core/envelope_struct.cpp src/core/envelope_struct.hpp
    src/core/project_struct.cpp src/core/project_struct.hpp
    # src/core/IEvent.cpp src/core/IEvent.h
    src/core/event_struct.hpp
    src/UI/FunctionGenerator.ui
    src/UI/ProjectPropertiesDialog.ui
)

target_link_libraries(LASSIE PRIVATE 
    Qt6::Widgets
    Qt6::Core
    Qt6::Xml
    LASS
    CMOD
)

include(GNUInstallDirs)
target_include_directories(LASSIE PRIVATE
    ${XercesC_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/LASS/src
    ${CMAKE_SOURCE_DIR}/CMOD/src
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