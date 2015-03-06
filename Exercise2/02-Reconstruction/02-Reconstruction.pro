TARGET = Reconstruction
TEMPLATE = app

CONFIG -= qt
CONFIG -= app_bundle
CONFIG += console

SOURCES += ImplicitRBF.cc \
    PointCloudNormals.cc \
    ReconstructionViewer.cc \
    reconstruct.cc

HEADERS += ../KdTree/KdTree.h \
    ImplicitRBF.hh \
    PointCloudNormals.hh \
    ImplicitHoppe.hh \
    ReconstructionViewer.hh \
    ../eigen/eigen_include.h

include(../LibsInclude.pri)
include(../MarchingCubes/LibMarchingCubes.pri)
