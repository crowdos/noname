#include "powerkey.h"
#include <hwhal/context.h>
#include <hwhal/keys.h>
#include <QSocketNotifier>
#include <unistd.h>

PowerKey::PowerKey(QObject *parent) :
  QObject(parent),
  m_ctx(Context::create()),
  m_keys(nullptr),
  m_notifier(nullptr),
  m_state(Unknown) {

  if (!m_ctx) {
    qFatal("Failed to create hwhal context");
  }

  m_keys = m_ctx->control<Keys>(ControlId::Keys);

  int fd = m_keys->fd(Keys::Key::Power);

  m_notifier = new QSocketNotifier(fd, QSocketNotifier::Read, this);
  QObject::connect(m_notifier, &QSocketNotifier::activated,
		   this, [this]() {
		     char c;
		     while (::read(m_notifier->socket(), &c, sizeof(c)) > 0) {
		       if (c == KEY_RELEASED) {
			 setState(Pressed);
		       } else if (c == KEY_PRESSED) {
			 setState(Released);
		       }
		     }
		   });

  m_keys->monitor(Keys::Key::Power, true);
}

PowerKey::~PowerKey() {
  m_ctx->put(m_keys);
  m_keys = nullptr;

  delete m_ctx;
  m_ctx = nullptr;
}

void PowerKey::setState(const PowerKey::State& state) {
  if (m_state != state) {
    m_state = state;
    emit stateChanged();
  }
}

PowerKey::State PowerKey::state() {
  return m_state;
}
