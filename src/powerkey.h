#ifndef POWER_KEY_H
#define POWER_KEY_H

#include <QObject>

class Context;
class Keys;
class QSocketNotifier;

class PowerKey : public QObject {
  Q_OBJECT
  Q_PROPERTY(State state READ state NOTIFY stateChanged);
  Q_ENUMS(State);

public:
  PowerKey(QObject *parent = 0);
  ~PowerKey();

  typedef enum {
    Unknown = 0,
    Pressed = 1,
    Released = 2,
  } State;

  void setState(const State& state);
  State state();

signals:
  void stateChanged();

private:
  Context *m_ctx;
  Keys *m_keys;
  QSocketNotifier *m_notifier;
  State m_state;
};

#endif /* POWER_KEY_H */
