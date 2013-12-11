/*
 * (C) Copyright 2013 Emil Ljungdahl
 *
 * This file is part of Openambit.
 *
 * Openambit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contributors:
 *
 */
#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QMetaType>

#include "settings.h"
#include "logstore.h"
#include "movescount.h"
#include <libambit.h>

class DeviceManager : public QObject
{
    Q_OBJECT
public:
    void start();
    ~DeviceManager();
signals:
    void deviceDetected(ambit_device_info_t deviceInfo, bool supported);
    void deviceRemoved(void);
    void deviceCharge(quint8 percent);
    void syncFinished(bool success);
    void syncProgressInform(QString message, bool newRow, quint8 percentDone);
public slots:
    void detect(void);
    void startSync(bool readAllLogs);

private slots:
    void chargeTimerHit();

private:
    static int log_skip_cb(void *ref, ambit_log_header_t *log_header);
    static void log_push_cb(void *ref, ambit_log_entry_t *log_entry);
    static void log_progress_cb(void *ref, uint16_t log_count, uint16_t log_current, uint8_t progress_percent);

    ambit_object_t *deviceObject = NULL;
    ambit_device_info_t currentDeviceInfo;
    ambit_personal_settings_t currentPersonalSettings;

    QMutex mutex;
    QTimer chargeTimer;
    MovesCount movesCount;
    Settings settings;
    LogStore logStore;

};

#endif // DEVICEMANAGER_H