CONFIG -= qt

win32{
    INCLUDEPATH *= $${_PRO_FILE_PWD_}/../freeglut/include
    DESTDIR = ./
}

TARGET = GLUTViewer
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH *= $${_PRO_FILE_PWD_}/../

SOURCES += GlutViewer.cc \
    GlutExaminer.cc \
    MeshViewer.cc

HEADERS += gl.hh \
    GlutViewer.hh \
    GlutExaminer.hh \
    MeshViewer.hh
