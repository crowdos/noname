#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>

#include "compositor.h"
#include "windowlistmodel.h"
#include "declarativedirection.h"
#include "surfacecontainer.h"
#include "surfacecontainerview.h"

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

const QSize windowSize(400, 600); // TODO:

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

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
  output->setPhysicalSize(QSize(269, 202)); // TODO:
  output->addMode(windowSize);
  output->create();

  Compositor comp(display, windowSize);

  QObject::connect(shell, &ShellInterface::surfaceCreated,
		   &comp, &Compositor::surfaceCreated);

  QQuickView view;
  view.rootContext()->setContextProperty("compositor", &comp);

  QUrl qmlUrl("/usr/share/noname/qml/main.qml");
  if (int index = app.arguments().indexOf("-qml") != -1) {
    if (index + 1 < app.arguments().size()) {
      qmlUrl = QUrl(app.arguments()[index + 1]);
    }
  }

  qmlRegisterUncreatableType<WindowListModel>(URL, MAJOR, MINOR, "WindowListModel", "Cannot be created");
  qmlRegisterUncreatableType<DeclarativeDirection>(URL, MAJOR, MINOR, "Direction", "Cannot be created");
  qmlRegisterUncreatableType<SurfaceContainer>(URL, MAJOR, MINOR, "SurfaceContainer", "Cannot be created");
  qmlRegisterType<SurfaceContainerView>(URL, MAJOR, MINOR, "SurfaceContainerView");

  view.setSource(qmlUrl);
  view.setResizeMode(QQuickView::SizeRootObjectToView);
  view.resize(windowSize);

  QObject::connect(&comp, SIGNAL(windowAdded(QVariant)),
		   view.rootObject(), SLOT(windowAdded(QVariant)));
  QObject::connect(&comp, SIGNAL(windowRemoved(QVariant)),
		   view.rootObject(), SLOT(windowRemoved(QVariant)));

  view.show(); // TODO full screen

  return app.exec();
}
