/*
    Copyright 2010 Mario Bensi <mbensi@ipsquad.net>

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

#ifndef SOLID_BACKENDS_FSTAB_WATCHER_H
#define SOLID_BACKENDS_FSTAB_WATCHER_H

#include <QObject>

class QFileSystemWatcher;
class QFile;
class QSocketNotifier;

namespace Solid
{
namespace Backends
{
namespace Fstab
{

class FstabWatcher : public QObject
{
    Q_OBJECT
public:
    FstabWatcher();
    virtual ~FstabWatcher();

    static FstabWatcher *instance();

Q_SIGNALS:
    void mtabChanged();
    void fstabChanged();

private Q_SLOTS:
    void onFileChanged(const QString &path);
    void orphanFileSystemWatcher();

private:
    bool m_isRoutineInstalled;
    QFileSystemWatcher *m_fileSystemWatcher;
    QSocketNotifier *m_mtabSocketNotifier;
    QFile *m_mtabFile;
};
}
}
}
#endif // SOLID_BACKENDS_FSTAB_WATCHER_H

