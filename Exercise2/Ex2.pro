TEMPLATE = subdirs

SUBDIRS += \
    surface_mesh \
    GLUTViewer \
    MarchingCubes \
    02-Reconstruction

GLUTViewer.depends = surface_mesh
MarchingCubes.depends = surface_mesh
02-Reconstruction.depends = GLUTViewer

