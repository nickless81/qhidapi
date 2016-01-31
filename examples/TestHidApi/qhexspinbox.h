#ifndef QHEXSPINBOX_H
#define QHEXSPINBOX_H

#include <QSpinBox>
#include <QValidator>
#include <QRegularExpressionValidator>

class QHexSpinBox : public QSpinBox {
    Q_OBJECT

    Q_PROPERTY(int numDigits READ numDigits WRITE setNumDigits)

public:
    explicit QHexSpinBox(QWidget *parent = 0);

    quint8 numDigits();
    void setNumDigits(quint8);

signals:

public slots:

protected:
    QRegularExpressionValidator *m_validator;
    quint8 m_numDidits;
    QString m_regString;

    QValidator::State validate(QString &value, int &pos) const;
    int valueFromText(const QString &text) const;
    QString textFromValue(int val) const;

};

#endif // QHEXSPINBOX_H
