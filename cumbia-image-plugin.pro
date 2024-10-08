# + ----------------------------------------------------------------- +
#
# Customization section:
#
# Customize the following paths according to your installation:
#
#
# Here qumbia-qtcontrols will be installed
# INSTALL_ROOT can be specified from the command line running qmake "INSTALL_ROOT=/my/install/path"
#

isEmpty(INSTALL_ROOT) {
    INSTALL_ROOT = /usr/local/cumbia-libs
}

isEmpty(prefix) {
    prefix = $${INSTALL_ROOT}
}

include ($${INSTALL_ROOT}/include/cumbia-qtcontrols/cumbia-qtcontrols.pri)

#
#
# Here qumbia-plugins include files will be installed
    QUMBIA_PLUGINS_INCLUDES=$${INSTALL_ROOT}/include/qumbia-plugins
#
#
# Here qumbia-plugins share files will be installed
#
    QUMBIA_PLUGINS_SHARE=$${INSTALL_ROOT}/share/qumbia-plugins
#
#
# Here qumbia-plugins libraries will be installed
    QUMBIA_PLUGINS_LIBDIR=$${INSTALL_ROOT}/lib/qumbia-plugins
#
#
# Here qumbia-plugins documentation will be installed
    QUMBIA_PLUGINS_DOCDIR=$${INSTALL_ROOT}/share/doc/qumbia-plugins
#
#
# # Plugin interface will be installed under cumbia-qtcontrols/include dirs
# CUMBIA_QTCONTROLS_INCLUDES is defined in cumbia-qtcontrols.pri, which is included above
#
# + ----------------------------------------------------------------- +
#


INC_DIR = $${QUMBIA_PLUGINS_INCLUDES}
SHAREDIR = $${QUMBIA_PLUGINS_SHARE}
PLUGIN_LIB_DIR = $${QUMBIA_PLUGINS_LIBDIR}

DESTDIR = plugins

VERSION_HEX = 0x010103
VERSION = 1.1.3
DEFINES += CUMBIA_IMAGE_VERSION_STR=\"\\\"$${VERSION}\\\"\" \
    CUMBIA_IMAGE_VERSION=$${VERSION_HEX}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES -= QT_NO_DEBUG_OUTPUT

unix:!android-g++ {
    DEFINES += CUMBIAQTCONTROLS_HAS_QWT=1
}

DOC_DIR = $${QUMBIA_PLUGINS_DOCDIR}
doc.commands = \
    doxygen \
    Doxyfile;
doc.files = doc/*
doc.path = $${DOC_DIR}

QMAKE_EXTRA_TARGETS += doc

# lib

target.path = $${PLUGIN_LIB_DIR}
inc.path = $${INC_DIR}

# plugin interface, installed under CUMBIA_QTCONTROLS_INCLUDES
# defined in cumbia-qtcontrols.pri, by default $${INSTALL_ROOT}/include/cumbia-qtcontrols
#
iface_inc.path = $${CUMBIA_QTCONTROLS_INCLUDES}
iface_inc.files = quimageplugininterface.h quimage.h

# installation

INSTALLS += target \
    iface_inc \
    inc \
    doc

DISTFILES += \
    $$PWD/qumbia-plugins.md \
    CHANGELOG


TARGET = cumbia-image-plugin
TEMPLATE = lib
CONFIG += plugin

isEmpty(buildtype) {
        buildtype = release
} else {
    equals(buildtype, debug) {
        message("")
        message("debug build")
        message("")
    }
}

CONFIG += $${buildtype}

SOURCES += \
    colortablemap.cpp \
	quimageGLwidget.cpp \
	quimagebase.cpp \
	quimagewidget.cpp \
	quimgconfdialog.cpp \
    quimageplugin.cpp \
    quimgmousetracker.cpp \
    quimgpainter.cpp \
    quimgscrollarea.cpp \
    quimgzoomer.cpp

HEADERS += \
    colortablemap.h \
	quimageGLwidget.h \
	quimagebase.h \
    quimagemouseeventif.h \
	quimagewidget.h \
	quimgconfdialog.h \
    quimageplugin.h \
    quimgmousetracker.h \
    quimgpainter.h \
    quimgscrollarea.h \
	quimgzoomer.h

DISTFILES +=  \
    cumbia-image.json \
    quimageplugininterface.h

inc.files += $${HEADERS}

RESOURCES += \
    confdialog.qrc

FORMS += \
    quimgconfdialog.ui

