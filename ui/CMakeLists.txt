cmake_minimum_required(VERSION 3.22)
set(CMAKE_PREFIX_PATH "/home/dario/Qt/6.2.4/gcc_64/lib/cmake")
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

find_package(RtMidi REQUIRED)

add_library(ventana_principal OBJECT
        ventana_principal.cpp
        ventana_principal.ui
        )

target_link_libraries(ventana_principal
        Qt::Core
        Qt::Gui
        Qt::Widgets
        RtMidi::rtmidi
        )