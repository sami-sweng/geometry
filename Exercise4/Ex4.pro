TEMPLATE = subdirs

SUBDIRS += \
    surface_mesh \
    GLUTViewer \
    04-Smoothing

GLUTViewer.depends = surface_mesh
04-Smoothing.depends = GLUTViewer
