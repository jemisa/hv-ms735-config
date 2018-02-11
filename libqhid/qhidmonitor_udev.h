/*
 *      Copyright 2017-2018 Pavel Bludov <pbludov@gmail.com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License along
 *      with this program; if not, write to the Free Software Foundation, Inc.,
 *      51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef QHIDMONITOR_UDEV_H
#define QHIDMONITOR_UDEV_H

#include <QObject>

class QHIDMonitor;
class QHIDMonitorPrivate : public QObject
{
    int vendorId;
    int deviceId;

    Q_OBJECT
    Q_DECLARE_PUBLIC(QHIDMonitor)

public:
    QHIDMonitorPrivate(QHIDMonitor *q_ptr, int vendorId, int deviceId);
    ~QHIDMonitorPrivate();

private slots:
    void udevDataAvailable();

private:
    struct udev_monitor *monitor;
    class QSocketNotifier *monitorNotifier;
    class QHIDMonitor *q_ptr;
};

#endif // QHIDMONITOR_UDEV_H
