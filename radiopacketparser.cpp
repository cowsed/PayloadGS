#include "radiopacketparser.h"
#include <QFile>
#include <QtLogging>
#include "cubesat_comms/packets_p2g.h"

PayloadFlags PayloadFlags::fromBits(uint16_t bits)
{
    return {
        .Active = (bits & (1 << StatusBit::Active)) != 0,
        .Autonomous = (bits & (1 << StatusBit::Autonomous)) != 0,
        .LastArmMovedStalled = (bits & (1 << StatusBit::LastArmMoveStalled)) != 0,
        .LastServoMoveStalled = (bits & (1 << StatusBit::LastServoMoveStalled)) != 0,
        .ArmMoving = (bits & (1 << StatusBit::ArmMoving)) != 0,
        .ServoMoving = (bits & (1 << StatusBit::ServoMoving)) != 0,
        .InIdlePosition = (bits & (1 << StatusBit::InIdlePosition)) != 0,
        .MotorsOverTemp = (bits & (1 << StatusBit::MotorsOverTemp)) != 0,
        .RadioOverTemp = (bits & (1 << StatusBit::RadioOverTemp)) != 0,
        .GPSHasFix = (bits & (1 << StatusBit::GPSHasFix)) != 0,
    };
}

RadioPacketParser::RadioPacketParser(QObject *parent)
    : QObject{parent}
{

}

void RadioPacketParser::packetReceived(QDateTime time, int snr, int rssi, const QByteArray &packet)
{
    qDebug("Got SNR: %d - RSSI %d", snr, rssi);
    qDebug("Bytes: %s", qPrintable(packet.toHex()));
    P2GLinkHeader header;
    UnpackResult res = unpack_p2g_link_header((uint8_t *) packet.data(), packet.size(), &header);
    if (res != UnpackResult::UnpackResult_AllGood) {
        qDebug("failed to unpack reason %d", res);
        return;
    }
    qDebug("Header: type = %d, num_left = %d",
           header.packet_type,
           header.expected_packets_before_response);

    const uint8_t *rest = (uint8_t *) packet.constData() + 1;
    uint32_t rest_len = packet.size() - 1;

    CommandResponse cmd_resp = {};
    ImageData data = {0};

    switch (header.packet_type) {
    case P2GPacketType::P2GPacketType_CommandResponse:
        res = unpack_command_response(rest, rest_len, &cmd_resp);
        if (res != UnpackResult_AllGood) {
            qDebug("Failed to unpack command response: %d", res);
            break;
        }
        emitCommandResponse(time, &cmd_resp);
        break;
    case P2GPacketType::P2GPacketType_ImageResponse:
        res = unpack_image_data(rest, rest_len, &data);
        if (res != UnpackResult_AllGood) {
            qDebug("Failed to unpack image data");
            break;
        }
        emit imageDataReceived(time, data);
        break;
    default:
        qDebug("Other type of p2g packet");
        break;
    }
}

