#include "qhiddeviceinfoview.h"

QHidDeviceInfoView::QHidDeviceInfoView(QWidget *parent) : QTableView(parent) {
    setItemDelegateForColumn(0, new HexFormatDelegate(this));
    setItemDelegateForColumn(1, new HexFormatDelegate(this));
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setStretchLastSection(true);
}

QSize QHidDeviceInfoView::sizeHint () const {
    QSize size = QTableView::sizeHint();
    return QSize(700, size.height());
}
