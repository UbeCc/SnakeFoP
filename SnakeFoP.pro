QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += sdk_no_version_check

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    # for cli
    configmanager.cpp \
    mapmanager.cpp \
    game.cpp \
    # for gui
    main.cpp \
    playpage.cpp \
    recordmanager.cpp \
    resultpage.cpp \
    settingpage.cpp \
    status.cpp \
    widget.cpp

HEADERS += \
    # for cli
    configmanager.h \
    mapmanager.h \
    game.h \
    # for gui
    playpage.h \
    recordmanager.h \
    resultpage.h \
    settingpage.h \
    status.h \
    widget.h

FORMS += \
    playpage.ui \
    resultpage.ui \
    settingpage.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    config/* \
    maps/* \

copyconfig.commands = $(COPY_DIR) $$PWD/config $$OUT_PWD
copymaps.commands = $(COPY_DIR) $$PWD/maps $$OUT_PWD
first.depends = $(first) copymaps copyconfig
export(first.depends)
export(copyconfig.commands)
export(copymaps.commands)
QMAKE_EXTRA_TARGETS += first copyconfig copymaps
