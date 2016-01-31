#include "mainwindow.h"
#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>

//#include "udevmonitor.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    initBuild();

    hidData();
    udevData();
//    udevMonitoring();

    setBaseSize(1000, 900);

}

MainWindow::~MainWindow() {

}

void MainWindow::hidData() {
    api = new QHidApi(this);

    // get all HID devices.
    devices = api->enumerate(0x0, 0x0);    
    m_model.setDataSet(devices);
    infoView->setModel(&m_model);

    connectedDevice = 0;
    // wait for half a second before calling the read function.
    QTimer::singleShot(500, Qt::CoarseTimer, this, SLOT(timeout()));
}

void MainWindow::udevData() {
    struct udev *udev;
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices, *dev_list_entry;
    struct udev_device *dev;

    udev = udev_new();
    if (!udev) {
        printf("Can't create udev\n");
        exit(1);
    }

    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "hidraw");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);

    udev_list_entry_foreach(dev_list_entry, devices) {
        const char *path;
        path = udev_list_entry_get_name(dev_list_entry);
        dev = udev_device_new_from_syspath(udev, path);

        QString s = QString("%1").arg(udev_device_get_devnode(dev));

        QPlainTextEdit *edit = new QPlainTextEdit(this);
        udevTabs->addTab(edit, s);

        dev = udev_device_get_parent_with_subsystem_devtype(
                    dev,
                    "usb",
                    "usb_device");
        if (!dev) {
            s = QString("Unable to find parent usb device.");
            edit->appendPlainText(s);
            exit(1);
        }

        s = QString("VID: %1, PID %2")
                .arg(udev_device_get_sysattr_value(dev,"idVendor"))
                .arg(udev_device_get_sysattr_value(dev,"idProduct"));

        edit->appendPlainText(s);

        s = QString("%1,  %2,  Serial number: %3")
                .arg(udev_device_get_sysattr_value(dev,"manufacturer"))
                .arg(udev_device_get_sysattr_value(dev,"product"))
                .arg(udev_device_get_sysattr_value(dev,"serial"));
        edit->appendPlainText(s);

        s = QString("BCD Device: %1, Class: %2, Subclass: %3, Protocol: %4")
                .arg(udev_device_get_sysattr_value(dev,"bcdDevice"))
                .arg(udev_device_get_sysattr_value(dev,"bDeviceClass"))
                .arg(udev_device_get_sysattr_value(dev,"bDeviceSubClass"))
                .arg(udev_device_get_sysattr_value(dev,"bDeviceProtocol"));
        edit->appendPlainText(s);

        s = QString("Num Configurations: %1, Num Interfaces: %2")
                .arg(udev_device_get_sysattr_value(dev,"bNumConfigurations"))
                .arg(udev_device_get_sysattr_value(dev,"bNumInterfaces"));
        edit->appendPlainText(s);

        s = QString("Bus Num: %1, Dev Path: %2, Dev Num: %3")
                .arg(udev_device_get_sysattr_value(dev,"busnum"))
                .arg(udev_device_get_sysattr_value(dev,"devpath"))
                .arg(udev_device_get_sysattr_value(dev,"devnum"));

        edit->appendPlainText(s);

        s = QString("USB Version: %1, Speed: %2\n")
                .arg(udev_device_get_sysattr_value(dev,"version"))
                .arg(udev_device_get_sysattr_value(dev,"speed"));
        edit->appendPlainText(s);

        s = QString("UEvent:\n%1,")
                .arg(udev_device_get_sysattr_value(dev,"uevent"));
        edit->appendPlainText(s);

        s = QString("\n\n");
        edit->appendPlainText(s);

        udev_device_unref(dev);

    }

    udev_enumerate_unref(enumerate);

    udev_unref(udev);

}

//void MainWindow::udevMonitoring() {
//    UdevMonitor *monitor = new UdevMonitor(udevMonitor,this);
//    /*QThread *thread = */WrappableObject::wrapObjectInThreadAndStart(monitor, this);
//}

