#ifndef COMPOSITOR_WINDOW_H
#define COMPOSITOR_WINDOW_H

#include <QWaylandSurfaceItem>

class CompositorWindow : public QWaylandSurfaceItem {
  Q_OBJECT

public:
  CompositorWindow(QWaylandQuickSurface *surface, QQuickItem *parent = 0);
  ~CompositorWindow();
};

#endif /* COMPOSITOR_WINDOW_H */
