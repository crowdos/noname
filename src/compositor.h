#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <QWaylandQuickCompositor>
#include <QVariant>

class QWaylandSurfaceItem;
class WindowListModel;
class QWaylandQuickSurface;

class Compositor : public QObject, public QWaylandQuickCompositor {
  Q_OBJECT
  Q_PROPERTY(WindowListModel *windowList READ windowList CONSTANT);
  Q_PROPERTY(QWaylandQuickSurface *fullScreenSurface READ fullScreenSurface WRITE setFullScreenSurface NOTIFY fullScreenSurfaceChanged);

public:
  Compositor();
  ~Compositor();

  Q_INVOKABLE QWaylandSurfaceItem *item(QWaylandSurface *surface);

  WindowListModel *windowList() const;

  QWaylandQuickSurface *fullScreenSurface() const;
  void setFullScreenSurface(QWaylandQuickSurface *surface);

protected:
  void surfaceCreated(QWaylandSurface *surface);
  QWaylandSurfaceView *createView(QWaylandSurface *surface);

public slots:
  void sendCallbacks();

private slots:
  void surfaceMapped();
  void surfaceUnmapped();
  void surfaceRaiseRequested();
  void surfaceLowerRequested();
  void surfaceVisibilityChanged();
  void surfaceDestroyed();

signals:
  void windowAdded(QVariant window);
  void windowRemoved(QVariant window);
  void fullScreenSurfaceChanged();

private:
  void setSurfaceGeometry(QWaylandSurface *surface);

  WindowListModel *m_model;
  QWaylandQuickSurface *m_fullScreen;
};


#endif /* COMPOSITOR_H */
