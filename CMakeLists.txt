cmake_minimum_required(VERSION 3.23)
project(control_midi)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/home/dario/Qt/6.2.4/gcc_64/")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
 find_package(nlohmann_json REQUIRED PATHS lib/json-3.11.2)
add_executable(control_midi main.cpp)
target_link_libraries(control_midi
        Qt::Core
        Qt::Gui
        Qt::Widgets
        nlohmann_json::nlohmann_json
        )