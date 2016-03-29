#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QCommandLineParser>

#include "compositor.h"
#include "windowlistmodel.h"
#include "declarativedirection.h"
#include "surfacecontainer.h"
#include "surfacecontainerview.h"
#include "fullscreensurfaceview.h"
#include "backgroundimage.h"
#include "screenblank.h"

#include <KWayland/Server/display.h>
#include <KWayland/Server/seat_interface.h>
#include <KWayland/Server/datadevicemanager_interface.h>
#include <KWayland/Server/compositor_interface.h>
#include <KWayland/Server/output_interface.h>
#include <KWayland/Server/shell_interface.h>

#include <QDebug>

#define URL "Compositor"
#define MAJOR 1
#define MINOR 0

using namespace KWayland::Server;

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);
  app.setApplicationName("NoName");

  QCommandLineParser parser;

  parser.setApplicationDescription("NoNme Wayland compositor");
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption qml("qml", QGuiApplication::translate("main", "QML file to load"),
			 QCoreApplication::translate("main", "qml file"));
  parser.addOption(qml);

  QCommandLineOption size(QStringList() << "s" << "size",
			  QCoreApplication::translate("main", "Window size"),
			  QCoreApplication::translate("main", "size"));
  parser.addOption(size);

  parser.process(app);

  QSize windowSize(400, 600);

  if (parser.isSet(size)) {
    QString s = parser.value(size);
    QStringList parts(s.split('x'));
    if (parts.size() != 2) {
      qFatal("Invalid size %s", qPrintable(s));
    }

    int width = parts[0].toInt();
    int height = parts[1].toInt();

    if (width <= 0 || height <= 0) {
      qFatal("Invalid size %s", qPrintable(s));
    }

    windowSize = QSize(width, height);
  }

  Display display;
  display.start();

  DataDeviceManagerInterface *ddm = display.createDataDeviceManager(&display);
  ddm->create();

  CompositorInterface *compositor = display.createCompositor(&display);
  compositor->create();

  ShellInterface *shell = display.createShell(&display);
  shell->create();

  display.createShm();

  OutputInterface *output = display.createOutput(&display);
  output->setGlobalPosition(QPoint(0, 0));
  output->setPhysicalSize(QSize(269, 202)); // TODO:
  output->addMode(windowSize);
  output->create();

  Compositor comp(display, windowSize);

  QObject::connect(shell, &ShellInterface::surfaceCreated,
		   &comp, &Compositor::surfaceCreated);

  QQuickView view;
  view.rootContext()->setContextProperty("compositor", &comp);

  QUrl qmlUrl("/usr/share/noname/qml/main.qml");
  if (parser.isSet(qml)) {
    qmlUrl = parser.value(qml);
  }

  qmlRegisterUncreatableType<Compositor>(URL, MAJOR, MINOR, "Compositor", "Cannot be created");
  qmlRegisterUncreatableType<WindowListModel>(URL, MAJOR, MINOR, "WindowListModel", "Cannot be created");
  qmlRegisterUncreatableType<DeclarativeDirection>(URL, MAJOR, MINOR, "Direction", "Cannot be created");
  qmlRegisterUncreatableType<SurfaceContainer>(URL, MAJOR, MINOR, "SurfaceContainer", "Cannot be created");
  qmlRegisterType<SurfaceContainerView>(URL, MAJOR, MINOR, "SurfaceContainerView");
  qmlRegisterType<FullScreenSurfaceView>(URL, MAJOR, MINOR, "FullScreenSurfaceView");
  qmlRegisterType<BackgroundImage>(URL, MAJOR, MINOR, "BackgroundImage");
  qmlRegisterType<ScreenBlank>(URL, MAJOR, MINOR, "ScreenBlank");

  view.setSource(qmlUrl);
  view.setResizeMode(QQuickView::SizeRootObjectToView);

  QObject::connect(&comp, SIGNAL(windowAdded(QVariant)),
		   view.rootObject(), SLOT(windowAdded(QVariant)));
  QObject::connect(&comp, SIGNAL(windowRemoved(QVariant)),
		   view.rootObject(), SLOT(windowRemoved(QVariant)));

  view.setMinimumSize(windowSize);
  view.setMaximumSize(windowSize);

  view.show(); // TODO full screen

  return app.exec();
}
