#include "windowlistmodel.h"
#include "surfacecontainer.h"
#include <QDebug>

enum {
  ItemRole = Qt::UserRole + 1,
};


WindowListModel::WindowListModel(QObject *parent) :
  QAbstractListModel(parent) {

}

WindowListModel::~WindowListModel() {
  m_windows.clear();
}

int WindowListModel::rowCount(const QModelIndex& parent) const {
  return parent.isValid() ? 0 : m_windows.size();
}

QVariant WindowListModel::data(const QModelIndex& index, int role) const {
  if (index.row() < m_windows.size() && role == ItemRole) {
    return QVariant::fromValue(m_windows[index.row()]);
  }

  return QVariant();
}

void WindowListModel::addWindow(SurfaceContainer *window) {
  qDebug() << Q_FUNC_INFO << window;
  if (m_windows.indexOf(window) == -1) {
    beginInsertRows(QModelIndex(), m_windows.size(), m_windows.size());
    m_windows << window;
    endInsertRows();
  }
}

void WindowListModel::removeWindow(SurfaceContainer *window) {
  qDebug() << Q_FUNC_INFO << window;
  int index = m_windows.indexOf(window);
  if (index != -1) {
    beginRemoveRows(QModelIndex(), index, index);
    m_windows.takeAt(index);
    endRemoveRows();
  }
}

QHash<int, QByteArray> WindowListModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[ItemRole] = "item";

  return roles;

}

