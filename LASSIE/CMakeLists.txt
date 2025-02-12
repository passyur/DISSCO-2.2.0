cmake_minimum_required(VERSION 3.25)

project(LASSIE)
message(STATUS "== LASSIE ==")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
# qt_standard_project_setup()
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# find_package(XercesC REQUIRED)
# find_package(SndFile REQUIRED)
# qt_standard_project_setup()

file(GLOB_RECURSE LASSIE_SRC
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.hpp"
    "${PROJECT_SOURCE_DIR}/src/*.ui"
)
# set(PROJECT_SOURCES
#         src/main.cpp
#         src/mainwindow.cpp
#         src/mainwindow.hpp

# )
# if(${QT_VERSION} VERSION_LESS 6.1.0)
# set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.lassie_qt)
# endif()

qt_add_executable(LASSIE
    MANUAL_FINALIZATION
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.hpp
    src/mainwindow.ui
)

target_link_libraries(LASSIE PRIVATE Qt6::Widgets)
# target_link_libraries(LASSIE PRIVATE ${SNDFILE_LIBRARY})
# target_link_libraries(LASSIE PRIVATE ${XercesC_LIBRARY})
# target_link_libraries(LASSIE PRIVATE LASS)

set_target_properties(LASSIE PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

include(GNUInstallDirs)
install(TARGETS LASSIE
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LASSIE)
endif()