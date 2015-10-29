#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>
#include "compositor.h"
#include "windowlistmodel.h"
#include "declarativedirection.h"

#define URL "Compositor"
#define MAJOR 1
#define MINOR 0

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  QQuickView view;
  Compositor comp(&view);

  QObject::connect(&view, SIGNAL(afterRendering()), &comp, SLOT(sendCallbacks()));

  qmlRegisterUncreatableType<WindowListModel>(URL, MAJOR, MINOR, "WindowListModel", "Cannot be created");
  qmlRegisterUncreatableType<DeclarativeDirection>(URL, MAJOR, MINOR, "Direction", "Cannot be created");

  view.rootContext()->setContextProperty("compositor", &comp);
  view.setSource(QUrl("/usr/share/noname/qml/main.qml"));
  view.setResizeMode(QQuickView::SizeRootObjectToView);

  QObject::connect(&comp, SIGNAL(windowAdded(QVariant)), view.rootObject(), SLOT(windowAdded(QVariant)));
  QObject::connect(&comp, SIGNAL(windowRemoved(QVariant)), view.rootObject(), SLOT(windowRemoved(QVariant)));

  view.show();

  return app.exec();
}
