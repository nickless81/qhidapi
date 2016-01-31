#ifndef QHidDeviceInfoModel_H
#define QHidDeviceInfoModel_H

#include <QAbstractTableModel>
#include <QStyledItemDelegate>

#include "qhidapi_global.h"
#include "qhidapi.h"
#include "qhiddeviceinfomodel.h"

class QHIDAPISHARED_EXPORT QHidDeviceInfoModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit QHidDeviceInfoModel(QObject *parent = 0);
    explicit QHidDeviceInfoModel(QList<QHidDeviceInfo> data, QObject *parent = 0);
    ~QHidDeviceInfoModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    static const int s_columnCount = 6;

signals:

public slots:
    void setDataSet(QList<QHidDeviceInfo> data);

protected:
    QList<QHidDeviceInfo> m_data;

};

#endif // QHidDeviceInfoModel_H
