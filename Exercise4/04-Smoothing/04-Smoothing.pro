TARGET = Smoothing
TEMPLATE = app

CONFIG -= qt
CONFIG -= app_bundle
CONFIG += console


SOURCES += QualityViewer.cc \
    SmoothingViewer.cc \
    smoother.cc

HEADERS += QualityViewer.hh \
    SmoothingViewer.hh

include(../LibsInclude.pri)
