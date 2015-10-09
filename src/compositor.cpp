#include "compositor.h"
#include <QWaylandSurface>
#include <QWaylandSurfaceView>
#include <QWaylandSurfaceItem>
#include "windowlistmodel.h"
#include "compositorwindow.h"

#include <QDebug>

Compositor::Compositor(QQuickWindow *window) :
  QWaylandQuickCompositor(window),
  m_model(new WindowListModel(this)) {
  addDefaultShell();

}

Compositor::~Compositor() {

}

QWaylandSurfaceItem *Compositor::item(QWaylandSurface *surface) {
  // TODO: declarative ownership
  return static_cast<QWaylandSurfaceItem *>(surface->views().first());
}

void Compositor::surfaceCreated(QWaylandSurface *surface) {
  //  qDebug() << surface << surface->title() << surface->className();

  QObject::connect(surface, SIGNAL(mapped()), this, SLOT(surfaceMapped()));
  QObject::connect(surface, SIGNAL(unmapped()), this, SLOT(surfaceUnmapped()));
  QObject::connect(surface, SIGNAL(raiseRequested()), this, SLOT(surfaceRaiseRequested()));
  QObject::connect(surface, SIGNAL(lowerRequested()), this, SLOT(surfaceLowerRequested()));
  QObject::connect(surface, SIGNAL(visibilityChanged()), this, SLOT(surfaceVisibilityChanged()));
  QObject::connect(surface, SIGNAL(surfaceDestroyed()), this, SLOT(surfaceDestroyed()));
}

QWaylandSurfaceView *Compositor::createView(QWaylandSurface *surface) {
  return new CompositorWindow(static_cast<QWaylandQuickSurface *>(surface));
}

void Compositor::sendCallbacks() {
  sendFrameCallbacks(surfaces());
}

WindowListModel *Compositor::windowList() const {
  return m_model;
}

void Compositor::surfaceMapped() {
  QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
  m_model->addWindow(static_cast<CompositorWindow *>(surface->views().first()));
  emit windowAdded(QVariant::fromValue(surface));
}

void Compositor::surfaceUnmapped() {
  //  QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());

  qDebug() << Q_FUNC_INFO;
}

void Compositor::surfaceRaiseRequested() {
  qDebug() << Q_FUNC_INFO;
}

void Compositor::surfaceLowerRequested() {
  qDebug() << Q_FUNC_INFO;
}

void Compositor::surfaceVisibilityChanged() {
  qDebug() << Q_FUNC_INFO;
}

void Compositor::surfaceDestroyed() {
  QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());

  if (!surface->views().isEmpty()) {
    m_model->removeWindow(static_cast<CompositorWindow *>(surface->views().first()));
    emit windowRemoved(QVariant::fromValue(surface));

    delete surface->views().first();
  }
}