void MainWindow::initBuild() {
    setWindowTitle(tr("QHidApi Test Utility"));

    QFrame *mainFrame = new QFrame(this);
    setCentralWidget(mainFrame);
    QGridLayout *mainLayout = new QGridLayout;
    mainFrame->setLayout(mainLayout);

    m_tabs = new QTabWidget(this);
    mainLayout->addWidget(m_tabs, 0, 0);

//    udevMonitor = new QPlainTextEdit(this);
//    m_tabs->addTab(udevMonitor, "UDEV Monitoring");

    QFrame *hidFrame = new QFrame(this);
    QGridLayout *hidLayout = new QGridLayout;
    hidFrame->setLayout(hidLayout);
    m_tabs->addTab(hidFrame, "HID Data");
//    setCentralWidget(frame);

    hidLayout->addWidget(initInfoFrame(), 0, 0);
    hidLayout->addWidget(initDataFrame(), 1, 0);
    hidLayout->addWidget(initInputFrame(), 2, 0);

    QFrame *udevFrame = new QFrame(this);
    QGridLayout *udevLayout = new QGridLayout;
    udevFrame->setLayout(udevLayout);

    udevTabs = new QTabWidget(this);
    udevLayout->addWidget(udevTabs);

    m_tabs->addTab(udevFrame, "UDEV Data");

    QPushButton *closeBtn = new QPushButton(tr("Close"), this);
    mainLayout->addWidget(closeBtn, 1, 0);
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(close()));
}

QFrame* MainWindow::initInputFrame() {
    QFrame *frame = new QFrame(this);
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    frame->setFrameStyle(QFrame::StyledPanel);

    QGridLayout *layout = new QGridLayout;
    frame->setLayout(layout);

    layout->addWidget(new QLabel(tr("Data received from the device appears in the Input section."), this), 0, 0);

    inputText = new QPlainTextEdit(this);
    inputText->setCenterOnScroll(true);
    inputText->setWordWrapMode(QTextOption::NoWrap);
    layout->addWidget(inputText, 1, 0);

    clearBtn = new QPushButton(tr("Clear"), this);
    layout->addWidget(clearBtn, 1, 1);
    connect(clearBtn, SIGNAL(clicked()), this, SLOT(clear()));

    return frame;
}

