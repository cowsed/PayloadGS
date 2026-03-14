#include "telemetrylogholder.h"

TelemetryLogHolder::TelemetryLogHolder(QObject *parent)
    : QObject(parent)
    , battery_current(MAX_IN_MEM_BATT_VOLTAGE_ENTRIES)
    , battery_voltage(MAX_IN_MEM_BATT_VOLTAGE_ENTRIES)
    , cpu_temp(MAX_IN_MEM_CPU_TEMP_ENTRIES)
    , radio_temp(MAX_IN_MEM_RADIO_TEMP_ENTRIES)

{}

void TelemetryLogHolder::newRamUsage(QDateTime ts, uint64_t bytes) {}
void TelemetryLogHolder::newFsUsage(QDateTime ts, uint64_t bytes) {}

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
    last_rocket_pos = coord;
    emit rocketPositionChanged();
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
    return QGeoCoordinate(31.0443, -103.53507, 120);
}

QGeoCoordinate TelemetryLogHolder::latestStationPosition()
{
    return QGeoCoordinate(43.0839380, -77.6757720, 10);
}
QGeoCoordinate TelemetryLogHolder::latestRocketPosition()
{
    return last_rocket_pos;
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

FrontBackDataHolder *TelemetryLogHolder::getCpuTemp()
{
    return &cpu_temp;
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
