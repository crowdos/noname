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
  if (surfaceIsValid()) {
    m_compositor->seat()->setTimestamp(event->timestamp());
    // From the documentation:
    // The location of the click is given by the last motion or enter event.
    m_compositor->seat()->setPointerPos(event->localPos().toPoint());
    m_compositor->seat()->pointerButtonPressed(event->button());
  } else {
    QQuickItem::mousePressEvent(event);
  }
}

void FullScreenSurfaceView::mouseMoveEvent(QMouseEvent *event) {
  if (surfaceIsValid()) {
    m_compositor->seat()->setTimestamp(event->timestamp());
    m_compositor->seat()->setPointerPos(event->localPos().toPoint());
  } else {
    QQuickItem::mouseMoveEvent(event);
  }
}

void FullScreenSurfaceView::mouseReleaseEvent(QMouseEvent *event) {
  if (surfaceIsValid()) {
    m_compositor->seat()->setTimestamp(event->timestamp());
    // From the documentation:
    // The location of the click is given by the last motion or enter event.
    m_compositor->seat()->setPointerPos(event->localPos().toPoint());
    m_compositor->seat()->pointerButtonReleased(event->button());
  } else {
    QQuickItem::mouseReleaseEvent(event);
  }
}

void FullScreenSurfaceView::wheelEvent(QWheelEvent *event) {
  // TODO: Directions are reversed :(
  // To test: qmlscene -platform wayland
  // Stolen blindly from KWayland renderingservertest.cpp
  if (surfaceIsValid()) {
    m_compositor->seat()->setTimestamp(event->timestamp());
    m_compositor->seat()->setPointerPos(event->pos());

    const QPoint& angle = event->angleDelta() / (8 * 15);
    if (angle.x() != 0) {
      m_compositor->seat()->pointerAxis(Qt::Horizontal, angle.x());
    }

    if (angle.y() != 0) {
      m_compositor->seat()->pointerAxis(Qt::Vertical, angle.y());
    }
  } else {
    QQuickItem::wheelEvent(event);
  }
}

void FullScreenSurfaceView::keyPressEvent(QKeyEvent *event) {
  if (surfaceIsValid()) {
    m_compositor->seat()->setTimestamp(event->timestamp());
    m_compositor->seat()->keyPressed(event->nativeScanCode() - 8);
  } else {
    QQuickItem::keyPressEvent(event);
  }
}

void FullScreenSurfaceView::keyReleaseEvent(QKeyEvent *event) {
  if (surfaceIsValid()) {
    m_compositor->seat()->setTimestamp(event->timestamp());
    m_compositor->seat()->keyReleased(event->nativeScanCode() - 8);
  } else {
    QQuickItem::keyReleaseEvent(event);
  }
}
