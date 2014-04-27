/*
 *   Copyright (C) 2013 Ivan Cukic <ivan.cukic(at)kde.org>
 *   Copyright (C) 2014 Kai Uwe Broulik <kde@privat.broulik.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "declarativedevices.h"
#include "declarativedevices_p.h"

#include <solid/device.h>
#include <solid/deviceinterface.h>
#include <solid/devicenotifier.h>
#include <solid/genericinterface.h>

namespace Solid
{

// Maps queries to the handler objects
QHash<QString, QWeakPointer<DevicesQueryPrivate> > DevicesQueryPrivate::handlers;

QSharedPointer<DevicesQueryPrivate> DevicesQueryPrivate::forQuery(const QString &query)
{
    if (handlers.contains(query)) {
        return handlers[query].toStrongRef();
    }

    // Creating a new shared backend instance
    QSharedPointer<DevicesQueryPrivate> backend(new DevicesQueryPrivate(query));

    // Storing a weak pointer to the backend
    handlers[query] = backend;

    // Returns the newly created backend
    // TODO: It would be nicer with std::move and STL's smart pointers,
    // but RVO should optimize this out.
    return backend;
}

DevicesQueryPrivate::DevicesQueryPrivate(const QString &query)
    : query(query)
    , predicate(Solid::Predicate::fromString(query))
    , notifier(Solid::DeviceNotifier::instance())
{
    connect(notifier, &Solid::DeviceNotifier::deviceAdded,
            this,     &DevicesQueryPrivate::addDevice);
    connect(notifier, &Solid::DeviceNotifier::deviceRemoved,
            this,     &DevicesQueryPrivate::removeDevice);

    if (!query.isEmpty() && !predicate.isValid()) {
        return;
    }

    Q_FOREACH (const Solid::Device &device, Solid::Device::listFromQuery(predicate)) {
        QObject *deviceInterface = deviceInterfaceFromUdi(device.udi());
        if (deviceInterface) {
            matchingDevices << deviceInterface;
            connect(deviceInterface, &QObject::destroyed, this, &DevicesQueryPrivate::deviceDestroyed);
        }
    }
}

DevicesQueryPrivate::~DevicesQueryPrivate()
{
    handlers.remove(query);
}

#define return_conditionally_SOLID_DEVICE_INTERFACE(Type, Device) \
    if (Device.isDeviceInterface(Type)) { \
        return Device.asDeviceInterface(Type); \
    }

QObject *DevicesQueryPrivate::deviceInterfaceFromUdi(const QString &udi)
{
    Solid::Device device(udi);

    return_conditionally_SOLID_DEVICE_INTERFACE(Solid::DeviceInterface::Processor, device);
    return_conditionally_SOLID_DEVICE_INTERFACE(Solid::DeviceInterface::Block, device);
    return_conditionally_SOLID_DEVICE_INTERFACE(Solid::DeviceInterface::StorageAccess, device);
    return_conditionally_SOLID_DEVICE_INTERFACE(Solid::DeviceInterface::StorageDrive, device);
    return_conditionally_SOLID_DEVICE_INTERFACE(Solid::DeviceInterface::OpticalDrive, device);
    return_conditionally_SOLID_DEVICE_INTERFACE(Solid::DeviceInterface::StorageVolume, device);
    return_conditionally_SOLID_DEVICE_INTERFACE(Solid::DeviceInterface::OpticalDisc, device);
    return_conditionally_SOLID_DEVICE_INTERFACE(Solid::DeviceInterface::Camera, device);
    return_conditionally_SOLID_DEVICE_INTERFACE(Solid::DeviceInterface::PortableMediaPlayer, device);
    return_conditionally_SOLID_DEVICE_INTERFACE(Solid::DeviceInterface::Battery, device);
    return_conditionally_SOLID_DEVICE_INTERFACE(Solid::DeviceInterface::NetworkShare, device);
    return_conditionally_SOLID_DEVICE_INTERFACE(Solid::DeviceInterface::GenericInterface, device);

    return 0;
}

void DevicesQueryPrivate::addDevice(const QString &udi)
{
    if (predicate.isValid() && predicate.matches(Solid::Device(udi))) {
        QObject *device = deviceInterfaceFromUdi(udi);
        if (device) {
            matchingDevices << device;
            connect(device, &QObject::destroyed, this, &DevicesQueryPrivate::deviceDestroyed);
            emit deviceAdded(device);
        }
    }
}

void DevicesQueryPrivate::removeDevice(const QString &udi)
{
    emit deviceRemoved(udi);
}

void DevicesQueryPrivate::deviceDestroyed(QObject *obj)
{
    const int index = matchingDevices.indexOf(obj);
    if (index == -1) {
        return;
    }
    matchingDevices.removeAll(obj);
    emit deviceRemovedFromModel(index);
}

QList<QObject *> DevicesQueryPrivate::devices() const
{
    return matchingDevices;
}

void DeclarativeDevices::initialize() const
{
    if (m_backend) {
        return;
    }

    m_backend = DevicesQueryPrivate::forQuery(m_query);

    connect(m_backend.data(), &DevicesQueryPrivate::deviceAdded,
            this, &DeclarativeDevices::addDevice);
    connect(m_backend.data(), &DevicesQueryPrivate::deviceRemoved,
            this, &DeclarativeDevices::removeDevice);
    connect(m_backend.data(), &DevicesQueryPrivate::deviceRemovedFromModel,
            this, &DeclarativeDevices::removeDeviceFromModel);

    const int matchesCount = m_backend->devices().count();

    if (matchesCount != 0) {
        emit emptyChanged(false);
    }
}

void DeclarativeDevices::reset()
{
    if (!m_backend) {
        return;
    }

    m_backend->disconnect(this);
    m_backend.reset();

    emit emptyChanged(true);
    emit rowCountChanged(0);
}

void DeclarativeDevices::addDevice(const QObject *device)
{
    if (!m_backend) {
        return;
    }

    const int count = m_backend->devices().count();

    if (count == 1) {
        emit emptyChanged(false);
    }

    beginInsertRows(QModelIndex(), rowCount() - 1, rowCount() - 1);
    endInsertRows();

    emit rowCountChanged(count);
    emit deviceAdded(device);
}

void DeclarativeDevices::removeDevice(const QString &udi)
{
    if (!m_backend) {
        return;
    }

    const int count = m_backend->devices().count();

    if (count == 0) {
        emit emptyChanged(true);
    }

    emit rowCountChanged(count);
    emit deviceRemoved(udi);
}

void DeclarativeDevices::removeDeviceFromModel(const int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    endRemoveRows();
}

DeclarativeDevices::DeclarativeDevices(QAbstractListModel *parent)
    : QAbstractListModel(parent)
{
}

DeclarativeDevices::~DeclarativeDevices()
{
}

bool DeclarativeDevices::isEmpty() const
{
    initialize();
    return rowCount() == 0;
}

int DeclarativeDevices::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    initialize();
    return m_backend->devices().count();
}

QVariant DeclarativeDevices::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount()) {
        return QVariant();
    }

    if (role == DeviceRole) {
        return QVariant::fromValue(m_backend->devices().at(index.row()));
    }
    return QVariant();
}

QObject *DeclarativeDevices::get(const int index) const
{
    return m_backend->devices().at(index);
}

QList<QObject *> DeclarativeDevices::devices() const
{
    initialize();
    return m_backend->devices();
}

QString DeclarativeDevices::query() const
{
    return m_backend->query;
}

void DeclarativeDevices::setQuery(const QString &query)
{
    if (m_query == query) {
        return;
    }

    m_query = query;

    reset();
    initialize();

    emit queryChanged(query);
}

QHash<int, QByteArray> DeclarativeDevices::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DeviceRole] = "device";
    return roles;
}

} // namespace Solid
