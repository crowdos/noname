#ifndef BACKGROUND_IMAGE_H
#define BACKGROUND_IMAGE_H

#include <QQuickItem>

class QSGTexture;

class BackgroundImage : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged);
  Q_PROPERTY(qreal size READ size WRITE setSize NOTIFY sizeChanged);

public:
  BackgroundImage(QQuickItem *parent = 0);
  ~BackgroundImage();

  qreal size() const;
  void setSize(qreal size);

  QString source() const;
  void setSource(const QString& source);

signals:
  void sizeChanged();
  void sourceChanged();

protected:
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);
  QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *updatePaintNodeData);

private:
  void updateImage();

  qreal m_size;
  QString m_source;
  QSGTexture *m_texture;
  bool m_sizeChanged;
};

#endif /* BACKGROUND_IMAGE_H */
