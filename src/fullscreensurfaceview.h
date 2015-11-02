#ifndef FULL_SCREEN_SURFACE_VIEW_H
#define FULL_SCREEN_SURFACE_VIEW_H

#include <QQuickItem>

class SurfaceContainer;
class Compositor;

class FullScreenSurfaceView : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(SurfaceContainer *surface READ surface WRITE setSurface NOTIFY surfaceChanged);
  Q_PROPERTY(Compositor *compositorObject READ compositor WRITE setCompositor NOTIFY compositorChanged);

public:
  FullScreenSurfaceView(QQuickItem *parent = 0);
  ~FullScreenSurfaceView();

  SurfaceContainer *surface() const;
  void setSurface(SurfaceContainer *surface);

  Compositor *compositor() const;
  void setCompositor(Compositor *compositor);

  QSGNode *updatePaintNode(QSGNode *oldNode,
			   UpdatePaintNodeData *updatePaintNodeData) Q_DECL_OVERRIDE;

protected:
  void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
  void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
  void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

signals:
  void surfaceChanged();
  void compositorChanged();

private:
  bool surfaceIsValid();

  SurfaceContainer *m_surface;
  Compositor *m_compositor;
};

#endif /* FULL_SCREEN_SURFACE_VIEW_H */
