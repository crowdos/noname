#include "surfacecontainer.h"
#include <KWayland/Server/shell_interface.h>
#include <KWayland/Server/surface_interface.h>
#include <QDebug>

SurfaceContainer::SurfaceContainer(KWayland::Server::ShellSurfaceInterface *surface,
				   QObject *parent) :
  QObject(parent),
  m_surface(surface) {

  // TODO:
  QObject::connect(m_surface,
		   &KWayland::Server::ShellSurfaceInterface::destroyed,
		   this, [this] {
		     if (m_surface) {
		       m_surface = nullptr;
		       deleteLater();
		     }
		   });

  QObject::connect(m_surface, &KWayland::Server::ShellSurfaceInterface::titleChanged,
		   this, &SurfaceContainer::titleChanged);

  QObject::connect(m_surface->surface(), &KWayland::Server::SurfaceInterface::damaged,
		   this, &SurfaceContainer::update);
  QObject::connect(m_surface->surface(), &KWayland::Server::SurfaceInterface::opaqueChanged,
		   this, &SurfaceContainer::update);
  QObject::connect(m_surface->surface(), &KWayland::Server::SurfaceInterface::unmapped,
		   this, &SurfaceContainer::update);

  qDebug() << surface << surface->isToplevel();
}

SurfaceContainer::~SurfaceContainer() {
  if (m_surface) {
    m_surface->deleteLater();
    m_surface = nullptr;
  }
}

QString SurfaceContainer::title() const {
  return m_surface->title();
}
