#ifndef UDEVMONITOR_H
#define UDEVMONITOR_H

#include <QObject>

class QPlainTextEdit;

class UdevMonitor : public QObject {
    Q_OBJECT
public:
    explicit UdevMonitor(QPlainTextEdit *edit, QObject *parent = 0);

signals:

public slots:

};

#endif // UDEVMONITOR_H
