#ifndef HEXFORMATDELEGATE_H
#define HEXFORMATDELEGATE_H

#include <QStyledItemDelegate>

class HexFormatDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit HexFormatDelegate(QObject *parent = 0);
    virtual QString displayText(const QVariant &value, const QLocale &locale) const;

signals:

public slots:

};

#endif // HEXFORMATDELEGATE_H
