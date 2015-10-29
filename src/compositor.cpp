#include "compositor.h"
#include <QWaylandSurface>
#include <QWaylandSurfaceView>
#include <QWaylandSurfaceItem>
#include <QWaylandInputDevice>
#include <QQmlEngine>
#include <QQuickWindow>
#include "windowlistmodel.h"
#include "compositorwindow.h"

#include <QDebug>

Compositor::Compositor(QQuickWindow *window) :
  QWaylandQuickCompositor(0, (ExtensionFlags)DefaultExtensions & ~QtKeyExtension),
  m_model(new WindowListModel(this)),
  m_fullScreen(0) {
  addDefaultShell();

  createOutput(window, "", "");
}

Compositor::~Compositor() {

}

QWaylandSurfaceItem *Compositor::item(QWaylandSurface *surface) {
  QWaylandSurfaceItem *item =
    surface ? static_cast<QWaylandSurfaceItem *>(surface->views().first()) : 0;

  if (item) {
    QQmlEngine::setObjectOwnership(item, QQmlEngine::CppOwnership);
  }

  return item;
}

QWaylandQuickSurface *Compositor::fullScreenSurface() const {
  return m_fullScreen;
}

void Compositor::setFullScreenSurface(QWaylandQuickSurface *surface) {
  if (m_fullScreen == surface) {
    return;
  }

  if (m_fullScreen) {
    QWaylandSurfaceItem *item = Compositor::item(m_fullScreen);
    item->setTouchEventsEnabled(false);
    defaultInputDevice()->setKeyboardFocus(0);
    item->setFocus(false);
  }

  m_fullScreen = surface;

  if (m_fullScreen) {
    QWaylandSurfaceItem *item = Compositor::item(m_fullScreen);
    item->takeFocus();
    item->setTouchEventsEnabled(true);
  }
  // TODO:

  emit fullScreenSurfaceChanged();
}

void Compositor::surfaceCreated(QWaylandSurface *surface) {
  QObject::connect(surface, SIGNAL(mapped()), this, SLOT(surfaceMapped()));
  QObject::connect(surface, SIGNAL(unmapped()), this, SLOT(surfaceUnmapped()));
  QObject::connect(surface, SIGNAL(raiseRequested()), this, SLOT(surfaceRaiseRequested()));
  QObject::connect(surface, SIGNAL(lowerRequested()), this, SLOT(surfaceLowerRequested()));
  QObject::connect(surface, SIGNAL(visibilityChanged()), this, SLOT(surfaceVisibilityChanged()));
  QObject::connect(surface, SIGNAL(surfaceDestroyed()), this, SLOT(surfaceDestroyed()));
}

QWaylandSurfaceView *Compositor::createView(QWaylandSurface *surface) {
  CompositorWindow *win = new CompositorWindow(qobject_cast<QWaylandQuickSurface *>(surface));

  win->setResizeSurfaceToItem(true);

  return win;
}

void Compositor::sendCallbacks() {
  sendFrameCallbacks(surfaces());
}

WindowListModel *Compositor::windowList() const {
  return m_model;
}

void Compositor::surfaceMapped() {
  QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
  static_cast<QWaylandQuickSurface *>(surface)->setClientRenderingEnabled(true);
  m_model->addWindow(static_cast<CompositorWindow *>(surface->views().first()));
  emit windowAdded(QVariant::fromValue(surface));

  setSurfaceGeometry(surface);

  setFullScreenSurface(qobject_cast<QWaylandQuickSurface *>(surface));
}

void Compositor::surfaceUnmapped() {
  //  QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());

  // TODO:
  qDebug() << Q_FUNC_INFO;
}

void Compositor::surfaceRaiseRequested() {
  QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());

  setFullScreenSurface(qobject_cast<QWaylandQuickSurface *>(surface));
}

void Compositor::surfaceLowerRequested() {
  QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());

  if (surface == m_fullScreen) {
    setFullScreenSurface(0);
  }
}

void Compositor::surfaceVisibilityChanged() {
  // TODO:
  qDebug() << Q_FUNC_INFO;
}

void Compositor::surfaceDestroyed() {
  QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());

  if (surface == m_fullScreen) {
    setFullScreenSurface(0);
  }

  if (!surface->views().isEmpty()) {
    m_model->removeWindow(static_cast<CompositorWindow *>(surface->views().first()));
    emit windowRemoved(QVariant::fromValue(surface));

    delete surface->views().first();
  }
}

void Compositor::setSurfaceGeometry(QWaylandSurface *surface) {
  QWaylandSurfaceItem *item = Compositor::item(surface);
  if (item) {
    item->setPos(QPointF(0, 0));
    item->setSize(outputGeometry().size());
  }
}
