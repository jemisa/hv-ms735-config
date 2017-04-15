/*
 *      Copyright 2017 Pavel Bludov <pbludov@gmail.com>
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

#include "qhiddevice.h"
#if defined(WITH_HIDAPI) || defined(WITH_HIDAPI_LIBUSB) || defined(WITH_HIDAPI_HIDRAW)
#include "qhiddevice_hidapi.h"
#elif defined(Q_OS_WIN32)
#include "qhiddevice_win32.h"
#endif

#include <QDebug>
#include <QThread>

QHIDDevice::QHIDDevice(int vendorId, int deviceId, int usagePage, QObject *parent)
    : QObject(parent)
    , inputBufferLength(64)
    , outputBufferLength(64)
    , writeDelayValue(20000UL)
    , d_ptr(new QHIDDevicePrivate(this, vendorId, deviceId, usagePage))
{
}

QHIDDevice::~QHIDDevice()
{
    d_ptr->q_ptr = nullptr;
    delete d_ptr;
    d_ptr = nullptr;
}

bool QHIDDevice::open(int vendorId, int deviceId, int usagePage)
{
    d_ptr->q_ptr = nullptr;
    delete d_ptr;
    d_ptr = new QHIDDevicePrivate(this, vendorId, deviceId, usagePage);
    return d_ptr->isValid();
}

bool QHIDDevice::isValid() const
{
    Q_D(const QHIDDevice);
    return d->isValid();
}

int QHIDDevice::sendFeatureReport(const char *report, int length)
{
    Q_D(QHIDDevice);
    auto ret = d->sendFeatureReport(report, length);
    QThread::usleep(writeDelayValue);
    return ret;
}

int QHIDDevice::getFeatureReport(char *report, int length)
{
    Q_D(QHIDDevice);
    return d->getFeatureReport(report, length);
}

int QHIDDevice::write(char report, const char *buffer, int length)
{
    Q_D(QHIDDevice);
    int offset = 0;

    while (length > 0)
    {
        QByteArray chunk;
        chunk.reserve(outputBufferLength + 1);
        chunk.append(report).append(buffer + offset, qMin(length, outputBufferLength));
        auto written = d->write(chunk.cbegin(), chunk.size());

        if (written <= 0)
            return written;

        QThread::usleep(writeDelayValue);
        offset += written - 1;
        length -= written - 1;
    }

    return offset;
}

int QHIDDevice::read(char *buffer, int length)
{
    Q_D(QHIDDevice);
    int offset = 0;

    while (length > 0)
    {
        auto read = d->read(buffer + offset, qMin(length, inputBufferLength));

        if (read <= 0)
            return read;

        offset += read;
        length -= read;
    }

    return offset;
}

unsigned long QHIDDevice::writeDelay() const
{
    return writeDelayValue;
}

void QHIDDevice::setWriteDelay(unsigned long value)
{
    writeDelayValue = value;
}
