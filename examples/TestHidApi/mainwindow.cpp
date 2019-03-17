#include "mainwindow.h"
#include <libudev.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

//#include "udevmonitor.h"

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{

  initBuild();

  hidData();
  udevData();
  //    udevMonitoring();

  setBaseSize(1000, 900);
}

MainWindow::~MainWindow() {}

void
MainWindow::hidData()
{
  pHidApi = new QHidApi(this);

  // get all HID devices.
  mDevices = pHidApi->enumerate(0x0, 0x0);
  mModel.setDataSet(mDevices);
  pInfoView->setModel(&mModel);

  mConnectedDevice = 0;
  // wait for half a second before calling the read function.
  QTimer::singleShot(500, Qt::CoarseTimer, this, SLOT(timeout()));
}

void
MainWindow::udevData()
{
  struct udev* udev;
  struct udev_enumerate* enumerate;
  struct udev_list_entry *devices, *dev_list_entry;
  struct udev_device* dev;

  udev = udev_new();
  if (!udev) {
    printf("Can't create udev\n");
    exit(1);
  }

  enumerate = udev_enumerate_new(udev);
  udev_enumerate_add_match_subsystem(enumerate, "hidraw");
  udev_enumerate_scan_devices(enumerate);
  devices = udev_enumerate_get_list_entry(enumerate);

  udev_list_entry_foreach(dev_list_entry, devices)
  {
    const char* path;
    path = udev_list_entry_get_name(dev_list_entry);
    dev = udev_device_new_from_syspath(udev, path);

    QString s = QString("%1").arg(udev_device_get_devnode(dev));

    QPlainTextEdit* edit = new QPlainTextEdit(this);
    pUdevTabs->addTab(edit, s);

    dev =
      udev_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device");
    if (!dev) {
      s = QString("Unable to find parent usb device.");
      edit->appendPlainText(s);
      exit(1);
    }

    s = QString("VID: %1, PID %2")
          .arg(udev_device_get_sysattr_value(dev, "idVendor"))
          .arg(udev_device_get_sysattr_value(dev, "idProduct"));

    edit->appendPlainText(s);

    s = QString("%1,  %2,  Serial number: %3")
          .arg(udev_device_get_sysattr_value(dev, "manufacturer"))
          .arg(udev_device_get_sysattr_value(dev, "product"))
          .arg(udev_device_get_sysattr_value(dev, "serial"));
    edit->appendPlainText(s);

    s = QString("BCD Device: %1, Class: %2, Subclass: %3, Protocol: %4")
          .arg(udev_device_get_sysattr_value(dev, "bcdDevice"))
          .arg(udev_device_get_sysattr_value(dev, "bDeviceClass"))
          .arg(udev_device_get_sysattr_value(dev, "bDeviceSubClass"))
          .arg(udev_device_get_sysattr_value(dev, "bDeviceProtocol"));
    edit->appendPlainText(s);

    s = QString("Num Configurations: %1, Num Interfaces: %2")
          .arg(udev_device_get_sysattr_value(dev, "bNumConfigurations"))
          .arg(udev_device_get_sysattr_value(dev, "bNumInterfaces"));
    edit->appendPlainText(s);

    s = QString("Bus Num: %1, Dev Path: %2, Dev Num: %3")
          .arg(udev_device_get_sysattr_value(dev, "busnum"))
          .arg(udev_device_get_sysattr_value(dev, "devpath"))
          .arg(udev_device_get_sysattr_value(dev, "devnum"));

    edit->appendPlainText(s);

    s = QString("USB Version: %1, Speed: %2\n")
          .arg(udev_device_get_sysattr_value(dev, "version"))
          .arg(udev_device_get_sysattr_value(dev, "speed"));
    edit->appendPlainText(s);

    s =
      QString("UEvent:\n%1,").arg(udev_device_get_sysattr_value(dev, "uevent"));
    edit->appendPlainText(s);

    s = QString("\n\n");
    edit->appendPlainText(s);

    udev_device_unref(dev);
  }

  udev_enumerate_unref(enumerate);

  udev_unref(udev);
}

// void MainWindow::udevMonitoring() {
//    UdevMonitor *monitor = new UdevMonitor(udevMonitor,this);
//    QThread *thread = new QThread(this);
//    monitor->moveToThread(thread);
//}

