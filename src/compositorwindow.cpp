#include "compositorwindow.h"

CompositorWindow::CompositorWindow(QWaylandQuickSurface *surface, QQuickItem *parent) :
  QWaylandSurfaceItem(surface, parent) {

}

CompositorWindow::~CompositorWindow() {

}
