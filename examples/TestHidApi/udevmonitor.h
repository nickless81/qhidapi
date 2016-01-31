#ifndef UDEVMONITOR_H
#define UDEVMONITOR_H

#include <QObject>
#include <wrappableobject.h>

class QPlainTextEdit;

class UdevMonitor : public WrappableObject {
    Q_OBJECT
public:
    explicit UdevMonitor(QPlainTextEdit *edit, QObject *parent = 0);

signals:

public slots:

};

#endif // UDEVMONITOR_H
