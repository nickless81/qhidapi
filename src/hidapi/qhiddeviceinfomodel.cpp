#include "qhiddeviceinfomodel.h"
/*
Copyright (C) [year] by Simon Meaden <[simonmeaden@virginmedia.com]>

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

QHidDeviceInfoModel::QHidDeviceInfoModel(QObject *parent) : QAbstractTableModel(parent) {
}

QHidDeviceInfoModel::QHidDeviceInfoModel(QList<QHidDeviceInfo> data, QObject *parent) : QAbstractTableModel(parent) {
    m_data = data;
}

QHidDeviceInfoModel::~QHidDeviceInfoModel() {

}

void QHidDeviceInfoModel::setDataSet(QList<QHidDeviceInfo> data) {
    emit beginResetModel();
    m_data = data;
    emit endResetModel();
}

int QHidDeviceInfoModel::rowCount(const QModelIndex& /*parent*/) const {
    return m_data.size();
}

int QHidDeviceInfoModel::columnCount(const QModelIndex& /*parent*/) const {
    return s_columnCount;
}

QVariant QHidDeviceInfoModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return QVariant(m_data.at(index.row()).vendorId);
            break;
        case 1:
            return QVariant(m_data.at(index.row()).productId);
            break;
        case 2:
            return QVariant(m_data.at(index.row()).manufacturerString);
            break;
        case 3:
            return QVariant(m_data.at(index.row()).productString);
            break;
        case 4:
            return QVariant(m_data.at(index.row()).serialNumber);
            break;
        case 5:
            return QVariant(m_data.at(index.row()).releaseNumber);
            break;
        default:
            break;
        }
    }
    return QVariant();
}

QVariant QHidDeviceInfoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0:
                return QString("Vendor Id");
            case 1:
                return QString("Manufacturer");
            case 2:
                return QString("Product Id");
            case 3:
                return QString("Product");
            case 4:
                return QString("Serial Number");
            case 5:
                return QString("Release");
            default:
                return "";
            }
        }
    }
    return QVariant();
}
