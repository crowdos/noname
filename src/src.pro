TARGET = noname

QT += quick qml KWaylandServer gui_private
CONFIG += c++11

SOURCES += main.cpp \
           compositor.cpp \
           surfacecontainer.cpp \
           windowlistmodel.cpp \
           surfacecontainerview.cpp \
           fullscreensurfaceview.cpp \
           backgroundimage.cpp \
           screenblank.cpp \
           powerkey.cpp
#           compositorwindow.cpp

HEADERS += compositor.h \
           surfacecontainer.h \
           windowlistmodel.h \
           declarativedirection.h \
           surfacecontainerview.h \
           fullscreensurfaceview.h \
           backgroundimage.h \
           screenblank.h \
           powerkey.h
#           compositorwindow.h \

LIBS += -lhwhal

target.path = /usr/bin/

qml.files = main.qml \
            EdgeHandler.qml \
            WindowList.qml
qml.path = /usr/share/noname/qml/

edge.files = plugins/edge/Close.qml \
             plugins/edge/Lock.qml \
             plugins/edge/Minimize.qml

edge.path = /usr/share/noname/qml/plugins/edge/

dconf.files = ../data/noname.txt
dconf.path = /etc/dconf/db/crowdos.d/

INSTALLS += target qml edge dconf
