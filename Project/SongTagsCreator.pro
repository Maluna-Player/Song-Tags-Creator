QT += core gui widgets

CONFIG += c++14
QMAKE_CXXFLAGS += -std=c++14

TARGET = SongTagsCreator
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    MainWindow.cpp \
    SongFile.cpp \
    ChoiceDialog.cpp

HEADERS += \
    MainWindow.h \
    SongFile.h \
    ChoiceDialog.h
