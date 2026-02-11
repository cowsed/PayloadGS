#include "telemetrylogholder.h"

TelemetryLogHolder::TelemetryLogHolder(QObject *parent)
    : QObject(parent)
    , line_data(this)
{}

void TelemetryLogHolder::newBatteryVoltage(QDateTime ts, double voltage)
{
    battery_voltages.push_back({static_cast<qreal>(ts.toMSecsSinceEpoch()), voltage});

    if (battery_voltages.length() > MAX_IN_MEM_BATT_VOLTAGE_ENTRIES) {
        battery_voltages.removeFirst();
    }

    emit batteryVoltageChanged();
}
void TelemetryLogHolder::newBatteryCurrent(QDateTime ts, double current) {}
void TelemetryLogHolder::newCpuTemp(QDateTime ts, double temp)
{
    cpu_temps.push_back({static_cast<qreal>(ts.toMSecsSinceEpoch()), temp});

    if (cpu_temps.length() > MAX_IN_MEM_CPU_TEMP_ENTRIES) {
        cpu_temps.removeFirst();
    }

    emit cpuTempChanged();
}
void TelemetryLogHolder::newRadioTemp(QDateTime ts, double temp)
{
    {
        radio_temps.push_back({static_cast<qreal>(ts.toMSecsSinceEpoch()), temp});

        if (radio_temps.length() > MAX_IN_MEM_RADIO_TEMP_ENTRIES) {
            radio_temps.removeFirst();
        }

        emit radioTempChanged();
    }
}

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

double TelemetryLogHolder::latestRadioTemp()
{
    return 35;
}
double TelemetryLogHolder::latestCpuTemp()
{
    if (cpu_temps.isEmpty()) {
        return NAN;
    }
    return cpu_temps.last().y();
}

QDateTime TelemetryLogHolder::earliestCpuTempTime()
{
    if (cpu_temps.isEmpty()) {
        return QDateTime::fromMSecsSinceEpoch(0);
    }
    return QDateTime::fromMSecsSinceEpoch(cpu_temps.first().x());
}

QDateTime TelemetryLogHolder::earliestRadioTempTime()
{
    if (radio_temps.isEmpty()) {
        return QDateTime::fromMSecsSinceEpoch(0);
    }
    return QDateTime::fromMSecsSinceEpoch(radio_temps.first().x());
}

QDateTime TelemetryLogHolder::earliestBatteryVoltageTime()
{
    if (battery_voltages.isEmpty()) {
        return QDateTime::fromMSecsSinceEpoch(0);
    }
    return QDateTime::fromMSecsSinceEpoch(battery_voltages.first().x());
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
    if (battery_voltages.isEmpty()) {
        return 0;
    }
    return battery_voltages.last().y();
}

double TelemetryLogHolder::latestBatteryCurrent()
{
    return 1234;
}

QDateTime TelemetryLogHolder::cpuTempUpdateTime()
{
    if (cpu_temps.isEmpty()) {
        return QDateTime::fromMSecsSinceEpoch(0);
    }

    return QDateTime::fromMSecsSinceEpoch(cpu_temps.last().x());
}

QDateTime TelemetryLogHolder::radioTempUpdateTime()
{
    if (radio_temps.isEmpty()) {
        return QDateTime::fromMSecsSinceEpoch(0);
    }

    return QDateTime::fromMSecsSinceEpoch(radio_temps.last().x());
}

void TelemetryLogHolder::updateCpuTempSeries(QAbstractSeries *series)
{
    if (series) {
        auto xySeries = static_cast<QXYSeries *>(series);
        xySeries->replace(cpu_temps);
    }
}

void TelemetryLogHolder::updateRadioTempSeries(QAbstractSeries *series)
{
    if (series) {
        auto xySeries = static_cast<QXYSeries *>(series);
        xySeries->replace(radio_temps);
    }
}

void TelemetryLogHolder::updateBatteryVoltageSeries(QAbstractSeries *series)
{
    if (series) {
        auto xySeries = static_cast<QXYSeries *>(series);
        xySeries->replace(battery_voltages);
    }
}
