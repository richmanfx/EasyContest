######################################################################
# Generated by Zoer, 2017.11.26
######################################################################

# Используемые Qt модули
QT += widgets gui core network websockets

# На выходе приложение, не библиотека
TEMPLATE = app

# Имя выходного файла
TARGET = EasyContest

# Выходная директория
unix {
  CONFIG(release, debug|release): DESTDIR = bin/release    # release
  CONFIG(debug  , debug|release): DESTDIR = bin/debug      # debug
}

# Директории для временных файлов
MOC_DIR     = tmp
OBJECTS_DIR = tmp
RCC_DIR     = tmp
UI_DIR      = tmp

# Ворнинги при использовании устаревших Qt функций
DEFINES += QT_DEPRECATED_WARNINGS

# Разрешить синтаксис С++11
CONFIG += c++11

INCLUDEPATH += .

# Input
HEADERS += mainwindow.h ui_mainwindow.h
FORMS += mainwindow.ui

SOURCES += \
  main.cpp \
  mainwindow.cpp

# Подключить файлы ресурсов
RESOURCES += resource/resource.qrc