QFrame* MainWindow::initDataFrame() {
    QFrame* frame = new QFrame(this);
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    frame->setFrameStyle(QFrame::StyledPanel);
    QGridLayout *layout = new QGridLayout;
    frame->setLayout(layout);

    layout->addWidget(new QLabel(tr("Data"), this), 0, 0, 1, 1, Qt::AlignHCenter);
    layout->addWidget(new QLabel(tr("Length"), this), 0, 1, 1, 1, Qt::AlignHCenter);
    layout->addWidget(new QLabel(tr("Rep. Number"), this), 0, 2, 1, 1, Qt::AlignHCenter);

    outputTextEdit = new QLineEdit(this);
    outputTextEdit->setText("OUTPUT");
    layout->addWidget(outputTextEdit, 1, 0);

    outputLenBox = new QSpinBox(this);
    outputLenBox->setMinimum(0);
    outputLenBox->setMaximum(255);
    layout->addWidget(outputLenBox, 1, 1);

    outputIdBox = new QSpinBox(this);
    outputIdBox->setMinimum(0);
    outputIdBox->setMaximum(0xFF);
    outputIdBox->setDisplayIntegerBase(16);
    outputIdBox->setPrefix("0x");
    layout->addWidget(outputIdBox, 1, 2);

    outputBtn = new QPushButton(tr("Output"), this);
    outputBtn->setEnabled(false);
    layout->addWidget(outputBtn, 1, 3);
    connect(outputBtn, SIGNAL(clicked()), this, SLOT(sendOutputReport()));

    featureTextEdit = new QLineEdit(this);
    featureTextEdit->setText("FEATURE");
    layout->addWidget(featureTextEdit, 2, 0);

    featureLenBox = new QSpinBox(this);
    featureLenBox->setMinimum(0);
    featureLenBox->setMaximum(64);
    layout->addWidget(featureLenBox, 2, 1);

    featureIdBox = new QSpinBox(this);
    featureIdBox->setMinimum(0);
    featureIdBox->setMaximum(0xFF);
    featureIdBox->setDisplayIntegerBase(16);
    featureIdBox->setPrefix("0x");
    layout->addWidget(featureIdBox, 2, 2);

    featureBtn = new QPushButton(tr("Feature"), this);
    featureBtn->setEnabled(false);
    layout->addWidget(featureBtn, 2, 3);
    connect(featureBtn, SIGNAL(clicked()), this, SLOT(sendFeatureReport()));

    getFeatureTextEdit = new QLineEdit(this);
    getFeatureTextEdit->setText("GET FEATURE");
    layout->addWidget(getFeatureTextEdit, 3, 0);

    getFeatureIdBox = new QSpinBox(this);
    getFeatureIdBox->setMinimum(0);
    getFeatureIdBox->setMaximum(0xFF);
    getFeatureIdBox->setDisplayIntegerBase(16);
    getFeatureIdBox->setPrefix("0x");
    layout->addWidget(getFeatureIdBox, 3, 2);

    getFeatureBtn = new QPushButton(tr("Get feature"), this);
    getFeatureBtn->setEnabled(false);
    layout->addWidget(getFeatureBtn, 3, 3);
    connect(getFeatureBtn, SIGNAL(clicked()), this, SLOT(getFeatureReport()));

    QLabel *lbl1 = new QLabel(tr("Output data bytes can be entered in the Output section, "
                                 "separated by space, comma or brackets. Data starting with "
                                 "0x is treated as hex. Data beginning with a 0 is treated "
                                 "as octal. All other data is treated as decimal."), this);
    lbl1->setWordWrap(true);
    layout->addWidget(lbl1 ,4, 0);

    QLabel *lbl2 = new QLabel(tr("Optionally, a report length may be specified. Extra bytes "
                                 "are padded with zeros. If no length is specified, the length "
                                 "is inferred from the data."), this);
    lbl2->setWordWrap(true);
    layout->addWidget(lbl2, 4, 1);

    return frame;
}

QFrame* MainWindow::initInfoFrame() {
    QFrame *frame = new QFrame(this);
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    frame->setFrameStyle(QFrame::StyledPanel);
    QGridLayout *layout = new QGridLayout;
    frame->setLayout(layout);

    QLabel *lbl1 = new QLabel(tr("Select a device and press Connect."), this);
    layout->addWidget(lbl1, 0, 0, 1, 2);

    infoView = new QTableView(this);
    infoView->horizontalHeader()->setStretchLastSection(true);
    infoView->setSelectionMode(QTableView::SingleSelection);
    infoView->setSelectionBehavior(QTableView::SelectRows);
    layout->addWidget(infoView, 1, 0, 3, 1);

    connectBtn = new QPushButton(tr("Connect"), this);
    layout->addWidget(connectBtn, 1, 1);
    connect(connectBtn, SIGNAL(clicked()), this, SLOT(connectDevice()));

    disconnectBtn = new QPushButton(tr("Disconnect"), this);
    disconnectBtn->setEnabled(false);
    layout->addWidget(disconnectBtn, 2, 1);
    connect(disconnectBtn, SIGNAL(clicked()), this, SLOT(disconnectDevice()));

    rescanBtn = new QPushButton(tr("Rescan"), this);
    layout->addWidget(rescanBtn, 3, 1);
    connect(rescanBtn, SIGNAL(clicked()), this, SLOT(rescan()));

    connectedLbl = new QLabel(tr("Disconnected"), this);
    layout->addWidget(connectedLbl, 4, 0, 1, 2);

    return frame;
}

/*
 * Opens the selected device and sets the connectedDevice to the id of this device.
 */
