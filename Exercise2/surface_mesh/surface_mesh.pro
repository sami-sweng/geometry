#-------------------------------------------------
#
# Project created by QtCreator 2015-01-23T12:53:46
#
#-------------------------------------------------

CONFIG -= qt

win32{
    DESTDIR = ./
}

TARGET = surface_mesh
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH *= $${_PRO_FILE_PWD_}/../


SOURCES += Surface_mesh.cpp \
    IO.cpp \
    IO_obj.cpp \
    IO_off.cpp \
    IO_poly.cpp \
    IO_stl.cpp

HEADERS += Vector.h \
    types.h \
    properties.h \
    Surface_mesh.h \
    IO.h
