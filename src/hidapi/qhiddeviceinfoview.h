#ifndef QHIDDEVICEINFOVIEW_H
#define QHIDDEVICEINFOVIEW_H

#include <QTableView>
#include <QHeaderView>

#include "qhidapi_global.h"
#include "hexformatdelegate.h"

class QHIDAPISHARED_EXPORT QHidDeviceInfoView : public QTableView {
    Q_OBJECT
public:
    explicit QHidDeviceInfoView(QWidget *parent = 0);

    virtual QSize sizeHint () const;

signals:

public slots:

};

#endif // QHIDDEVICEINFOVIEW_H
