/*
    Copyright 2005 Kevin Ottens <ervin@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HALBASICTEST_H
#define HALBASICTEST_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QString>

#include "solid/devices/backends/hal/haldevice.h"

class HalBasicTest : public QObject
{
    Q_OBJECT
public:
    HalBasicTest(QObject *parent = 0);
private Q_SLOTS:
    void initTestCase();
    void testBasic();
    void testProcessorList();
    void testDeviceCreation();
    void testSignalHandling();

    void slotPropertyChanged(const QMap<QString, int> &changes);

private:
    Solid::Backends::Hal::HalDevice *m_device;
};

#endif
