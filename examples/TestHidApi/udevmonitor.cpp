#include <QPlainTextEdit>
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

#include "udevmonitor.h"
#include <libudev.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

UdevMonitor::UdevMonitor(QPlainTextEdit* edit, QObject* parent)
  : QObject(parent)
{
  struct udev* udev;
  struct udev_device* dev;
  struct udev_monitor* mon;
  int fd;

  udev = udev_new();
  if (!udev) {
    printf("Can't create udev\n");
    exit(1);
  }

  /* Set up a monitor to monitor hidraw devices */
  mon = udev_monitor_new_from_netlink(udev, "udev");
  udev_monitor_filter_add_match_subsystem_devtype(mon, "hidraw", NULL);
  udev_monitor_enable_receiving(mon);
  /* Get the file descriptor (fd) for the monitor.
     This fd will get passed to select() */
  fd = udev_monitor_get_fd(mon);

  /* This section will run continuously, calling usleep() at
     the end of each pass. This is to demonstrate how to use
     a udev_monitor in a non-blocking way. */
  while (1) {
    /* Set up the call to select(). In this case, select() will
       only operate on a single file descriptor, the one
       associated with our udev_monitor. Note that the timeval
       object is set to 0, which will cause select() to not
       block. */
    fd_set fds;
    struct timeval tv;
    int ret;

    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    QString s;

    ret = select(fd + 1, &fds, NULL, NULL, &tv);

    /* Check if our file descriptor has received data. */
    if (ret > 0 && FD_ISSET(fd, &fds)) {

      s = QString("select() says there should be data");
      edit->appendPlainText(s);

      /* Make the call to receive the device.
         select() ensured that this will not block. */
      dev = udev_monitor_receive_device(mon);
      if (dev) {
        s = QString("Got Device");
        edit->appendPlainText(s);

        s = QString(" Node: %1").arg(udev_device_get_devnode(dev));
        edit->appendPlainText(s);

        s = QString(" Subsystem: %1").arg(udev_device_get_subsystem(dev));
        edit->appendPlainText(s);

        s = QString(" Devtype: %1").arg(udev_device_get_devtype(dev));
        edit->appendPlainText(s);

        s = QString(" Action: %1").arg(udev_device_get_action(dev));
        edit->appendPlainText(s);

        udev_device_unref(dev);
      } else {
        s = QString(" No Device from receive_device(). An error occured.");
        edit->appendPlainText(s);
      }
    }
    usleep(250 * 1000);
    fflush(stdout);
  }
}
