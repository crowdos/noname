#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <QObject>
#include <QVariant>
#include <QSize>

class WindowListModel;
namespace KWayland {
  namespace Server {
    class Display;
    class SeatInterface;
    class ShellSurfaceInterface;
    class SurfaceInterface;
  };
};

class SurfaceContainer;

class Compositor : public QObject {
  Q_OBJECT
  Q_PROPERTY(WindowListModel *windowList READ windowList CONSTANT);
  Q_PROPERTY(SurfaceContainer *fullScreenSurface READ fullScreenSurface WRITE setFullScreenSurface NOTIFY fullScreenSurfaceChanged);

public:
  Compositor(KWayland::Server::Display& display, const QSize& windowSize, QObject *parent = 0);
  ~Compositor();

  WindowListModel *windowList() const;

  SurfaceContainer *fullScreenSurface() const;
  void setFullScreenSurface(SurfaceContainer *surface);

public slots:
  void surfaceCreated(KWayland::Server::ShellSurfaceInterface *surface);

signals:
  void windowAdded(QVariant window);
  void windowRemoved(QVariant window);
  void fullScreenSurfaceChanged();

private:
  void surfaceDestroyed(SurfaceContainer *container);

  void setSurfaceGeometry(KWayland::Server::ShellSurfaceInterface *surface);

  WindowListModel *m_model;
  SurfaceContainer *m_fullScreen;

  QSize m_windowSize;
  KWayland::Server::Display& m_display;
  KWayland::Server::SeatInterface *m_seat;
};


#endif /* COMPOSITOR_H */
