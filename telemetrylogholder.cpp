#include "telemetrylogholder.h"
#include <qdir.h>

TelemetryLogHolder::TelemetryLogHolder(QObject *parent)
    : QObject(parent)
    , battery_current(MAX_IN_MEM_BATT_VOLTAGE_ENTRIES)
    , battery_voltage(MAX_IN_MEM_BATT_VOLTAGE_ENTRIES)
    , motor_temp(MAX_IN_MEM_MOTOR_TEMP_ENTRIES)
    , radio_temp(MAX_IN_MEM_RADIO_TEMP_ENTRIES)

{}

void TelemetryLogHolder::newMotorTemp(QDateTime ts, double tempC)
{
    motor_temp.newValue(ts, tempC);
}

void TelemetryLogHolder::newRadioTemp(QDateTime ts, double tempC)
{
    radio_temp.newValue(ts, tempC);
}

void TelemetryLogHolder::newRamUsage(QDateTime ts, uint64_t bytes) {}
void TelemetryLogHolder::newFsUsage(QDateTime ts, uint64_t bytes) {}

void TelemetryLogHolder::newPayloadPosition(QDateTime ts, QGeoCoordinate coord)
{
    last_payload_pos = coord;
    last_payload_pos_update = ts;

    emit payloadPositionChanged();
    emit payloadPositionUpdateTimeChanged();
}

void TelemetryLogHolder::newStationPosition(QDateTime ts, QGeoCoordinate coord)
{
    last_station_pos_update = ts;
    last_station_pos = coord;

    emit stationPositionChanged();
    emit stationPositionUpdateTimeChanged();
}

void TelemetryLogHolder::newRocketPosition(QDateTime ts, QGeoCoordinate coord)
{
    last_rocket_pos_update = ts;
    last_rocket_pos = coord;
    emit rocketPositionChanged();
    emit rocketPositionUpdateTimeChanged();
}

void TelemetryLogHolder::newBatteryInformation(QDateTime ts, double volts, double amps)
{
    battery_voltage.newValue(ts, volts);
    battery_current.newValue(ts, amps);
}

uint64_t TelemetryLogHolder::latestRamUsage()
{
    return (uint64_t) 1000 * 1024 * 1024;
}
uint64_t TelemetryLogHolder::latestFsUsage()
{
    return (uint64_t) 40 * 1024 * 1024 * 1024;
}

QGeoCoordinate TelemetryLogHolder::latestPayloadPosition()
{
    return last_payload_pos;
}

QGeoCoordinate TelemetryLogHolder::latestStationPosition()
{
    return last_station_pos;
}
QGeoCoordinate TelemetryLogHolder::latestRocketPosition()
{
    return last_rocket_pos;
}

QDateTime TelemetryLogHolder::latestPayloadPositionUpdateTime()
{
    return last_payload_pos_update;
}
QDateTime TelemetryLogHolder::latestStationPositionUpdateTime()
{
    return last_station_pos_update;
}
QDateTime TelemetryLogHolder::latestRocketPositionUpdateTime()
{
    return last_rocket_pos_update;
}

QDateTime TelemetryLogHolder::latestImuUpdateTime()
{
    return latest_imu_update_time;
}

FrontBackDataHolder *TelemetryLogHolder::getCpuTemp()
{
    return &motor_temp;
}

FrontBackDataHolder *TelemetryLogHolder::getRadioTemp()
{
    return &radio_temp;
}

FrontBackDataHolder *TelemetryLogHolder::getBatteryCurrent()
{
    return &battery_current;
}

FrontBackDataHolder *TelemetryLogHolder::getBatteryVoltage()
{
    return &battery_voltage;
}

QVector3D TelemetryLogHolder::baseImu()
{
    return last_base_imu;
}

QVector3D TelemetryLogHolder::link2Imu()
{
    return last_link2_imu;
}

void TelemetryLogHolder::newImuData(QDateTime ts, QVector3D base, QVector3D link2)
{
    latest_imu_update_time = ts;
    last_base_imu = base;
    last_link2_imu = link2;
    emit baseImuChanged();
    emit link2ImuChanged();
    emit imuUpdateTimeChanged();
}

bool TelemetryLogHolder::newDirectory(QString new_dir)
{
    QDir dir{new_dir};
    if (!dir.cdUp()) {
        return false;
    }

    if (!dir.mkpath(new_dir)) {
        return false;
    }

    dir.mkpath(new_dir + "/Images");
    dir.mkpath(new_dir + "/Packets");

    return true;
}
