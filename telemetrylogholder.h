#ifndef TELEMETRYLOGHOLDER_H
#define TELEMETRYLOGHOLDER_H

#include <QDateTime>
#include <QVector3D>
#include <QXYSeries>
#include "QObject"
#include "frontbackdataholder.h"

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


    Q_PROPERTY(QDateTime latestPayloadPositionUpdateTime READ latestPayloadPositionUpdateTime NOTIFY
                   payloadPositionUpdateTimeChanged FINAL)
    Q_PROPERTY(QDateTime latestStationPositionUpdateTime READ latestStationPositionUpdateTime NOTIFY
                   stationPositionUpdateTimeChanged FINAL)
    Q_PROPERTY(QDateTime latestRocketPositionUpdateTime READ latestRocketPositionUpdateTime NOTIFY
                   rocketPositionUpdateTimeChanged FINAL)

    Q_PROPERTY(
        QDateTime latestImuUpdateTime READ latestImuUpdateTime NOTIFY imuUpdateTimeChanged FINAL)

    // Q_PROPERTY(QVector3D baseImu READ baseImu NOTIFY baseImuChanged)
    // Q_PROPERTY(QVector3D link2Imu READ link2Imu NOTIFY link2ImuChanged)

    Q_PROPERTY(uint64_t latestRamUsage READ latestRamUsage NOTIFY ramUsageChanged FINAL)
    Q_PROPERTY(uint64_t latestFsUsage READ latestFsUsage NOTIFY fsUsageChanged FINAL)

    Q_PROPERTY(FrontBackDataHolder *cpuTemp READ getCpuTemp NOTIFY cpuTempChanged)
    Q_PROPERTY(FrontBackDataHolder *radioTemp READ getRadioTemp NOTIFY radioTempChanged)
    Q_PROPERTY(
        FrontBackDataHolder *batteryCurrent READ getBatteryCurrent NOTIFY batteryCurrentChanged)
    Q_PROPERTY(
        FrontBackDataHolder *batteryVoltage READ getBatteryVoltage NOTIFY batteryVoltageChanged)

    QML_ELEMENT
    QML_SINGLETON

public:
    static constexpr size_t MAX_IN_MEM_MOTOR_TEMP_ENTRIES = 20;
    static constexpr size_t MAX_IN_MEM_RADIO_TEMP_ENTRIES = 20;
    static constexpr size_t MAX_IN_MEM_BATT_VOLTAGE_ENTRIES = 40;

    TelemetryLogHolder(QObject *parent = nullptr);

    Q_INVOKABLE QGeoCoordinate latestPayloadPosition();
    Q_INVOKABLE QGeoCoordinate latestStationPosition();
    Q_INVOKABLE QGeoCoordinate latestRocketPosition();

    Q_INVOKABLE QDateTime latestPayloadPositionUpdateTime();
    Q_INVOKABLE QDateTime latestStationPositionUpdateTime();
    Q_INVOKABLE QDateTime latestRocketPositionUpdateTime();
    Q_INVOKABLE QDateTime latestImuUpdateTime();

    Q_INVOKABLE uint64_t latestRamUsage();
    Q_INVOKABLE uint64_t latestFsUsage();

    FrontBackDataHolder *getCpuTemp();
    FrontBackDataHolder *getRadioTemp();
    FrontBackDataHolder *getBatteryCurrent();
    FrontBackDataHolder *getBatteryVoltage();
public slots:

    void newMotorTemp(QDateTime ts, double tempC);
    void newRadioTemp(QDateTime ts, double tempC);

    void newRamUsage(QDateTime ts, uint64_t);
    void newFsUsage(QDateTime ts, uint64_t);

    void newPayloadPosition(QDateTime ts, QGeoCoordinate coord);
    void newRocketPosition(QDateTime ts, QGeoCoordinate coord);
    Q_INVOKABLE void newStationPosition(QDateTime ts, QGeoCoordinate coord);

    void newImuData(QDateTime ts, QVector3D base, QVector3D link2);

    void newBatteryInformation(QDateTime ts, double volts, double amps);

    Q_INVOKABLE bool newDirectory(QString new_dir);

signals:
    void payloadPositionChanged();
    void stationPositionChanged();
    void rocketPositionChanged();

    void payloadPositionUpdateTimeChanged();
    void stationPositionUpdateTimeChanged();
    void rocketPositionUpdateTimeChanged();

    void imuUpdateTimeChanged();
    void baseImuChanged();
    void link2ImuChanged();

    void ramUsageChanged();
    void fsUsageChanged();

    void cpuTempChanged();
    void radioTempChanged();
    void batteryCurrentChanged();
    void batteryVoltageChanged();

private:
    QDateTime last_station_pos_update;
    QGeoCoordinate last_station_pos{43.0839380, -77.6757720, 10};
    QDateTime last_rocket_pos_update;
    QGeoCoordinate last_rocket_pos{0, 0, 120};
    QDateTime last_payload_pos_update;
    QGeoCoordinate last_payload_pos{31.0443, -103.53507, 120};

    QVector3D last_base_imu;
    QVector3D last_link2_imu;
    QDateTime latest_imu_update_time;

    FrontBackDataHolder battery_voltage;
    FrontBackDataHolder battery_current;
    FrontBackDataHolder motor_temp;
    FrontBackDataHolder radio_temp;
};

#endif // TELEMETRYLOGHOLDER_H
