QT += core gui widgets

CONFIG += c++14
QMAKE_CXXFLAGS += -std=c++14

TARGET = SongTagsCreator
TEMPLATE = app

SOURCES += main.cpp \
    MainWindow.cpp \
    SongFile.cpp \
    ChoiceDialog.cpp \
    TagsManager.cpp

HEADERS += \
    MainWindow.h \
    SongFile.h \
    ChoiceDialog.h \
    TagsManager.h
