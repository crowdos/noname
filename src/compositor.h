#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <QWaylandQuickCompositor>
#include <QVariant>

class QWaylandSurfaceItem;
class WindowListModel;

class Compositor : public QObject, public QWaylandQuickCompositor {
  Q_OBJECT
  Q_PROPERTY(WindowListModel *windowList READ windowList CONSTANT);

public:
  Compositor(QQuickWindow *window);
  ~Compositor();

  Q_INVOKABLE QWaylandSurfaceItem *item(QWaylandSurface *surface);

  WindowListModel *windowList() const;

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

private:
  WindowListModel *m_model;
};


#endif /* COMPOSITOR_H */
