#include "surfacecontainerview.h"
#include "surfacecontainer.h"
#include <KWayland/Server/surface_interface.h>
#include <KWayland/Server/buffer_interface.h>
#include <KWayland/Server/shell_interface.h>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>

SurfaceContainerView::SurfaceContainerView(QQuickItem *parent) :
  QQuickItem(parent),
  m_surface(0) {
  setFlag(ItemHasContents, true);
}

SurfaceContainerView::~SurfaceContainerView() {
  m_surface = 0;
}

SurfaceContainer *SurfaceContainerView::surface() const {
  return m_surface;
}

void SurfaceContainerView::setSurface(SurfaceContainer *surface) {
  if (m_surface != surface) {

    if (m_surface) {
      QObject::disconnect(m_surface, &SurfaceContainer::update,
			  this, &SurfaceContainerView::update);
    }

    m_surface = surface;

    if (m_surface) {
      QObject::connect(m_surface, &SurfaceContainer::update,
		       this, &SurfaceContainerView::update);
    }

    emit surfaceChanged();

    update();
  }
}

QSGNode *SurfaceContainerView::updatePaintNode(QSGNode *oldNode,
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
