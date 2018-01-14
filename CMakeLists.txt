cmake_minimum_required(VERSION 3.9)
project(EasyContest)

set(CMAKE_CXX_STANDARD 11)

set (SOURCES
        source/main.cpp
        source/mainwindow.cpp
        source/logging_categories.cpp
        source/customlineedit.cpp
        contestload.cpp
        main.cpp
        libs/TciClient/tciclient.cpp
        libs/TciClient/TciParser/TciCommand.cpp
        libs/TciClient/TciParser/TciParser.cpp
        libs/TciClient/TciTrxState/TciTrxState.cpp
)

set (HEADERS
        source/mainwindow.h
        source/logging_categories.h
        source/customlineedit.h
        contestload.h
        libs/TciClient/tciclient.h
        libs/TciClient/TciParser/TciCommand.h
        libs/TciClient/TciParser/TciParser.h
        libs/TciClient/TciParser/TciParserAbstract.h
        libs/TciClient/TciParser/TciParserCommandAbsract.h
        libs/TciClient/TciTrxState/TciTrxState.h
)

set (UIS
        source/mainwindow.ui
        contestload.ui
)

set (RESOURCES
        resource/resource.qrc
)

#add_subdirectory(libs/TciClient)           # Подлючает локальный CMakeLists.txt


# включаем поиск библиотек с использованием find_package()
# через REQUIRED указываем, что библиотеки обязательны
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5WebSockets REQUIRED)
find_package(Qt5X11Extras REQUIRED)

set(TciClient_DIR libs/TciClient)
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/libs/TciClient)
#find_package (TciClient REQUIRED)
#target_link_libraries(${PROJECT_NAME} TciClient)


set(SOURCE_FILES main.cpp)

add_executable(EasyContest
        libs/QCustomPlot/qcustomplot.cpp
        libs/QCustomPlot/qcustomplot.h
        libs/QGlobalShortcut/qglobalshortcut.cpp
        libs/QGlobalShortcut/qglobalshortcut.h
        libs/TciClient/TciParser/TciCommand.cpp
        libs/TciClient/TciParser/TciCommand.h
        libs/TciClient/TciParser/TciParser.cpp
        libs/TciClient/TciParser/TciParser.h
        libs/TciClient/TciParser/TciParserAbstract.h
        libs/TciClient/TciParser/TciParserCommandAbsract.h
        libs/TciClient/TciTrxState/TciTrxState.cpp
        libs/TciClient/TciTrxState/TciTrxState.h
        libs/TciClient/tciclient.cpp
        libs/TciClient/tciclient.h
        resource/EasyContest.desktop.example
        resource/ec.png
        resource/eng-flag.png
        resource/resource.qrc
        resource/rus-flag.png
        source/mainwindow.cpp
        source/mainwindow.h
        source/customlineedit.cpp
        source/customlineedit.h
        source/logging_categories.cpp
        source/logging_categories.h
        source/main.cpp
        source/contestload.cpp
        source/contestload.h
        source/contestload.ui
        tmp/moc_customlineedit.cpp
        tmp/moc_mainwindow.cpp
        tmp/moc_predefs.h
        tmp/moc_qcustomplot.cpp
        tmp/moc_qglobalshortcut.cpp
        tmp/moc_tciclient.cpp
        tmp/moc_TciCommand.cpp
        tmp/moc_TciParser.cpp
        tmp/moc_TciParserAbstract.cpp
        tmp/moc_TciParserCommandAbsract.cpp
        tmp/moc_TciTrxState.cpp
        tmp/qrc_resource.cpp
        tmp/ui_mainwindow.h
        translations/EasyContest_ru.qm
        translations/EasyContest_ru.ts
        EasyContest.pro)

# указываем, какие библиотеки необходимо подключить
target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)
target_link_libraries(${PROJECT_NAME} Qt5::Network)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::WebSockets)
target_link_libraries(${PROJECT_NAME} Qt5::OpenGL)
target_link_libraries(${PROJECT_NAME} Qt5::X11Extras)



set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
