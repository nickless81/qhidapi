#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QFrame>
#include <QGridLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QSpinBox>
#include <QByteArray>
#include <QTimer>
#include <QScrollArea>
#include <QTabWidget>

#include <qhidapi.h>
#include <qhiddeviceinfomodel.h>

class QHidDeviceInfoModel;
struct udev;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

public slots:

protected slots:
    void udevData();
    void hidData();
//    void udevMonitoring();
    void connectDevice();
    void disconnectDevice();
    void rescan(ushort vendorId=0x0, ushort productId=0x0);
    int sendOutputReport();
    int sendFeatureReport();
    int getFeatureReport();
    void clear();
    void timeout();

protected:
    QHidApi *api;
    QList<QHidDeviceInfo> devices;
    QHidDeviceInfoModel m_model;
    QTableView *infoView;
    QPushButton *connectBtn, *disconnectBtn, *rescanBtn, *clearBtn, *outputBtn, *featureBtn, *getFeatureBtn;
    QPlainTextEdit *inputText/*, *udevMonitor*/;
    QLineEdit *outputTextEdit, *featureTextEdit, *getFeatureTextEdit;
    QSpinBox *outputLenBox, *featureLenBox, *outputIdBox, *featureIdBox, *getFeatureIdBox;
    QLabel *connectedLbl;
    QTabWidget *m_tabs, *udevTabs;

    quint32 connectedDevice;
    ushort vendorId, productId, releaseNumber;
    QString manufacturer, product;
    QString serialNumber;

    void initBuild();
    QFrame* initInfoFrame();
    QFrame* initDataFrame();
    QFrame* initInputFrame();

private:

};

#endif // MAINWINDOW_H
