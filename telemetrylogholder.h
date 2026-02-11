#ifndef TELEMETRYLOGHOLDER_H
#define TELEMETRYLOGHOLDER_H

#include <QDateTime>
#include <QXYSeries>
#include "QObject"
#include "datasource.h"

#include <QtPositioning/QGeoCoordinate>
#include <qqmlintegration.h>
/**
 * @brief The TelemetryLogHolder class 
 * holds telem data series and saves them to CSV log file so it can comme back to it
 */

class TelemetryLogHolder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QGeoCoordinate latestPayloadPosition READ latestPayloadPosition NOTIFY
                   payloadPositionChanged FINAL)
    Q_PROPERTY(QGeoCoordinate latestStationPosition READ latestStationPosition NOTIFY
                   stationPositionChanged FINAL)
    Q_PROPERTY(QGeoCoordinate latestRocketPosition READ latestRocketPosition NOTIFY
                   rocketPositionChanged FINAL)

    Q_PROPERTY(
        double latestBatteryVoltage READ latestBatteryVoltage NOTIFY batteryVoltageChanged FINAL)
    Q_PROPERTY(
        double latestBatteryCurrent READ latestBatteryCurrent NOTIFY batteryCurrentChanged FINAL)

    Q_PROPERTY(double latestCpuTemp READ latestCpuTemp NOTIFY cpuTempChanged FINAL)
    Q_PROPERTY(double latestRadioTemp READ latestRadioTemp NOTIFY radioTempChanged FINAL)

    Q_PROPERTY(QDateTime latestPayloadPositionUpdateTime READ latestPayloadPositionUpdateTime NOTIFY
                   payloadPositionUpdateTimeChanged FINAL)
    Q_PROPERTY(QDateTime latestStationPositionUpdateTime READ latestStationPositionUpdateTime NOTIFY
                   stationPositionUpdateTimeChanged FINAL)
    Q_PROPERTY(QDateTime latestRocketPositionUpdateTime READ latestRocketPositionUpdateTime NOTIFY
                   rocketPositionUpdateTimeChanged FINAL)

    Q_PROPERTY(QDateTime batteryVoltageUpdateTime READ batteryVoltageUpdateTime NOTIFY
                   batteryVoltageUpdateTimeChanged FINAL)
    Q_PROPERTY(QDateTime batteryCurrentUpdateTime READ batteryCurrentUpdateTime NOTIFY
                   batteryCurrentUpdateTimeChanged FINAL)

    Q_PROPERTY(uint64_t latestRamUsage READ latestRamUsage NOTIFY ramUsageChanged FINAL)
    Q_PROPERTY(uint64_t latestFsUsage READ latestFsUsage NOTIFY fsUsageChanged FINAL)

    Q_PROPERTY(
        QDateTime cpuTempUpdateTime READ cpuTempUpdateTime NOTIFY cpuTempUpdateTimeChanged FINAL)
    Q_PROPERTY(QDateTime radioTempUpdateTime READ radioTempUpdateTime NOTIFY
                   radioTempUpdateTimeChanged FINAL)

    QML_ELEMENT
    QML_SINGLETON

public:
    static constexpr size_t MAX_IN_MEM_CPU_TEMP_ENTRIES = 20;
    static constexpr size_t MAX_IN_MEM_RADIO_TEMP_ENTRIES = 20;
    static constexpr size_t MAX_IN_MEM_BATT_VOLTAGE_ENTRIES = 20;

    TelemetryLogHolder(QObject *parent = nullptr);

    Q_INVOKABLE QGeoCoordinate latestPayloadPosition();
    Q_INVOKABLE QGeoCoordinate latestStationPosition();
    Q_INVOKABLE QGeoCoordinate latestRocketPosition();

    Q_INVOKABLE QDateTime latestPayloadPositionUpdateTime();
    Q_INVOKABLE QDateTime latestStationPositionUpdateTime();
    Q_INVOKABLE QDateTime latestRocketPositionUpdateTime();

    Q_INVOKABLE double latestBatteryVoltage();
    Q_INVOKABLE double latestBatteryCurrent();

    Q_INVOKABLE QDateTime batteryVoltageUpdateTime();
    Q_INVOKABLE QDateTime batteryCurrentUpdateTime();

    Q_INVOKABLE QDateTime earliestBatteryVoltageTime();

    Q_INVOKABLE double latestCpuTemp();
    Q_INVOKABLE double latestRadioTemp();

    Q_INVOKABLE QDateTime cpuTempUpdateTime();
    Q_INVOKABLE QDateTime radioTempUpdateTime();

    Q_INVOKABLE QDateTime earliestCpuTempTime();
    Q_INVOKABLE QDateTime earliestRadioTempTime();

    Q_INVOKABLE uint64_t latestRamUsage();
    Q_INVOKABLE uint64_t latestFsUsage();

    Q_INVOKABLE void updateCpuTempSeries(QAbstractSeries *series);
    Q_INVOKABLE void updateRadioTempSeries(QAbstractSeries *series);
    Q_INVOKABLE void updateBatteryVoltageSeries(QAbstractSeries *series);

public slots:
    void newBatteryVoltage(QDateTime ts, double voltage);
    void newBatteryCurrent(QDateTime ts, double current);

    void newCpuTemp(QDateTime ts, double temp);
    void newRadioTemp(QDateTime ts, double temp);

    void newRamUsage(QDateTime ts, uint64_t);
    void newFsUsage(QDateTime ts, uint64_t);

    void newPayloadPosition(QDateTime ts, QGeoCoordinate coord);
    void newStationPosition(QDateTime ts, QGeoCoordinate coord);
    Q_INVOKABLE void newRocketPosition(QDateTime ts, QGeoCoordinate coord);

signals:
    void payloadPositionChanged();
    void stationPositionChanged();
    void rocketPositionChanged();

    void payloadPositionUpdateTimeChanged();
    void stationPositionUpdateTimeChanged();
    void rocketPositionUpdateTimeChanged();

    void batteryVoltageChanged();
    void batteryCurrentChanged();

    void batteryVoltageUpdateTimeChanged();
    void batteryCurrentUpdateTimeChanged();

    void cpuTempChanged();
    void radioTempChanged();

    void cpuTempUpdateTimeChanged();
    void radioTempUpdateTimeChanged();

    void ramUsageChanged();
    void fsUsageChanged();

private:
    QGeoCoordinate last_rocket_pos{0, 0, 120};

    QList<QPointF> cpu_temps{};
    QList<QPointF> radio_temps{};
    QList<QPointF> battery_voltages{};

    DataSource line_data;
};

#endif // TELEMETRYLOGHOLDER_H
