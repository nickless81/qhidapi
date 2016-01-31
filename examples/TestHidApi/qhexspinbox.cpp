#include "qhexspinbox.h"

QHexSpinBox::QHexSpinBox(QWidget *parent) : QSpinBox(parent) {
    setRange(0x0, 0xFF);
    m_regString = "[0-9A-Fa-f]{1,2}";
    QRegularExpression regex(m_regString);
    m_validator = new QRegularExpressionValidator(regex, this);
    m_numDidits = 2;
}

QValidator::State QHexSpinBox::validate(QString &value, int &pos) const {
    return m_validator->validate(value, pos);
}

int QHexSpinBox::valueFromText(const QString &text) const {
    return text.toInt();
}

QString QHexSpinBox::textFromValue(int value) const {
    return QString::number(value, 16).toUpper();
}

/*!
 * \brief Gets the number of digits allowed in the text string. By default 2.
 *
 * \return the number of digits.
 */
quint8 QHexSpinBox::numDigits() {
    return m_numDidits;
}

/*!
 * \brief Sets the number of digits allowed in the text string. By default 2.
 *
 * \param numDigits the number of digits.
 */
void QHexSpinBox::setNumDigits(quint8 numDigits) {
    m_numDidits = numDigits;
    m_regString = "[0-9A-Fa-f]{1," + QString::number(m_numDidits) + "}";
}
