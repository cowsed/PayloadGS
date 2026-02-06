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

    Q_PROPERTY(QDateTime latestPayloadPositionUpdateTime READ latestPayloadPositionUpdateTime NOTIFY
                   payloadPositionUpdateTimeChanged FINAL)
    Q_PROPERTY(QDateTime latestStationPositionUpdateTime READ latestStationPositionUpdateTime NOTIFY
                   stationPositionUpdateTimeChanged FINAL)
    Q_PROPERTY(QDateTime latestRocketPositionUpdateTime READ latestRocketPositionUpdateTime NOTIFY
                   rocketPositionUpdateTimeChanged FINAL)

    Q_PROPERTY(QDateTime batteryVoltageUpdateTime READ batteryVoltageUpdateTime NOTIFY
                   batteryVoltageUpdateTimeChanged FINAL)

    QML_ELEMENT
    QML_SINGLETON

public:
    TelemetryLogHolder(QObject *parent = nullptr);

    static constexpr size_t num_line_channels = 6;

    Q_INVOKABLE uint64_t latest_ram_usage();
    Q_INVOKABLE uint64_t latest_fs_usage();

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

public slots:
    void newBatteryVoltage(QDateTime ts, double voltage);
    void newBatteryCurrent(QDateTime ts, double current);

    void new_cpu_temp_reading(QDateTime ts, double temp);
    void new_radio_temp_reading(QDateTime ts, double temp);

    void new_ram_usage(QDateTime ts);
    void new_fs_usage(QDateTime ts);

    void newPayloadPosition(QDateTime ts, QGeoCoordinate coord);
    void newStationPosition(QDateTime ts, QGeoCoordinate coord);
    void newRocketPosition(QDateTime ts, QGeoCoordinate coord);

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

private:
    DataSource line_data;
};

#endif // TELEMETRYLOGHOLDER_H
