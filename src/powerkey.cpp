#include "powerkey.h"
#include <hwhal/context.h>
#include <hwhal/keys.h>
#include <hwhal/loopintegration/glib.h>
#include <unistd.h>

PowerKey::PowerKey(QObject *parent) :
  QObject(parent),
  m_loop(new LoopIntegrationGlib),
  m_ctx(Context::create(m_loop)),
  m_keys(nullptr),
  m_state(Unknown) {

  if (!m_ctx) {
    qFatal("Failed to create hwhal context");
  }

  m_keys = m_ctx->control<Keys>(ControlId::Keys);

  m_keys->monitor(Keys::Key::Power, [this](bool pressed) {
      setState(pressed ? Pressed : Released);
    });
}

PowerKey::~PowerKey() {
  m_keys->monitor(Keys::Key::Power, nullptr);

  m_ctx->put(m_keys);
  m_keys = nullptr;

  delete m_ctx;
  m_ctx = nullptr;

  delete m_loop;
  m_loop = nullptr;
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