void RadioPacketParser::emitTelemetry(QDateTime time, const Telemetry *telem)
{
    switch (telem->telem_type) {
    case TelemetryType_FlightHeartbeat:
        printf("Emitting telem: %d\n", (int) (telem->flight_heartbeat_stats.state.phase));
        emit flightHeartbeat(time,
                             telem->flight_heartbeat_stats.state,
                             telem->flight_heartbeat_stats.latitude,
                             telem->flight_heartbeat_stats.longitude,
                             telem->flight_heartbeat_stats.altitude,
                             telem->flight_heartbeat_stats.s_since_boost,
                             telem->flight_heartbeat_stats.battery_mV,
                             telem->flight_heartbeat_stats.radio_temp);
        emit flightStateUpdated(time,
                                telem->flight_heartbeat_stats.state.phase,
                                telem->flight_heartbeat_stats.state.status_bits);
        emit payloadGPSUpdated(time,
                               QGeoCoordinate(telem->flight_heartbeat_stats.latitude,
                                              telem->flight_heartbeat_stats.longitude,
                                              telem->flight_heartbeat_stats.altitude));
        emit batteryUpdated(time, telem->flight_heartbeat_stats.battery_mV / 1000.0, NAN);
        emit radioTempUpdated(time, telem->flight_heartbeat_stats.radio_temp);
        break;
    case TelemetryType_LandedHeartbeat:
        emit landedHeartbeat(time,
                             telem->landed_heartbeat_stats.state,
                             telem->landed_heartbeat_stats.next_image_id,
                             telem->landed_heartbeat_stats.next_exec_id,
                             telem->landed_heartbeat_stats.arm_position,
                             telem->landed_heartbeat_stats.battery_mV,
                             telem->landed_heartbeat_stats.motor_temp,
                             telem->landed_heartbeat_stats.radio_temp);
        emit flightStateUpdated(time,
                                telem->landed_heartbeat_stats.state.phase,
                                telem->landed_heartbeat_stats.state.status_bits);
        emit motorTempUpdated(time, telem->landed_heartbeat_stats.motor_temp);
        emit radioTempUpdated(time, telem->landed_heartbeat_stats.radio_temp);
    case TelemetryType_Actuators:
        emit armAnglesUpdated(time,
                              telem->actuators.arms.shoulder_yaw,
                              telem->actuators.arms.shoulder_pitch,
                              telem->actuators.arms.elbow_pitch,
                              telem->actuators.arms.wrist_pitch);
        emit servoAnglesUpdated(time,
                                telem->actuators.servo1,
                                telem->actuators.servo2,
                                telem->actuators.servo3,
                                telem->actuators.servo4);
        break;
    case TelemetryType_GNSS:
    case TelemetryType_System:
    case TelemetryType_Orientations:
    case TelemetryType_Temps:
    case TelemetryType_Power:

    default:
        qDebug("Unhandled emitTelemetry");
        break;
    }
}

void RadioPacketParser::emitCommandResponse(QDateTime time, const CommandResponse *resp)
{
    switch (resp->cmd) {
    case Command_ForceManual:
    case Command_ForceFlight:
    case Command_ExpectFlight:
    case Command_UnexpectFlight:
    case Command_StartVideo:
    case Command_StopVideo:
    case Command_ReCrop:
    case Command_SendArmTargetAndComeBack:
    case Command_SendArmTargetForPhotoAndComeBack:
    case Command_SendIdlePosition:
    case Command_WriteArmSequence:
    case Command_ReadArmSequence:
    case Command_ExecuteArmSequence:
    case Command_CancelExecutingArmSequence:
    case Command_ZeroShoulder_AssumeOpen:
    case Command_RunOpenSequence:
    case Command_ShellExec:
    case Command_ClearFlightDANGER:
    case Command_GetFlightNumber:
    case Command_Callsign:
    case Command_MaxCommand:
        qDebug("COMMAND RESPONSERECEIVED WITH NO EMIT HANDLER");
        break;
    case Command_ShellExecInfo:
        break;
    case Command_ShellReadStdout:
        break;
    case Command_ShellReadStderr:
        break;
    case Command_TelemetryRequest:
        emitTelemetry(time, &resp->telemetry);
        break;
    }
}

void RadioPacketParser::b64PacketReceived(QDateTime time, int snr, int rssi, const QString &packet)
{
    QByteArray::FromBase64Result res = QByteArray::fromBase64Encoding(packet.toUtf8());
    if (res.decodingStatus != QByteArray::Base64DecodingStatus::Ok) {
        qDebug("Bad b64 decode");
        return;
    }
    qDebug("From %s", qPrintable(packet));
    qDebug("B64 dec from %lld to %lld", packet.size(), res.decoded.size());
    packetReceived(time, snr, rssi, res.decoded);
}

QString RadioPacketParser::phaseToShortString(FlightPhaseQML phase)
{
    switch (phase) {
    case Starting:
        return "Starting";
    case Pad:
        return "Pad";
    case Expecting:
        return "Expecting";
    case Flight:
        return "Flight";
    case LandedFlipping:
        return "LFlip";
    case LandedAutomatic:
        return "LAuto";
    case LandedManual:
        return "LManual";
    }
    return "???";
}

PayloadFlags RadioPacketParser::statusBitsToFlags(uint16_t bits)
{
    return PayloadFlags::fromBits(bits);
}
