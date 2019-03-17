#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

#include <QByteArray>
#include <QFrame>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QTabWidget>
#include <QTableView>
#include <QThread>
#include <QTimer>

#include <QHidApi>

struct udev;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = 0);
  ~MainWindow();

signals:

public slots:

protected slots:
  void udevData();
  void hidData();
  //    void udevMonitoring();
  void connectDevice();
  void disconnectDevice();
  void rescan(ushort mVendorId = 0x0, ushort mProductId = 0x0);
  int sendOutputReport();
  int sendFeatureReport();
  int getFeatureReport();
  void clear();
  void timeout();

protected:
  QHidApi* pHidApi;
  QList<QHidDeviceInfo> mDevices;
  QHidDeviceInfoModel mModel;
  QTableView* pInfoView;
  QPushButton *pConnectBtn, *pDisconnectBtn, *pSescanBtn, *pClearBtn,
    *pOutputBtn, *pFeatureBtn, *pGetFeatureBtn;
  QPlainTextEdit* pInputText /*, *udevMonitor*/;
  QLineEdit *pOutputTextEdit, *pFeatureTextEdit, *pGetFeatureTextEdit;
  QSpinBox *pOutputLenBox, *pFeatureLenBox, *pOutputIdBox, *pFeatureIdBox,
    *pGetFeatureIdBox;
  QLabel* pConnectedLbl;
  QTabWidget *pTabs, *pUdevTabs;

  quint32 mConnectedDevice;
  ushort mVendorId, mProductId, mReleaseNumber;
  QString mManufacturer, mProduct;
  QString mSerialNumber;

  void initBuild();
  QFrame* initInfoFrame();
  QFrame* initDataFrame();
  QFrame* initInputFrame();

private:
};

#endif // MAINWINDOW_H
