#include "compositor.h"
#include <KWayland/Server/seat_interface.h>
#include <KWayland/Server/display.h>
#include <KWayland/Server/shell_interface.h>
#include <QQmlEngine>
#include "surfacecontainer.h"
#include "windowlistmodel.h"
#include <QDebug>

Compositor::Compositor(KWayland::Server::Display& display,
		       const QSize& windowSize, QObject *parent) :
  QObject(parent),
  m_model(new WindowListModel(this)),
  m_fullScreen(nullptr),
  m_windowSize(windowSize),
  m_display(display) {

  m_seat = display.createSeat(this);
  m_seat->setHasKeyboard(true);
  m_seat->setHasPointer(true);
  m_seat->setHasTouch(true);
  m_seat->setName(QStringLiteral("main"));
  m_seat->create();
}

Compositor::~Compositor() {

}

void Compositor::surfaceCreated(KWayland::Server::ShellSurfaceInterface *surface) {
  SurfaceContainer *container = new SurfaceContainer(surface, this);

  QObject::connect(container, &SurfaceContainer::destroyed,
		   this, [container, this] { surfaceDestroyed(container); });

  QQmlEngine::setObjectOwnership(container, QQmlEngine::CppOwnership); // TODO: is this needed?

  setSurfaceGeometry(surface);

  m_model->addWindow(container);

  emit windowAdded(QVariant::fromValue(container));

  qDebug() << Q_FUNC_INFO << container;
}

void Compositor::surfaceDestroyed(SurfaceContainer *container) {
  // TODO: container will take care of destroying itself
  qDebug() << Q_FUNC_INFO << container;

  if (m_fullScreen == container) {
    setFullScreenSurface(nullptr);
  }

  m_model->removeWindow(container);

  emit windowRemoved(QVariant::fromValue(container));
}

SurfaceContainer *Compositor::fullScreenSurface() const {
  return m_fullScreen;
}

void Compositor::setFullScreenSurface(SurfaceContainer *surface) {
  if (m_fullScreen == surface) {
    return;
  }

  m_fullScreen = surface;

  m_seat->setFocusedPointerSurface(surface ? surface->surface()->surface() : nullptr);

  // TODO:

  emit fullScreenSurfaceChanged();
}

#if 0
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
#endif

WindowListModel *Compositor::windowList() const {
  return m_model;
}

#if 0
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
#endif

void Compositor::setSurfaceGeometry(KWayland::Server::ShellSurfaceInterface *surface) {
  surface->requestSize(m_windowSize);
}

