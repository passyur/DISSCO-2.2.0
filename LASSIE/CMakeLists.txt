cmake_minimum_required(VERSION 3.25)

project(LASSIE)
message(STATUS "== LASSIE ==")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# qt_standard_project_setup()

### This will need to be updated as we add more windows/classes
qt_add_executable(LASSIE
    src/main.cpp
    src/mainwindow.cpp src/mainwindow.hpp src/mainwindow.ui
    src/markovwindow.cpp src/markovwindow.hpp
    src/envelopewindow.cpp src/envelopewindow.hpp
    src/inst.hpp
    src/projectview.cpp src/projectview.hpp
    src/core/project_struct.cpp src/core/project_struct.hpp
)

target_link_libraries(LASSIE PRIVATE 
    Qt6::Widgets
    Qt6::Core
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