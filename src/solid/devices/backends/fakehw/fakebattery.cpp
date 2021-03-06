/*
    Copyright 2006 Kevin Ottens <ervin@kde.org>
    Copyright 2012 Lukas Tinkl <ltinkl@redhat.com>
    Copyright 2014 Kai Uwe Broulik <kde@privat.broulik.de>

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

#include "fakebattery.h"

using namespace Solid::Backends::Fake;

FakeBattery::FakeBattery(FakeDevice *device)
    : FakeDeviceInterface(device)
{
}

FakeBattery::~FakeBattery()
{
}

bool FakeBattery::isPresent() const
{
    return fakeDevice()->property("isPresent").toBool();
}

Solid::Battery::BatteryType FakeBattery::type() const
{
    QString name = fakeDevice()->property("batteryType").toString();

    if (name == "pda") {
        return Solid::Battery::PdaBattery;
    } else if (name == "ups") {
        return Solid::Battery::UpsBattery;
    } else if (name == "primary") {
        return Solid::Battery::PrimaryBattery;
    } else if (name == "mouse") {
        return Solid::Battery::MouseBattery;
    } else if (name == "keyboard") {
        return Solid::Battery::KeyboardBattery;
    } else if (name == "keyboard_mouse") {
        return Solid::Battery::KeyboardMouseBattery;
    } else if (name == "camera") {
        return Solid::Battery::CameraBattery;
    } else {
        return Solid::Battery::UnknownBattery;
    }
}

int FakeBattery::chargePercent() const
{
    int last_full = fakeDevice()->property("lastFullLevel").toInt();
    int current = fakeDevice()->property("currentLevel").toInt();

    int percent = (100 * current) / last_full;

    return percent;
}

int FakeBattery::capacity() const
{
    return fakeDevice()->property("capacity").toInt();
}

bool FakeBattery::isRechargeable() const
{
    return fakeDevice()->property("isRechargeable").toBool();
}

bool FakeBattery::isPowerSupply() const
{
    return fakeDevice()->property("isPowerSupply").toBool();
}

Solid::Battery::ChargeState FakeBattery::chargeState() const
{
    QString state = fakeDevice()->property("chargeState").toString();

    if (state == "charging") {
        return Solid::Battery::Charging;
    } else if (state == "discharging") {
        return Solid::Battery::Discharging;
    } else if (state == "fullyCharged") {
        return Solid::Battery::FullyCharged;
    } else {
        return Solid::Battery::NoCharge;
    }
}

qlonglong FakeBattery::timeToEmpty() const
{
    return fakeDevice()->property("timeToEmpty").toLongLong();
}

qlonglong FakeBattery::timeToFull() const
{
    return fakeDevice()->property("timeToFull").toLongLong();
}

void FakeBattery::setChargeState(Solid::Battery::ChargeState newState)
{
    QString name;

    switch (newState) {
    case Solid::Battery::Charging:
        name = "charging";
        break;
    case Solid::Battery::Discharging:
        name = "discharging";
        break;
    case Solid::Battery::NoCharge:
        name = "noCharge";
        break;
    case Solid::Battery::FullyCharged:
        name = "fullyCharged";
        break;
    }

    fakeDevice()->setProperty("chargeState", name);
    emit chargeStateChanged(newState, fakeDevice()->udi());
}

void FakeBattery::setChargeLevel(int newLevel)
{
    fakeDevice()->setProperty("currentLevel", newLevel);
    emit chargePercentChanged(chargePercent(), fakeDevice()->udi());
}

Solid::Battery::Technology FakeBattery::technology() const
{
    return (Solid::Battery::Technology) fakeDevice()->property("technology").toInt();
}

double FakeBattery::energy() const
{
    return fakeDevice()->property("energy").toDouble();
}

double FakeBattery::energyFull() const
{
    return fakeDevice()->property("energyFull").toDouble();
}

double FakeBattery::energyFullDesign() const
{
    return fakeDevice()->property("energyFullDesign").toDouble();
}

double FakeBattery::energyRate() const
{
    return fakeDevice()->property("energyRate").toDouble();
}

double FakeBattery::voltage() const
{
    return fakeDevice()->property("voltage").toDouble();
}

double FakeBattery::temperature() const
{
    return fakeDevice()->property("temperature").toDouble();
}

bool FakeBattery::isRecalled() const
{
    return fakeDevice()->property("isRecalled").toBool();
}

QString FakeBattery::recallVendor() const
{
    return fakeDevice()->property("recallVendor").toString();
}

QString FakeBattery::recallUrl() const
{
    return fakeDevice()->property("recallUrl").toString();
}

QString FakeBattery::serial() const
{
    return fakeDevice()->property("serial").toString();
}

qlonglong FakeBattery::remainingTime() const
{
    return fakeDevice()->property("remainingTime").toLongLong();
}
