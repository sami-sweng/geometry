#Generate binary executables at the root of the build folder
DESTDIR = $${OUT_PWD}/../

win32{
    LIBS += "$${OUT_PWD}/../MarchingCubes/MarchingCubes.lib"
}

unix{
    LIBS += -L$${OUT_PWD}/../MarchingCubes -lMarchingCubes
}
