#include "windowlistmodel.h"
#include "compositorwindow.h"
#include <QWaylandSurfaceItem>
#include <QDebug>

enum {
  SurfaceRole = Qt::UserRole + 1,
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
  if (index.row() < m_windows.size()) {
    return QVariant::fromValue(static_cast<QWaylandSurfaceItem *>(m_windows[index.row()]));
  }

  return QVariant();
}

void WindowListModel::addWindow(CompositorWindow *window) {
  qDebug() << Q_FUNC_INFO << window;
  if (m_windows.indexOf(window) == -1) {
    beginInsertRows(QModelIndex(), m_windows.size(), m_windows.size());
    m_windows << window;
    endInsertRows();
  }
}

void WindowListModel::removeWindow(CompositorWindow *window) {
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
  roles[SurfaceRole] = "surface";

  return roles;

}

