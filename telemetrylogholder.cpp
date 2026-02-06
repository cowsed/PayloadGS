#include "telemetrylogholder.h"

TelemetryLogHolder::TelemetryLogHolder(QObject *parent)
    : QObject(parent)
    , line_data(this)
{}

void TelemetryLogHolder::newBatteryVoltage(QDateTime ts, double voltage) {}
void TelemetryLogHolder::newBatteryCurrent(QDateTime ts, double current) {}
void TelemetryLogHolder::new_cpu_temp_reading(QDateTime ts, double temp) {}
void TelemetryLogHolder::new_radio_temp_reading(QDateTime ts, double temp) {}

void TelemetryLogHolder::new_ram_usage(QDateTime ts) {}
void TelemetryLogHolder::new_fs_usage(QDateTime ts) {}

void TelemetryLogHolder::newPayloadPosition(QDateTime ts, QGeoCoordinate coord)
{
    emit payloadPositionChanged();
}

void TelemetryLogHolder::newStationPosition(QDateTime ts, QGeoCoordinate coord)
{
    emit payloadPositionChanged();
}

void TelemetryLogHolder::newRocketPosition(QDateTime ts, QGeoCoordinate coord)
{
    emit payloadPositionChanged();
}

uint64_t TelemetryLogHolder::latest_ram_usage()
{
    return 100 * 1000;
}
uint64_t TelemetryLogHolder::latest_fs_usage()
{
    return 100 * 1000;
}
QGeoCoordinate TelemetryLogHolder::latestPayloadPosition()
{
    return QGeoCoordinate(43.0827370, -77.6694478, 120);
}

QGeoCoordinate TelemetryLogHolder::latestStationPosition()
{
    return QGeoCoordinate(43.0839380, -77.6757720, 10);
}
QGeoCoordinate TelemetryLogHolder::latestRocketPosition()
{
    return QGeoCoordinate(43.0858885, -77.6790960, 120);
}

QDateTime TelemetryLogHolder::latestPayloadPositionUpdateTime()
{
    return QDateTime::currentDateTime();
}
QDateTime TelemetryLogHolder::latestStationPositionUpdateTime()
{
    return QDateTime::currentDateTime().addSecs(5);
}
QDateTime TelemetryLogHolder::latestRocketPositionUpdateTime()
{
    return QDateTime::currentDateTime().addSecs(50);
}

QDateTime TelemetryLogHolder::batteryVoltageUpdateTime()
{
    return QDateTime::currentDateTime();
}

QDateTime TelemetryLogHolder::batteryCurrentUpdateTime()
{
    return QDateTime::currentDateTime();
}

double TelemetryLogHolder::latestBatteryVoltage()
{
    return 10.3;
}

double TelemetryLogHolder::latestBatteryCurrent()
{
    return 1234;
}
