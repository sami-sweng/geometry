TEMPLATE = subdirs

SUBDIRS += \
    surface_mesh \
    GLUTViewer \
    01-ValenceViewer

GLUTViewer.depends = surface_mesh
01-ValenceViewer.depends = GLUTViewer
