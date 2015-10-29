TARGET = noname

QT += quick qml compositor

SOURCES += main.cpp \
           compositor.cpp \
           windowlistmodel.cpp \
           compositorwindow.cpp

HEADERS += compositor.h \
           windowlistmodel.h \
           compositorwindow.h \
           declarativedirection.h

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
