/*
    Copyright 2014 Alejandro Fiestas Olivares <afiestas@kde.org>

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

#include "fakeUpower.h"

#include <QDebug>
#include <QDBusPendingCall>
#include <QDBusConnection>
#include <qdbusmessage.h>

FakeUpower::FakeUpower(QObject* parent) : QObject(parent),
m_onBattery(false)
{

}

QString FakeUpower::daemonVersion() const
{
    return "POP";
}

QString FakeUpower::GetCriticalAction()
{
    return "AAAA";
}

QDBusObjectPath FakeUpower::GetDisplayDevice()
{
    return QDBusObjectPath("AA");
}

bool FakeUpower::isDocked() const
{
    return true;
}

bool FakeUpower::lidIsClosed() const
{
    return false;
}

bool FakeUpower::lidIsPresent() const
{
    return true;
}

bool FakeUpower::onBattery() const
{
    return m_onBattery;
}

void FakeUpower::setOnBattery(bool onBattery)
{
    m_onBattery = onBattery;

    emitPropertiesChanged(QStringLiteral("OnBattery"), m_onBattery);
}

void FakeUpower::emitPropertiesChanged(const QString& name, const QVariant& value)
{
    auto msg = QDBusMessage::createSignal(
        QStringLiteral("/org/freedesktop/UPower"),
        QStringLiteral("org.freedesktop.DBus.Properties"),
        QStringLiteral("PropertiesChanged"));

    QVariantMap map;
    map.insert(name, value);
    QList<QVariant> args;
    args << QString("org.freedesktop.UPower");
    args << map;
    args << QStringList();

    msg.setArguments(args);

    QDBusConnection::systemBus().asyncCall(msg);
}

QList< QDBusObjectPath > FakeUpower::EnumerateDevices()
{
    QList<QDBusObjectPath> list;
    return list;
}
