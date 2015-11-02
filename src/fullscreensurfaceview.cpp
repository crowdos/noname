#include "fullscreensurfaceview.h"
#include "surfacecontainer.h"
#include "compositor.h"
#include <KWayland/Server/surface_interface.h>
#include <KWayland/Server/buffer_interface.h>
#include <KWayland/Server/shell_interface.h>
#include <KWayland/Server/seat_interface.h>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>

FullScreenSurfaceView::FullScreenSurfaceView(QQuickItem *parent) :
  QQuickItem(parent),
  m_surface(nullptr),
  m_compositor(nullptr) {

  setSmooth(true);
  setFlag(ItemHasContents, true);
  setAcceptedMouseButtons(Qt::AllButtons);

#if 0
  setAcceptHoverEvents(true);
#endif
}

FullScreenSurfaceView::~FullScreenSurfaceView() {
  m_surface = nullptr;
}

SurfaceContainer *FullScreenSurfaceView::surface() const {
  return m_surface;
}

void FullScreenSurfaceView::setSurface(SurfaceContainer *surface) {
  if (m_surface != surface) {

    if (m_surface) {
      QObject::disconnect(m_surface, &SurfaceContainer::update,
			  this, &FullScreenSurfaceView::update);
    }

    m_surface = surface;

    if (m_surface) {
      QObject::connect(m_surface, &SurfaceContainer::update,
		       this, &FullScreenSurfaceView::update);
    }

    emit surfaceChanged();

    update();
  }
}

Compositor *FullScreenSurfaceView::compositor() const {
  return m_compositor;
}

void FullScreenSurfaceView::setCompositor(Compositor *compositor) {
  if (m_compositor != compositor) {
    m_compositor = compositor;
    emit compositorChanged();
  }
}

QSGNode *FullScreenSurfaceView::updatePaintNode(QSGNode *oldNode,
						UpdatePaintNodeData *updatePaintNodeData) {
  Q_UNUSED(updatePaintNodeData);

  delete oldNode;
  oldNode = nullptr;

  if (!m_surface || !m_surface->surface()->surface()->buffer()) {
    return oldNode;
  }

  QImage img(m_surface->surface()->surface()->buffer()->data());

  QSGTexture *texture = window()->createTextureFromImage(img);
  texture->setFiltering(QSGTexture::Nearest);
  texture->setHorizontalWrapMode(QSGTexture::ClampToEdge);
  texture->setVerticalWrapMode(QSGTexture::ClampToEdge);

  QSGSimpleTextureNode *node = new QSGSimpleTextureNode;
  node->setOwnsTexture(true);
  node->setRect(QRectF(0, 0, width(), height()));
  node->setTexture(texture);

  oldNode = node;

  return oldNode;
}

bool FullScreenSurfaceView::surfaceIsValid() {
  return m_surface
    && m_surface->surface()
    && m_surface->surface()->surface()
    && m_compositor
    && m_compositor->fullScreenSurface() == m_surface
    && m_compositor->seat()
    && m_compositor->seat()->focusedPointerSurface() == m_surface->surface()->surface();
}

void FullScreenSurfaceView::mousePressEvent(QMouseEvent *event) {
  //  QQuickItem::mousePressEvent(event);

  qDebug() << Q_FUNC_INFO << surfaceIsValid();

  if (surfaceIsValid()) {
    //    grabMouse();
    //    setFocus(true);
    m_compositor->seat()->setTimestamp(event->timestamp());
    m_compositor->seat()->pointerButtonPressed(event->button());
  }
}

void FullScreenSurfaceView::mouseMoveEvent(QMouseEvent *event) {
  //  QQuickItem::mouseMoveEvent(event);

  qDebug() << Q_FUNC_INFO << surfaceIsValid();
  if (surfaceIsValid()) {
    m_compositor->seat()->setTimestamp(event->timestamp());
    m_compositor->seat()->setPointerPos(event->localPos().toPoint());
  }
}

void FullScreenSurfaceView::mouseReleaseEvent(QMouseEvent *event) {
  //  QQuickItem::mouseReleaseEvent(event);

  qDebug() << Q_FUNC_INFO << surfaceIsValid();

  if (surfaceIsValid()) {
    m_compositor->seat()->setTimestamp(event->timestamp());
    m_compositor->seat()->pointerButtonReleased(event->button());
  }
}

#if 0
void FullScreenSurfaceView::hoverEnterEvent(QHoverEvent *event) {
  QQuickItem::hoverEnterEvent(event);

  qDebug() << Q_FUNC_INFO;
}

void FullScreenSurfaceView::hoverMoveEvent(QHoverEvent *event) {
  QQuickItem::hoverMoveEvent(event);

  qDebug() << Q_FUNC_INFO;
}
#endif

void FullScreenSurfaceView::wheelEvent(QWheelEvent *event) {
  QQuickItem::wheelEvent(event);

  // Stolen blindly from KWayland renderingservertest.cpp
  if (surfaceIsValid()) {
    m_compositor->seat()->setTimestamp(event->timestamp());
    const QPoint &angle = event->angleDelta() / (8 * 15);
    if (angle.x() != 0) {
      m_compositor->seat()->pointerAxis(Qt::Horizontal, angle.x());
    }

    if (angle.y() != 0) {
      m_compositor->seat()->pointerAxis(Qt::Vertical, angle.y());
    }
  }
}

void FullScreenSurfaceView::keyPressEvent(QKeyEvent *event) {
  //  QQuickItem::keyPressEvent(event);

  qDebug() << Q_FUNC_INFO << surfaceIsValid();

  if (surfaceIsValid()) {
    m_compositor->seat()->setTimestamp(event->timestamp());
    m_compositor->seat()->keyPressed(event->nativeScanCode() - 8);
  }
}

void FullScreenSurfaceView::keyReleaseEvent(QKeyEvent *event) {
  //  QQuickItem::keyReleaseEvent(event);

  qDebug() << Q_FUNC_INFO << surfaceIsValid();

  if (surfaceIsValid()) {
    m_compositor->seat()->setTimestamp(event->timestamp());
    m_compositor->seat()->keyPressed(event->nativeScanCode() - 8);
  }
}
