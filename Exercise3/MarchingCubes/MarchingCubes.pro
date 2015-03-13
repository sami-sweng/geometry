#-------------------------------------------------
#
# Project created by QtCreator 2015-01-23T12:53:46
#
#-------------------------------------------------

CONFIG -= qt

win32{
    DESTDIR = ./
}

TARGET = MarchingCubes
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH *= $${_PRO_FILE_PWD_}/../


SOURCES += Grid.cpp \
    Marching_cubes.cpp

HEADERS += Grid.h \
    Marching_cubes.h
