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
    src/windows/MainWindow.cpp src/windows/MainWindow.hpp src/ui/mainwindow.ui
    # src/windows/markovwindow.cpp src/windows/markovwindow.hpp
    src/windows/MarkovModelLibraryWindow.cpp src/windows/MarkovModelLibraryWindow.hpp
    src/windows/EnvelopeLibraryWindow.cpp src/windows/EnvelopeLibraryWindow.hpp 
    src/widgets/EnvLibDrawingArea.cpp src/widgets/EnvLibDrawingArea.hpp 
    src/core/EnvelopeLibraryEntry.cpp src/core/EnvelopeLibraryEntry.hpp
    src/dialogs/FunctionGenerator.cpp src/dialogs/FunctionGenerator.hpp src/UI/FunctionGenerator.ui
    src/widgets/REVPartialAlignment.cpp src/widgets/REVPartialAlignment.hpp src/UI/FunGenREVPartialAlignment.ui
    src/widgets/SPAPartialAlignment.cpp src/widgets/SPAPartialAlignment.hpp src/UI/FunGenSPAPartialAlignment.ui
    src/dialogs/ProjectPropertiesDialog.cpp src/dialogs/ProjectPropertiesDialog.hpp src/UI/ProjectPropertiesDialog.ui
    src/inst.hpp
    src/widgets/ProjectViewController.cpp src/widgets/ProjectViewController.hpp
    src/core/project_struct.cpp src/core/project_struct.hpp
    src/windows/ObjectWindow.cpp src/windows/ObjectWindow.hpp
    src/widgets/PaletteViewController.cpp src/widgets/PaletteViewController.hpp
    src/widgets/EventAttributesViewController.cpp src/widgets/EventAttributesViewController.hpp src/ui/Attributes.ui
    src/dialogs/FileNewObject.cpp src/dialogs/FileNewObject.hpp src/ui/FileNewObject.ui
    src/widgets/Modifiers.cpp src/widgets/Modifiers.hpp src/ui/Modifiers.ui
    # src/core/IEvent.cpp src/core/IEvent.h
)

target_link_libraries(LASSIE PRIVATE 
    Qt6::Widgets
    Qt6::Core
    Qt6::Xml
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