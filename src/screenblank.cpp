#include "screenblank.h"
#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>

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
  m_native->nativeResourceFunctionForIntegration(m_blank ? "displayoff" : "displayon");
}