void MainWindow::connectDevice() {
    if (connectedDevice > 0) return;

    QModelIndex selRow = infoView->currentIndex(), index;
    QVariant vId = m_model.index(selRow.row(), 0).data();
    QVariant m = m_model.index(selRow.row(), 1).data();
    QVariant pId = m_model.index(selRow.row(), 2).data();
    QVariant p = m_model.index(selRow.row(), 3).data();
    QVariant sn = m_model.index(selRow.row(), 4).data();
    QVariant rn = m_model.index(selRow.row(), 5).data();

    quint32 id = api->open(vId.toUInt(), pId.toUInt(), sn.toString());
    if (id > 0) {
        connectedDevice = id;
        vendorId = vId.toUInt();
        manufacturer = m.toString();
        productId = pId.toUInt();
        product = p.toString();
        serialNumber = sn.toString();
        releaseNumber = rn.toUInt();
        api->setNonBlocking(id);
        QString s = QString(tr("Connected to Vendor:0x%1, Product:%2, 0x%3, %4"))
                .arg(QString::number(vendorId, 16))
                .arg(manufacturer)
                .arg(QString::number(productId, 16))
                .arg(product);
        connectedLbl->setText(s);

        connectBtn->setEnabled(false);
        disconnectBtn->setEnabled(true);
        outputBtn->setEnabled(true);
        featureBtn->setEnabled(true);
        getFeatureBtn->setEnabled(true);
        inputText->clear();

    } else {
        QMessageBox::warning(this, tr("Open Failed"), tr("Unable to open device!"), QMessageBox::Ok);
    }
}

void MainWindow::disconnectDevice() {
    if (connectedDevice == 0) return;

    api->close(connectedDevice);
    connectedDevice = 0;
    connectedLbl->setText(tr("Disconnected"));
    outputBtn->setEnabled(false);
    featureBtn->setEnabled(false);
    getFeatureBtn->setEnabled(false);
    connectBtn->setEnabled(false);
    disconnectBtn->setEnabled(false);

}

void MainWindow::rescan(ushort vendorId, ushort productId) {
    QList<QHidDeviceInfo> list = api->enumerate(vendorId, productId);
    m_model.setDataSet(list);
}

int MainWindow::sendOutputReport() {

    quint8 reportNumber = outputIdBox->value();

    QByteArray data;
    data.append(reportNumber);

    QByteArray text = outputTextEdit->text().toLatin1();
    int len = outputLenBox->value();
    if (len == 0)
        len = text.length();


    int written = api->write(connectedDevice, data);

    if (written < 0) {
        QString s(tr("Could not write to device. Error reported was: %1"));
        s = s.arg(api->error(connectedDevice));
        QMessageBox::warning(this, tr("Error Writing"), s, QMessageBox::Ok);
    }

    return written;
}

int MainWindow::sendFeatureReport() {
    quint8 reportNumber = featureIdBox->value();

    QByteArray data;
    data.append(reportNumber);

    QByteArray text = featureTextEdit->text().toLatin1();
    int len = featureLenBox->value();
    if (len == 0)
        len = text.length();


    int written = api->sendFeatureReport(connectedDevice, reportNumber, data);

    if (written < 0) {
        QString s(tr("Could not send feature report to device. Error reported was: %1"));
        s = s.arg(api->error(connectedDevice));
        QMessageBox::warning(this, tr("Error Writing"), s, QMessageBox::Ok);
    }

    return written;
}

int MainWindow::getFeatureReport() {
    quint8 reportNumber = getFeatureIdBox->value();

    QByteArray data = api->featureReport(connectedDevice, reportNumber);

    if (data.isEmpty()) {
        QString s(tr("Could not get feature report from device. Error reported was: %1"));
        s = s.arg(api->error(connectedDevice));
        QMessageBox::warning(this, tr("Error Writing"), s, QMessageBox::Ok);
    }

    return data.length();
}

void MainWindow::clear() {
    inputText->clear();
}

void MainWindow::timeout() {

    if (connectedDevice > 0) {
        QByteArray data = api->read(connectedDevice);

        if (!data.isEmpty()) {
            QString s = tr("Received %1 bytes:\n").arg(data.length());
            inputText->appendPlainText(s);
            inputText->ensureCursorVisible();
        }
    }

    QTimer::singleShot(5, Qt::CoarseTimer, this, SLOT(timeout()));

}
