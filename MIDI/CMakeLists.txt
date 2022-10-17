cmake_minimum_required(VERSION 3.22)
add_library(comunicacion OBJECT
        comunicacion.cpp
        )
add_library(mensaje_midi OBJECT
        mensaje_midi.cpp
        )
set(CMAKE_PREFIX_PATH "/home/dario/Qt/6.2.4/gcc_64/lib/cmake")
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

find_package(RtMidi REQUIRED)

target_link_libraries(mensaje_midi
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )

target_link_libraries(comunicacion
        Qt::Core
        Qt::Gui
        Qt::Widgets
        RtMidi::rtmidi
        )