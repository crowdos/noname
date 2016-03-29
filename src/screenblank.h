#ifndef SCREEN_BLANK_H
#define SCREEN_BLANK_H

#include <QObject>

class QPlatformNativeInterface;

class ScreenBlank : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool blank READ isBlank WRITE setBlank NOTIFY blankChanged);

public:
  ScreenBlank(QObject *parent = 0);
  ~ScreenBlank();

  bool isBlank() const;
  void setBlank(bool blank);

signals:
  void blankChanged();

private:
  void update();

  bool m_blank;
  QPlatformNativeInterface *m_native;
};

#endif /* SCREEN_BLANK_H */
