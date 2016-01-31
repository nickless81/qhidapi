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
#include <QThread>

#include <QHidApi>

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
    void rescan(ushort mVendorId=0x0, ushort mProductId=0x0);
    int sendOutputReport();
    int sendFeatureReport();
    int getFeatureReport();
    void clear();
    void timeout();

protected:
    QHidApi *pHidApi;
    QList<QHidDeviceInfo> mDevices;
    QHidDeviceInfoModel mModel;
    QTableView *pInfoView;
    QPushButton *pConnectBtn, *pDisconnectBtn, *pSescanBtn, *pClearBtn, *pOutputBtn, *pFeatureBtn, *pGetFeatureBtn;
    QPlainTextEdit *pInputText/*, *udevMonitor*/;
    QLineEdit *pOutputTextEdit, *pFeatureTextEdit, *pGetFeatureTextEdit;
    QSpinBox *pOutputLenBox, *pFeatureLenBox, *pOutputIdBox, *pFeatureIdBox, *pGetFeatureIdBox;
    QLabel *pConnectedLbl;
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
