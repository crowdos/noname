#ifndef SURFACE_CONTAINER_H
#define SURFACE_CONTAINER_H

#include <QObject>

namespace KWayland {
  namespace Server{
    class ShellSurfaceInterface;
  };
};

class SurfaceContainer : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString title READ title NOTIFY titleChanged);

public:
  SurfaceContainer(KWayland::Server::ShellSurfaceInterface *surface, QObject *parent = 0);
  ~SurfaceContainer();

  QString title() const;

  KWayland::Server::ShellSurfaceInterface *surface() { return m_surface; }

signals:
  void titleChanged();
  void update();

private:
  KWayland::Server::ShellSurfaceInterface *m_surface;
};

#endif /* SURFACE_CONTAINER_H */
