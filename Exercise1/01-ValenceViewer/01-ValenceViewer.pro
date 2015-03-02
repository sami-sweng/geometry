TARGET = ValenceViewer
TEMPLATE = app

CONFIG -= qt
CONFIG -= app_bundle
CONFIG += console


SOURCES += ValenceViewer.cc \
    valenceview.cc

HEADERS += ValenceViewer.hh

include(../LibsInclude.pri)