void
MainWindow::initBuild()
{
  setWindowTitle(tr("QHidApi Test Utility"));

  QFrame* mainFrame = new QFrame(this);
  setCentralWidget(mainFrame);
  QGridLayout* mainLayout = new QGridLayout;
  mainFrame->setLayout(mainLayout);

  pTabs = new QTabWidget(this);
  mainLayout->addWidget(pTabs, 0, 0);

  //    udevMonitor = new QPlainTextEdit(this);
  //    m_tabs->addTab(udevMonitor, "UDEV Monitoring");

  QFrame* hidFrame = new QFrame(this);
  QGridLayout* hidLayout = new QGridLayout;
  hidFrame->setLayout(hidLayout);
  pTabs->addTab(hidFrame, "HID Data");
  //    setCentralWidget(frame);

  hidLayout->addWidget(initInfoFrame(), 0, 0);
  hidLayout->addWidget(initDataFrame(), 1, 0);
  hidLayout->addWidget(initInputFrame(), 2, 0);

  QFrame* udevFrame = new QFrame(this);
  QGridLayout* udevLayout = new QGridLayout;
  udevFrame->setLayout(udevLayout);

  pUdevTabs = new QTabWidget(this);
  udevLayout->addWidget(pUdevTabs);

  pTabs->addTab(udevFrame, "UDEV Data");

  QPushButton* closeBtn = new QPushButton(tr("Close"), this);
  mainLayout->addWidget(closeBtn, 1, 0);
  connect(closeBtn, SIGNAL(clicked()), this, SLOT(close()));
}

QFrame*
MainWindow::initInputFrame()
{
  QFrame* frame = new QFrame(this);
  frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  frame->setFrameStyle(QFrame::StyledPanel);

  QGridLayout* layout = new QGridLayout;
  frame->setLayout(layout);

  layout->addWidget(
    new QLabel(
      tr("Data received from the device appears in the Input section."), this),
    0,
    0);

  pInputText = new QPlainTextEdit(this);
  pInputText->setCenterOnScroll(true);
  pInputText->setWordWrapMode(QTextOption::NoWrap);
  layout->addWidget(pInputText, 1, 0);

  pClearBtn = new QPushButton(tr("Clear"), this);
  layout->addWidget(pClearBtn, 1, 1);
  connect(pClearBtn, SIGNAL(clicked()), this, SLOT(clear()));

  return frame;
}

