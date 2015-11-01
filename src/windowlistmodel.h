#ifndef WINDOW_LIST_MODEL_H
#define WINDOW_LIST_MODEL_H

#include <QAbstractListModel>

class SurfaceContainer;

class WindowListModel : public QAbstractListModel {
  Q_OBJECT

public:
  WindowListModel(QObject *parent = 0);
  ~WindowListModel();

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  void addWindow(SurfaceContainer *window);
  void removeWindow(SurfaceContainer *window);

  QHash<int, QByteArray> roleNames() const;

private:
  QList<SurfaceContainer *> m_windows;
};

#endif /* WINDOW_LIST_MODEL_H */
