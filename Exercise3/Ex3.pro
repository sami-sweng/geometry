TEMPLATE = subdirs

SUBDIRS += \
    surface_mesh \
    GLUTViewer \
    03-SurfaceQuality

GLUTViewer.depends = surface_mesh
03-SurfaceQuality.depends = GLUTViewer
