#ifndef SURFACE_CONTAINER_VIEW_H
#define SURFACE_CONTAINER_VIEW_H

#include <QQuickItem>

class SurfaceContainer;

class SurfaceContainerView : public QQuickItem {
  Q_OBJECT

  Q_PROPERTY(SurfaceContainer *surface READ surface WRITE setSurface NOTIFY surfaceChanged);

public:
  SurfaceContainerView(QQuickItem *parent = 0);
  ~SurfaceContainerView();

  SurfaceContainer *surface() const;
  void setSurface(SurfaceContainer *surface);

  QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *updatePaintNodeData);

signals:
  void surfaceChanged();

private:
  SurfaceContainer *m_surface;
};

#endif /* SURFACE_CONTAINER_VIEW_H */
