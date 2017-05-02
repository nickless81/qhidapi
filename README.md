		QHidApi Library
		===============
		
About
=====

QHidApi is a wrapper library for the HidApi library from Signal11. This doesn't
add much functionality to the original library, it primarily only allows the use 
of QString and QVariant for data transfers.

The only major difference is that all USB access is via quint32 id numbers 
rather than via hid_device structures. All of the functionality of the underlying
hidapi classes are available via QHidApi methods.

License
=======You may use, distribute and copy the files under the terms of
GNU General Public License version 3 (see the file gpl3.txt for details).

This code is just a wrapper around Signal11's hidapi which is also available
under the GPLV3 license.

Normal stuff. It's free, use it or modify it as you want but if your PC explodes
you are on your own.

Getting Started
===============

Usage(1): Use QHidApi as Qt5's addon module :
Building the module 

* Download the source code.
* Put the source code in any directory you like
* Go to top directory of the project in a terminal and run

```
    qmake
    make
    make install (or sudo make install in linux if you are not running as administrator)


    make html_docs can be used to generate documentations of the library
    
```
Using the module

* Add following line to your qmake's project file:

```
    QT += hidapi
```

Usage(2): Use source code directly

The package contains a **hidapi.pri** file that allows you to integrate the component into applications that use qmake for the build step.

* Download the source code.

* Put the source code in any directory you like. For example, 3rdparty:

```
    |-- project.pro
    |-- ....
    |-- 3rdparty\
    |     |-- qtxlsx\
    |     |
```

* Add following line to your qmake project file:

```
    include(3rdparty/qhidapi/src/hidapi/hidapi.pri)
```

> **Note**: If you like, you can copy all files from *src/hidapi* to your application's source path. Then add following line to your project file:

> ```
    include(hidapi.pri)
```

> **Note**: If you do not use qmake, you need to define the following macro manually

> ```
    HIDAPI_NO_LIB
```


* Then, using Qt Xlsx in your code

```
	#include <QHidApi>
	#include <QByteArray>
	
	class USBDevice(QObject *parent) : public QThread {
	public:
		USBDevice(QObject *parent=0);
		~USBDevice();
		
	protected:
		QHidApi *pHidApi;
		quint32 mId;
		QByteArray mReadData;
	}
	
	
    
    USBDevice::USBDevice(QObject *parent) : QThread(parent) {
    
		uint vendor = 0x04D8;
		uint product = 0xF1FA;
		QString serial = QString("1.0.0");
		
                pHidApi = new QHidApi(this);
                mId = pHidApi->open(vendor, product, serial); // TODO id's in config ??
		if (mId) {
			pHidApi->setNonBlocking(mId); // set the read() to be non-blocking.	
			return -1;
		}
		
	}
	
	USBDevice::~USBDevice() {
		pHidApi->close(mId);
	}
	
	void USBDevice::run() {
		while (true) {
                    mReadData = pHidApi->read(mId, timeout);
                    if (!mReadData.isEmpty())
                        handleReadyRead();
                    msleep(100);
                }
        }

	
	void USBDevice::sendData(QByteArray request) {
		pHidApi->write(mId, request);
	}

	void USBDevice::handleReadyRead() {
		// do something with the data
	}
 ```
