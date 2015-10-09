#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>
#include "compositor.h"
#include "windowlistmodel.h"

#define URL "Compositor"
#define MAJOR 1
#define MINOR 0

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  QQuickView view;
  view.setResizeMode(QQuickView::SizeRootObjectToView);

  Compositor comp(&view);

  QObject::connect(&view, SIGNAL(afterRendering()), &comp, SLOT(sendCallbacks()));

  view.resize(300, 500);//Geometry(0, 0, 1024, 768);

  qmlRegisterUncreatableType<WindowListModel>(URL, MAJOR, MINOR, "WindowListModel", "Cannot be created");

  view.rootContext()->setContextProperty("compositor", &comp);
  view.setSource(QUrl("main.qml"));

  QObject::connect(&comp, SIGNAL(windowAdded(QVariant)), view.rootObject(), SLOT(windowAdded(QVariant)));
  QObject::connect(&comp, SIGNAL(windowRemoved(QVariant)), view.rootObject(), SLOT(windowRemoved(QVariant)));

  view.show();

  return app.exec();
}
