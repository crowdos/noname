#ifndef DECLARATIVE_DIRECTION_H
#define DECLARATIVE_DIRECTION_H

#include <QObject>

class DeclarativeDirection : public QObject {
  Q_OBJECT
  Q_ENUMS(Direction);

public:
  enum Direction {
    TopToBottom,
    BottomToTop,
    RightToLeft,
    LeftToRight,
  };

};

#endif /* DECLARATIVE_DIRECTION_H */
