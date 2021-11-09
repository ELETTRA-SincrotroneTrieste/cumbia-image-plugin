isEmpty(CUMBIA_ROOT) {
    CUMBIA_ROOT=/usr/local/cumbia-libs
}
include($${CUMBIA_ROOT}/include/quapps/quapps.pri)

# CONFIG += debug | release

DEFINES -=  QT_NO_DEBUG_OUTPUT


SOURCES += src/main.cpp \
    src/cudisplay.cpp

HEADERS += \
    src/cudisplay.h

# cuuimake runs uic
# FORMS    = src/cudisplay.ui
# but we need to include ui_xxxx.h file amongst the headers
# in order to be recompiled when it changes
#
#HEADERS += \
#    ui/ui_cudisplay.h

# - ui: where to find cuuimake ui_*.h files
#   since FORMS is not used
# - src: where to find headers included by
#   ui_*.h (e.g. for custom widget promoted
#   from the Qt designer)
#
INCLUDEPATH += ui src

TARGET = cudisplay

!wasm-emscripten {
    TARGET   = bin/$${TARGET}
} else {
    TARGET = wasm/$${TARGET}
}



# unix:LIBS += -L. -lmylib

# unix:INCLUDEPATH +=  . ../../src

message("-")
message("NOTE")
message("You need to run cuuimake in order to build the project")
message("-")
message("        cuuimake --show-config to see cuuimake configuration options")
message("        cuuimake --configure to configure cuuimake")
message("        cuuimake -jN to execute cuuimake and then make -jN")
message("        cuuimake --make to run cuuimake and then make")
message("-")

RESOURCES += \
    src/cudisplay.qrc

#
# make install works if INSTALL_PREFIX is given to qmake
#
!isEmpty(INSTALL_PREFIX) {
    wasm-emscripten {
	    inst.files = wasm/*
		} else {
		inst.files = $${TARGET}
		}
	inst.path = $${INSTALL_PREFIX}/bin
	INSTALLS += inst
	message("-")
	message("INSTALLATION")
	message("       execute `make install` to install 'cudisplay' under $${INSTALL_PREFIX}/bin ")
	message("-")
} else {
    message(" ")
	message(" ----------------------------------------  INSTALLATION ---------------------------------------- ")
	message(" call qmake INSTALL_PREFIX=$${CUMBIA_ROOT} to install cudisplay later with `make install` ")
	message(" ----------------------------------------  ------------ ---------------------------------------- ")
	message(" ")
	message(" ")
}
