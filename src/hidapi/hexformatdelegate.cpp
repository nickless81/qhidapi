#include "hexformatdelegate.h"

HexFormatDelegate::HexFormatDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

QString HexFormatDelegate::displayText(const QVariant &value, const QLocale &locale) const {
    Q_UNUSED(locale)
    bool ok;
    int v = value.toInt(&ok);
    if (ok) {
        QString formattedNum = QString::number(v, 16);
        return formattedNum;
    }
    return QString();
}
