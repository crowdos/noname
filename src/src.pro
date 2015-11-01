TARGET = noname

QT += quick qml KWaylandServer
CONFIG += c++11

SOURCES += main.cpp \
           compositor.cpp \
           surfacecontainer.cpp \
           windowlistmodel.cpp \
           surfacecontainerview.cpp
#           compositorwindow.cpp

HEADERS += compositor.h \
           surfacecontainer.h \
           windowlistmodel.h \
           declarativedirection.h \
           surfacecontainerview.h
#           compositorwindow.h \


target.path = /usr/bin/

qml.files = main.qml \
            EdgeHandler.qml \
            WindowList.qml
qml.path = /usr/share/noname/qml/

edge.files = plugins/edge/Close.qml \
             plugins/edge/Lock.qml \
             plugins/edge/Minimize.qml

edge.path = /usr/share/noname/qml/plugins/edge/

INSTALLS += target qml edge
