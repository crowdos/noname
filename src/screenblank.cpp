#include "screenblank.h"
#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>
#include <QQuickWindow>

ScreenBlank::ScreenBlank(QObject *parent) :
  QObject(parent),
  m_blank(false),
  m_native(QGuiApplication::platformNativeInterface()) {

  update();
}

ScreenBlank::~ScreenBlank() {

}

bool ScreenBlank::isBlank() const {
  return m_blank;
}

void ScreenBlank::setBlank(bool blank) {
  if (blank != isBlank()) {
    m_blank = blank;
    update();
    emit blankChanged();
  }
}

void ScreenBlank::update() {
  auto callUpdate = [] () {
    QWindowList windows(QGuiApplication::topLevelWindows());
    for (int x = 0; x < windows.size(); x++) {
      if (QQuickWindow *win = dynamic_cast<QQuickWindow *>(windows[x])) {
	win->update();
      }
    }
  };

  m_native->nativeResourceForIntegration(m_blank ? "displayoff" : "displayon");

  if (!m_blank) {
    callUpdate();
  }
}
