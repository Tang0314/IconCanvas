cmake_minimum_required(VERSION 3.16)
project(IconCanvas VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(IconCanvas
    main.cpp
    mainwindow.h
    mainwindow.cpp
    canvaswidget.h
    canvaswidget.cpp
    colorpalette.h
    colorpalette.cpp
    translator.h
)

target_link_libraries(IconCanvas PRIVATE Qt6::Widgets)