QFrame*
MainWindow::initDataFrame()
{
  QFrame* frame = new QFrame(this);
  frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  frame->setFrameStyle(QFrame::StyledPanel);
  QGridLayout* layout = new QGridLayout;
  frame->setLayout(layout);

  layout->addWidget(new QLabel(tr("Data"), this), 0, 0, 1, 1, Qt::AlignHCenter);
  layout->addWidget(
    new QLabel(tr("Length"), this), 0, 1, 1, 1, Qt::AlignHCenter);
  layout->addWidget(
    new QLabel(tr("Rep. Number"), this), 0, 2, 1, 1, Qt::AlignHCenter);

  pOutputTextEdit = new QLineEdit(this);
  pOutputTextEdit->setText("OUTPUT");
  layout->addWidget(pOutputTextEdit, 1, 0);

  pOutputLenBox = new QSpinBox(this);
  pOutputLenBox->setMinimum(0);
  pOutputLenBox->setMaximum(255);
  layout->addWidget(pOutputLenBox, 1, 1);

  pOutputIdBox = new QSpinBox(this);
  pOutputIdBox->setMinimum(0);
  pOutputIdBox->setMaximum(0xFF);
  pOutputIdBox->setDisplayIntegerBase(16);
  pOutputIdBox->setPrefix("0x");
  layout->addWidget(pOutputIdBox, 1, 2);

  pOutputBtn = new QPushButton(tr("Output"), this);
  pOutputBtn->setEnabled(false);
  layout->addWidget(pOutputBtn, 1, 3);
  connect(pOutputBtn, SIGNAL(clicked()), this, SLOT(sendOutputReport()));

  pFeatureTextEdit = new QLineEdit(this);
  pFeatureTextEdit->setText("FEATURE");
  layout->addWidget(pFeatureTextEdit, 2, 0);

  pFeatureLenBox = new QSpinBox(this);
  pFeatureLenBox->setMinimum(0);
  pFeatureLenBox->setMaximum(64);
  layout->addWidget(pFeatureLenBox, 2, 1);

  pFeatureIdBox = new QSpinBox(this);
  pFeatureIdBox->setMinimum(0);
  pFeatureIdBox->setMaximum(0xFF);
  pFeatureIdBox->setDisplayIntegerBase(16);
  pFeatureIdBox->setPrefix("0x");
  layout->addWidget(pFeatureIdBox, 2, 2);

  pFeatureBtn = new QPushButton(tr("Feature"), this);
  pFeatureBtn->setEnabled(false);
  layout->addWidget(pFeatureBtn, 2, 3);
  connect(pFeatureBtn, SIGNAL(clicked()), this, SLOT(sendFeatureReport()));

  pGetFeatureTextEdit = new QLineEdit(this);
  pGetFeatureTextEdit->setText("GET FEATURE");
  layout->addWidget(pGetFeatureTextEdit, 3, 0);

  pGetFeatureIdBox = new QSpinBox(this);
  pGetFeatureIdBox->setMinimum(0);
  pGetFeatureIdBox->setMaximum(0xFF);
  pGetFeatureIdBox->setDisplayIntegerBase(16);
  pGetFeatureIdBox->setPrefix("0x");
  layout->addWidget(pGetFeatureIdBox, 3, 2);

  pGetFeatureBtn = new QPushButton(tr("Get feature"), this);
  pGetFeatureBtn->setEnabled(false);
  layout->addWidget(pGetFeatureBtn, 3, 3);
  connect(pGetFeatureBtn, SIGNAL(clicked()), this, SLOT(getFeatureReport()));

  QLabel* lbl1 =
    new QLabel(tr("Output data bytes can be entered in the Output section, "
                  "separated by space, comma or brackets. Data starting with "
                  "0x is treated as hex. Data beginning with a 0 is treated "
                  "as octal. All other data is treated as decimal."),
               this);
  lbl1->setWordWrap(true);
  layout->addWidget(lbl1, 4, 0);

  QLabel* lbl2 = new QLabel(
    tr("Optionally, a report length may be specified. Extra bytes "
       "are padded with zeros. If no length is specified, the length "
       "is inferred from the data."),
    this);
  lbl2->setWordWrap(true);
  layout->addWidget(lbl2, 4, 1);

  return frame;
}

QFrame*
MainWindow::initInfoFrame()
{
  QFrame* frame = new QFrame(this);
  frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  frame->setFrameStyle(QFrame::StyledPanel);
  QGridLayout* layout = new QGridLayout;
  frame->setLayout(layout);

  QLabel* lbl1 = new QLabel(tr("Select a device and press Connect."), this);
  layout->addWidget(lbl1, 0, 0, 1, 2);

  pInfoView = new QTableView(this);
  pInfoView->horizontalHeader()->setStretchLastSection(true);
  pInfoView->setSelectionMode(QTableView::SingleSelection);
  pInfoView->setSelectionBehavior(QTableView::SelectRows);
  layout->addWidget(pInfoView, 1, 0, 3, 1);

  pConnectBtn = new QPushButton(tr("Connect"), this);
  layout->addWidget(pConnectBtn, 1, 1);
  connect(pConnectBtn, SIGNAL(clicked()), this, SLOT(connectDevice()));

  pDisconnectBtn = new QPushButton(tr("Disconnect"), this);
  pDisconnectBtn->setEnabled(false);
  layout->addWidget(pDisconnectBtn, 2, 1);
  connect(pDisconnectBtn, SIGNAL(clicked()), this, SLOT(disconnectDevice()));

  pSescanBtn = new QPushButton(tr("Rescan"), this);
  layout->addWidget(pSescanBtn, 3, 1);
  connect(pSescanBtn, SIGNAL(clicked()), this, SLOT(rescan()));

  pConnectedLbl = new QLabel(tr("Disconnected"), this);
  layout->addWidget(pConnectedLbl, 4, 0, 1, 2);

  return frame;
}

/*
 * Opens the selected device and sets the connectedDevice to the id of this
 * device.
 */
