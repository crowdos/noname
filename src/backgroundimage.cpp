#include "backgroundimage.h"
#include <QImage>
#include <QQmlInfo>
#include <QQuickWindow>
#include <QSGTexture>
#include <QSGSimpleTextureNode>

// TODO: do we need to handle rotation?
BackgroundImage::BackgroundImage(QQuickItem *parent) :
  QQuickItem(parent),
  m_size(0),
  m_texture(0),
  m_sizeChanged(false),
  m_sourceChanged(false) {

  setFlag(ItemHasContents, true);
}

BackgroundImage::~BackgroundImage() {
  if (m_texture) {
    delete m_texture;
    m_texture = nullptr;
  }
}

qreal BackgroundImage::size() const {
  return m_size;
}

void BackgroundImage::setSize(qreal size) {
  if (qIsNaN(size)) {
    return;
  }

  if (m_size != size) {
    m_size = size;
    setImplicitWidth(m_size);
    setImplicitHeight(m_size);
    emit sizeChanged();

    m_sizeChanged = true;
    updateImage();
  }
}

QString BackgroundImage::source() const {
  return m_source;
}

void BackgroundImage::setSource(const QString& source) {
  if (m_source != source) {
    m_source = source;

    emit sourceChanged();

    m_sourceChanged = true;
    updateImage();
  }
}

void BackgroundImage::updateImage() {
  if (!isComponentComplete()) {
    return;
  }

  if (m_size <= 0) {
    return;
  }

  if (m_sourceChanged) {
    // No need.
    m_sizeChanged = false;
  } else {
    if (m_sizeChanged) {
      // no need to do anything.
      return;
    }
  }

  qreal sz = qMax(width(), height());
  QSizeF size(sz, sz);

  // TODO: This is not efficient at all.
  QImage img;
  if (!img.load(m_source)) {
    qmlInfo(this) << "Failed to load image" << m_source;
    return;
  }

  size = size.boundedTo(img.size());
  sz = qMin(size.width(), size.height());
  size = QSizeF(sz, sz);

  QRect rect(qAbs(img.width() - size.width()) / 2,
	     qAbs(img.height() - size.height()) / 2,
		  size.width(), size.height());

  img = img.copy(rect);

  if (m_texture) {
    delete m_texture;
  }

  m_texture = window()->createTextureFromImage(img);

  m_sourceChanged = false;
  if (m_texture) {
    update();
  }
}

void BackgroundImage::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) {
  QQuickItem::geometryChanged(newGeometry, oldGeometry);

  if (oldGeometry.size() != newGeometry.size()) {
    m_sizeChanged = true;
    updateImage();
  }
}

QSGNode *BackgroundImage::updatePaintNode(QSGNode *oldNode,
					  UpdatePaintNodeData *updatePaintNodeData) {

  Q_UNUSED(updatePaintNodeData);

  if (m_sizeChanged) {
    if (oldNode) {
      if (QSGSimpleTextureNode *node = dynamic_cast<QSGSimpleTextureNode *>(oldNode)) {
	node->setRect(QRectF(0, 0, width(), height()));
	m_sizeChanged = false;
	return oldNode;
      }
    }
  }

  delete oldNode;
  oldNode = nullptr;

  if (m_texture) {
    m_texture->setFiltering(QSGTexture::Nearest);
    m_texture->setHorizontalWrapMode(QSGTexture::ClampToEdge);
    m_texture->setVerticalWrapMode(QSGTexture::ClampToEdge);

    QSGSimpleTextureNode *node = new QSGSimpleTextureNode;
    node->setOwnsTexture(true);
    node->setRect(QRectF(0, 0, width(), height()));
    node->setTexture(m_texture);
    oldNode = node;
    m_texture = nullptr;
  }

  return oldNode;
}
