cmake_minimum_required(VERSION 3.16)

project(TerraLink VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Network Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Network Gui)

set(TS_FILES TerraLink_es_ES.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TerraLink
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        enviarmissatge.h enviarmissatge.cpp enviarmissatge.ui
        missatgesrebuts.h missatgesrebuts.cpp missatgesrebuts.ui
        contactes.h contactes.cpp contactes.ui
        qrwhatsapp.h qrwhatsapp.cpp qrwhatsapp.ui
        configuracio.h configuracio.cpp configuracio.ui
        controladoraplicacio.h controladoraplicacio.cpp
        gestoremergencia.h gestoremergencia.cpp
        missatge.h missatge.cpp
        contacte.h contacte.cpp
        missatgewhatsapp.h missatgewhatsapp.cpp
        missatgesms.h missatgesms.cpp
        missatgelora.h missatgelora.cpp
        repositoricontactes.h repositoricontactes.cpp
        repositorimissatges.h repositorimissatges.cpp
        iserveimissatgeria.h iserveimissatgeria.cpp
        serveiwhatsapp.h serveiwhatsapp.cpp
        serveisms.h serveisms.cpp
        serveilora.h serveilora.cpp
        missatgefactory.h missatgefactory.cpp
        gestorcanals.h gestorcanals.cpp
        adaptadormodem4g.h adaptadormodem4g.cpp
        adaptadorlora.h adaptadorlora.cpp
        adaptadordispositiuaudio.h adaptadordispositiuaudio.cpp
        adaptadorimpressoraescpos.h adaptadorimpressoraescpos.cpp
        serveimonitoratge.h serveimonitoratge.cpp
        estatsistema.h estatsistema.cpp
        serveiaudio.h serveiaudio.cpp
        serveitranscripcio.h serveitranscripcio.cpp
        serveiimpressio.h serveiimpressio.cpp
        adaptadorwhatsapp.cpp
        adaptadorwhatsapp.h
        indicadorcanal.h indicadorcanal.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TerraLink APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(TerraLink SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TerraLink
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(TerraLink PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)
target_include_directories(TerraLink PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TerraLink)
endif()
set_target_properties(TerraLink PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TerraLink
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TerraLink)
endif()
