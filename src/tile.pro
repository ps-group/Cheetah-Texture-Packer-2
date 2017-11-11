#-------------------------------------------------
#
# Project created by QtCreator 2012-04-29T22:23:49
#
#-------------------------------------------------

QT += core gui

TARGET = CheetahTexturePacker

CONFIG += c++14

QT_VERSION=$$[QT_VERSION]

contains(QT_VERSION, "^5.*") {
  QT += widgets
} else {
}

#####################################
# Win32 deployment
if(win32):CONFIG(release, debug|release) {
    DEPLOY_QTDIR = $$(QTDIR)
    DEPLOY_BIN_DIR = $$OUT_PWD
    win32-msvc*:DEPLOY_BIN_DIR=$$DEPLOY_BIN_DIR/release
    DEPLOY_SCRIPTS_DIR = $$clean_path($$PWD/../scripts)
    DEPLOY_OUT_DIR = $$clean_path($$PWD/../CheetahTexturePacker)

    build_package.output = $$shell_path($$DEPLOY_OUT_DIR/$$TARGET)
    build_package.commands = \
        \"$$shell_path($$DEPLOY_SCRIPTS_DIR/deploy.bat)\" \
        $$TARGET \
        \"$$shell_path($$DEPLOY_BIN_DIR)\" \
        \"$$shell_path($$DEPLOY_QTDIR)\" \
        \"$$shell_path($$DEPLOY_OUT_DIR)\"
    QMAKE_EXTRA_TARGETS+=build_package
}

#####################################

TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    view.cpp \
    imagepacker.cpp \
    imagecrop.cpp \
    imagesort.cpp \
    maxrects.cpp \
    commandlinehandler.cpp \
    atlasmetadatawriter.cpp \
    utils.cpp \
    cocosmetadatawriter.cpp \
    cssmetadatawriter.cpp

HEADERS  += mainwindow.h \
    view.h \
    imagepacker.h \
    maxrects.h \
    commandlinehandler.h \
    imetadatawriter.h \
    atlasmetadatawriter.h \
    utils.h \
    cocosmetadatawriter.h \
    cssmetadatawriter.h

FORMS    += mainwindow.ui

TRANSLATIONS += tile_ru.ts
