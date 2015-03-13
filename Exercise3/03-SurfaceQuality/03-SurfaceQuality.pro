TARGET = SurfaceQuality
TEMPLATE = app

CONFIG -= qt
CONFIG -= app_bundle
CONFIG += console


SOURCES += QualityViewer.cc \
    SurfaceQuality.cc

HEADERS += QualityViewer.hh

include(../LibsInclude.pri)
