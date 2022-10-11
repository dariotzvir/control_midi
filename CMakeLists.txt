cmake_minimum_required(VERSION 3.22)
project(control_midi)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#set(CMAKE_PREFIX_PATH "/home/dario/Qt/6.2.4/gcc_64/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
set(Boost_INCLUDE_DIR "/usr/include")
set(Boost_LIBRARY_DIR "/usr/lib64")
find_package(Boost COMPONENTS json REQUIRED)
find_package(RtMidi REQUIRED)

add_executable(control_midi main.cpp
        ui/ventana_principal.cpp ui/ventana_principal.h ui/ventana_principal.ui
        MIDI/mensaje_midi.cpp MIDI/mensaje_midi.hpp)
target_link_libraries(control_midi
        Qt::Core
        Qt::Gui
        Qt::Widgets
        RtMidi::rtmidi
        Boost::json
        )