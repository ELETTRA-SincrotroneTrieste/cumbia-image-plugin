
# The application will be installed under INSTALL_ROOT (i.e. prefix)
#
# WARNING: INSTALL_ROOT is defined by qumbia-plugins.pri, qumbia-epics-controls.pri and qumbia-tango-controls.pri
# The first definition in qumbia-plugins.pri is used.
#
#
# To set the prefix at build time, call
# qmake   "INSTALL_ROOT=/my/custom/path"
#
isEmpty(INSTALL_ROOT) {
    INSTALL_ROOT = /usr/local/cumbia-libs
}

isEmpty(prefix) {
    prefix=$${INSTALL_ROOT}
}

exists($${INSTALL_ROOT}/include/quapps/quapps.pri) {
    include($${INSTALL_ROOT}/include/quapps/quapps.pri)
} else {
    error("quapps.pri not found under $${INSTALL_ROOT}/include/quapps/");
}

QT += qml

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
# + ----------------------------------------------------------------- +
#

INC_DIR = $${QUMBIA_PLUGINS_INCLUDES}
SHAREDIR = $${QUMBIA_PLUGINS_SHARE}
PLUGIN_LIB_DIR = $${QUMBIA_PLUGINS_LIBDIR}

DESTDIR = plugins


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES -= QT_NO_DEBUG_OUTPUT

DOC_DIR = $${QUMBIA_PLUGINS_DOCDIR}
doc.commands = \
    doxygen \
    Doxyfile;
doc.files = doc/*
doc.path = $${DOC_DIR}

QMAKE_EXTRA_TARGETS += doc

# lib

target.path = $${PLUGIN_LIB_DIR}/designer
inc.path = $${INC_DIR}

# installation

INSTALLS += target \
    inc \
    doc

DISTFILES += \
    $$PWD/qumbia-plugin-designer.md


DESIGNER_PLUGIN_INSTALL_DIR=$${PLUGIN_LIB_DIR}/designer

DEFINES += CUMBIA_QTCONTROLS_PLUGIN_DIR=\"\\\"$${prefix}/lib/qumbia-plugins\\\"\"

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets x11extras printsupport uiplugin
    QT += designer
} else {
    CONFIG += designer
}

CONFIG += plugin
TEMPLATE = lib

TARGET = designer/cumbia-image-plugin-designer
CONFIG	+= qt thread warn_on debug

CONFIG +=

lessThan(QT_MAJOR_VERSION, 5) {
	CONFIG += designer
}

FORMS += \
    forms/editsourcetargetdialog.ui \
    forms/editsourcedialog.ui \
    forms/editsourcewidget.ui \

SOURCES	+= \
    cumbia-image-plugin-designer.cpp \
   # forms/cumbia-image-plugin-designer.cpp \
    pointeditor.cpp \
    forms/editsourcewidget.cpp

HEADERS	+= \
    cumbia-image-plugin-designer.h \
   # forms/editsourcedialog.h \
    pointeditor.h \
    forms/editsourcewidget.h

RESOURCES += cumbia-image-plugin-designer.qrc


unix:LIBS += -L.. 

# the same goes for INCLUDEPATH
INCLUDEPATH -= $${INC_DIR}
INCLUDEPATH += .. . ./forms


# qumbia-plugins.pri defines default INSTALLS for target inc and doc
# doc commands, target.path and inc.path are defined there as well.
