#ifndef QHIDDEVICEINFOVIEW_H
#define QHIDDEVICEINFOVIEW_H

#include <QTableView>
#include <QHeaderView>

#include "hexformatdelegate.h"

class QHidDeviceInfoView : public QTableView {
    Q_OBJECT
public:
    explicit QHidDeviceInfoView(QWidget *parent = 0);

    virtual QSize sizeHint () const;

signals:

public slots:

};

#endif // QHIDDEVICEINFOVIEW_H
