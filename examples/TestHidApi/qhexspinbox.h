#ifndef QHEXSPINBOX_H
#define QHEXSPINBOX_H
/*
Copyright (C) 2013 - 2016 by Simon Meaden <[simonmeaden@virginmedia.com]>

Permission to use, copy, modify, and/or distribute this software for any purpose
with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
THIS SOFTWARE.
*/

#include <QRegularExpressionValidator>
#include <QSpinBox>
#include <QValidator>

class QHexSpinBox : public QSpinBox
{
  Q_OBJECT

  Q_PROPERTY(int numDigits READ numDigits WRITE setNumDigits)

public:
  explicit QHexSpinBox(QWidget* parent = 0);

  quint8 numDigits();
  void setNumDigits(quint8);

signals:

public slots:

protected:
  QRegularExpressionValidator* m_validator;
  quint8 m_numDidits;
  QString m_regString;

  QValidator::State validate(QString& value, int& pos) const;
  int valueFromText(const QString& text) const;
  QString textFromValue(int val) const;
};

#endif // QHEXSPINBOX_H