void
MainWindow::connectDevice()
{
  if (mConnectedDevice > 0)
    return;

  QModelIndex selRow = pInfoView->currentIndex(), index;
  QVariant vId = mModel.index(selRow.row(), 0).data();
  QVariant m = mModel.index(selRow.row(), 1).data();
  QVariant pId = mModel.index(selRow.row(), 2).data();
  QVariant p = mModel.index(selRow.row(), 3).data();
  QVariant sn = mModel.index(selRow.row(), 4).data();
  QVariant rn = mModel.index(selRow.row(), 5).data();

  quint32 id = pHidApi->open(vId.toUInt(), pId.toUInt(), sn.toString());
  if (id > 0) {
    mConnectedDevice = id;
    mVendorId = vId.toUInt();
    mManufacturer = m.toString();
    mProductId = pId.toUInt();
    mProduct = p.toString();
    mSerialNumber = sn.toString();
    mReleaseNumber = rn.toUInt();
    pHidApi->setNonBlocking(id);
    QString s = QString(tr("Connected to Vendor:0x%1, Product:%2, 0x%3, %4"))
                  .arg(QString::number(mVendorId, 16))
                  .arg(mManufacturer)
                  .arg(QString::number(mProductId, 16))
                  .arg(mProduct);
    pConnectedLbl->setText(s);

    pConnectBtn->setEnabled(false);
    pDisconnectBtn->setEnabled(true);
    pOutputBtn->setEnabled(true);
    pFeatureBtn->setEnabled(true);
    pGetFeatureBtn->setEnabled(true);
    pInputText->clear();

  } else {
    QMessageBox::warning(
      this, tr("Open Failed"), tr("Unable to open device!"), QMessageBox::Ok);
  }
}

void
MainWindow::disconnectDevice()
{
  if (mConnectedDevice == 0)
    return;

  pHidApi->close(mConnectedDevice);
  mConnectedDevice = 0;
  pConnectedLbl->setText(tr("Disconnected"));
  pOutputBtn->setEnabled(false);
  pFeatureBtn->setEnabled(false);
  pGetFeatureBtn->setEnabled(false);
  pConnectBtn->setEnabled(false);
  pDisconnectBtn->setEnabled(false);
}

void
MainWindow::rescan(ushort vendorId, ushort productId)
{
  QList<QHidDeviceInfo> list = pHidApi->enumerate(vendorId, productId);
  mModel.setDataSet(list);
}

int
MainWindow::sendOutputReport()
{

  quint8 reportNumber = pOutputIdBox->value();

  QByteArray data;
  data.append(reportNumber);

  QByteArray text = pOutputTextEdit->text().toLatin1();
  int len = pOutputLenBox->value();
  if (len == 0)
    len = text.length();

  int written = pHidApi->write(mConnectedDevice, data);

  if (written < 0) {
    QString s(tr("Could not write to device. Error reported was: %1"));
    s = s.arg(pHidApi->error(mConnectedDevice));
    QMessageBox::warning(this, tr("Error Writing"), s, QMessageBox::Ok);
  }

  return written;
}

int
MainWindow::sendFeatureReport()
{
  quint8 reportNumber = pFeatureIdBox->value();

  QByteArray data;
  data.append(reportNumber);

  QByteArray text = pFeatureTextEdit->text().toLatin1();
  int len = pFeatureLenBox->value();
  if (len == 0)
    len = text.length();

  int written =
    pHidApi->sendFeatureReport(mConnectedDevice, reportNumber, data);

  if (written < 0) {
    QString s(
      tr("Could not send feature report to device. Error reported was: %1"));
    s = s.arg(pHidApi->error(mConnectedDevice));
    QMessageBox::warning(this, tr("Error Writing"), s, QMessageBox::Ok);
  }

  return written;
}

int
MainWindow::getFeatureReport()
{
  quint8 reportNumber = pGetFeatureIdBox->value();

  QByteArray data = pHidApi->featureReport(mConnectedDevice, reportNumber);

  if (data.isEmpty()) {
    QString s(
      tr("Could not get feature report from device. Error reported was: %1"));
    s = s.arg(pHidApi->error(mConnectedDevice));
    QMessageBox::warning(this, tr("Error Writing"), s, QMessageBox::Ok);
  }

  return data.length();
}

void
MainWindow::clear()
{
  pInputText->clear();
}

void
MainWindow::timeout()
{

  if (mConnectedDevice > 0) {
    QByteArray data = pHidApi->read(mConnectedDevice);

    if (!data.isEmpty()) {
      QString s = tr("Received %1 bytes:\n").arg(data.length());
      pInputText->appendPlainText(s);
      pInputText->ensureCursorVisible();
    }
  }

  QTimer::singleShot(5, Qt::CoarseTimer, this, SLOT(timeout()));
}
