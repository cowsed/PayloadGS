#include "radiopacketparser.h"
#include <QFile>
#include <QtLogging>
#include "cubesat_comms/lora.h"
#include "cubesat_comms/packets_g2p.h"
#include "cubesat_comms/packets_p2g.h"
#include "imagemetadataholder.h"
#include <algorithm>

LoraSettings::SpreadingFactor rsf_to_sf(RadioClient::SF sf);
LoraSettings::Bandwidth rbw_to_bw(RadioClient::BW bw);
LoraSettings::CodingRate rcr_to_cr(RadioClient::CR cr);

RadioClient::SF sf_to_rsf(LoraSettings::SpreadingFactor sf);
RadioClient::BW bw_to_rbw(LoraSettings::Bandwidth bw);
RadioClient::CR cr_to_rcr(LoraSettings::CodingRate cr);
bool compute_ldro(LoraSettings::SpreadingFactor spreading_factor, LoraSettings::Bandwidth bandwidth)
{
    if (spreading_factor >= LoraSettings::SpreadingFactor::SF11
        && bandwidth <= LoraSettings::Bandwidth::BW125) {
        return true;
    }
    if (spreading_factor == LoraSettings::SpreadingFactor::SF10
        && bandwidth <= LoraSettings::Bandwidth::BW62) {
        return true;
    }
    return false;
}

PayloadFlags PayloadFlags::fromBits(uint16_t bits)
{
    return {
        .Active = (bits & (1 << StatusBit_Active)) != 0,
        .Autonomous = (bits & (1 << StatusBit_Autonomous)) != 0,
        .LastArmMovedStalled = (bits & (1 << StatusBit_LastArmMoveStalled)) != 0,
        .LastServoMoveStalled = (bits & (1 << StatusBit_LastServoMoveStalled)) != 0,
        .ArmMoving = (bits & (1 << StatusBit_ArmMoving)) != 0,
        .ServoMoving = (bits & (1 << StatusBit_ServoMoving)) != 0,
        .InIdlePosition = (bits & (1 << StatusBit_InIdlePosition)) != 0,
        .MotorsOverTemp = (bits & (1 << StatusBit_MotorsOverTemp)) != 0,
        .RadioOverTemp = (bits & (1 << StatusBit_RadioOverTemp)) != 0,
        .GPSHasFix = (bits & (1 << StatusBit_GPSHasFix)) != 0,
    };
}

RadioPacketParser::RadioPacketParser(QObject *parent)
    : QObject{parent}
    , radio_snr(MAX_IN_MEM_RADIO_SIGNAL_ENTRIES)
    , radio_rssi(MAX_IN_MEM_RADIO_SIGNAL_ENTRIES)
{
    payload_client = new RadioClient();

    QObject::connect(payload_client,
                     &RadioClient::packetReceived,
                     this,
                     &RadioPacketParser::packetReceived,
                     Qt::QueuedConnection);

    QObject::connect(payload_client, &RadioClient::connected, this, [&]() {
        printf("Connected\n");

        const LoraSettings &defaults = defaultLoraSettings();
        bool ldro = compute_ldro(defaults.spreadingFactor(), defaults.bandwidth());
        this->payload_client->startReceiving(425450000,
                                             sf_to_rsf(defaults.spreadingFactor()),
                                             bw_to_rbw(defaults.bandwidth()),
                                             cr_to_rcr(defaults.codingRate()),
                                             (ldro ? RadioClient::LDR_On : RadioClient::LDR_Off));
    });

    QObject::connect(payload_client,
                     &RadioClient::beganReceiving,
                     this,
                     &RadioPacketParser::startedReceiving,
                     Qt::QueuedConnection);

    QObject::connect(payload_client,
                     &RadioClient::finishedTransmitting,
                     this,
                     &RadioPacketParser::finishedTransmitting,
                     Qt::QueuedConnection);

    payload_client->connect("/tmp/radio_serverD");
}

const LoraSettings &RadioPacketParser::defaultLoraSettings() const
{
    return defaultSettings;
}
void RadioPacketParser::finishedTransmitting(QDateTime time)
{
    lastTxDateTime = time;
    emit latestTxDateTimeChanged();
}

QDateTime RadioPacketParser::latestTxDateTime(){
    return lastTxDateTime;
}
QDateTime RadioPacketParser::latestRxDateTime(){
    return lastRxDateTime;
}

void RadioPacketParser::packetReceived(QDateTime time, int snr, int rssi, const QByteArray &packet)
{
    P2GLinkHeader header;
    UnpackResult res = unpack_p2g_link_header((uint8_t *) packet.data(), packet.size(), &header);
    if (res != UnpackResult::UnpackResult_AllGood) {
        qDebug("failed to unpack reason %d", res);
        return;
    }
    const uint8_t *rest = (uint8_t *) packet.constData() + 1;
    uint32_t rest_len = packet.size() - 1;

    CommandResponse cmd_resp = {};
    ImageData data = {0};

    switch (header.packet_type) {
    case P2GPacketType::P2GPacketType_CommandResponse:
        res = unpack_command_response(rest, rest_len, &cmd_resp);
        if (res != UnpackResult_AllGood) {
            qDebug("Failed to unpack command response: %d", (int) res);
            break;
        }
        qDebug("Got command response for cm type %d", (int) cmd_resp.cmd);
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
    lastRxDateTime = QDateTime::currentDateTime();
    emit latestRxDateTimeChanged();

    radio_snr.newValue(time, snr);
    radio_rssi.newValue(time, rssi);
    emit radioSNRChanged();
    emit radioRSSIChanged();
    emit packetReceivedFromRadio(time, snr, rssi, packet);
}

FrontBackDataHolder *RadioPacketParser::getRadioSNR()
{
    return &radio_snr;
}
FrontBackDataHolder *RadioPacketParser::getRadioRSSI()
{
    return &radio_rssi;
}

LoraSettings::SpreadingFactor rsf_to_sf(RadioClient::SF sf)
{
    switch (sf) {
    case RadioClient::SF5:
        return LoraSettings::SpreadingFactor::SF5;
    case RadioClient::SF6:
        return LoraSettings::SpreadingFactor::SF6;
    case RadioClient::SF7:
        return LoraSettings::SpreadingFactor::SF7;
    case RadioClient::SF8:
        return LoraSettings::SpreadingFactor::SF8;
    case RadioClient::SF9:
        return LoraSettings::SpreadingFactor::SF9;
    case RadioClient::SF10:
        return LoraSettings::SpreadingFactor::SF10;
    case RadioClient::SF11:
        return LoraSettings::SpreadingFactor::SF11;
    case RadioClient::SF12:
        return LoraSettings::SpreadingFactor::SF12;
    }
    return LoraSettings::SpreadingFactor::SF7;
}
LoraSettings::Bandwidth rbw_to_bw(RadioClient::BW bw)
{
    switch (bw) {
    case RadioClient::BW8:
        return LoraSettings::Bandwidth::BW8;
    case RadioClient::BW10:
        return LoraSettings::Bandwidth::BW10;
    case RadioClient::BW15:
        return LoraSettings::Bandwidth::BW15;
    case RadioClient::BW20:
        return LoraSettings::Bandwidth::BW20;
    case RadioClient::BW31:
        return LoraSettings::Bandwidth::BW31;
    case RadioClient::BW42:
        return LoraSettings::Bandwidth::BW41;
    case RadioClient::BW62:
        return LoraSettings::Bandwidth::BW62;
    case RadioClient::BW125:
        return LoraSettings::Bandwidth::BW125;
    case RadioClient::BW250:
        return LoraSettings::Bandwidth::BW250;
    case RadioClient::BW500:
        return LoraSettings::Bandwidth::BW500;
    default:
        // todo something about this
        return LoraSettings::Bandwidth::BW125;
    }
}

LoraSettings::CodingRate rcr_to_cr(RadioClient::CR cr)
{
    switch (cr) {
    case RadioClient::CR4_5:
        return LoraSettings::CodingRate::CR4_5;
    case RadioClient::CR4_6:
        return LoraSettings::CodingRate::CR4_6;
    case RadioClient::CR4_7:
        return LoraSettings::CodingRate::CR4_7;
    case RadioClient::CR4_8:
        return LoraSettings::CodingRate::CR4_8;
    }
    return LoraSettings::CodingRate::CR4_8;
}

RadioClient::SF sf_to_rsf(LoraSettings::SpreadingFactor sf)
{
    switch (sf) {
    case LoraSettings::SpreadingFactor::SF5:
        return RadioClient::SF::SF5;
    case LoraSettings::SpreadingFactor::SF6:
        return RadioClient::SF::SF6;
    case LoraSettings::SpreadingFactor::SF7:
        return RadioClient::SF::SF7;
    case LoraSettings::SpreadingFactor::SF8:
        return RadioClient::SF::SF8;
    case LoraSettings::SpreadingFactor::SF9:
        return RadioClient::SF::SF9;
    case LoraSettings::SpreadingFactor::SF10:
        return RadioClient::SF::SF10;
    case LoraSettings::SpreadingFactor::SF11:
        return RadioClient::SF::SF11;
    case LoraSettings::SpreadingFactor::SF12:
        return RadioClient::SF::SF12;
        break;
    }
    return RadioClient::SF::SF7;
}
RadioClient::BW bw_to_rbw(LoraSettings::Bandwidth bw)
{
    switch (bw) {
    case LoraSettings::Bandwidth::BW8:
        return RadioClient::BW::BW8;
    case LoraSettings::Bandwidth::BW10:
        return RadioClient::BW::BW10;
    case LoraSettings::Bandwidth::BW15:
        return RadioClient::BW::BW15;
    case LoraSettings::Bandwidth::BW20:
        return RadioClient::BW::BW20;
    case LoraSettings::Bandwidth::BW31:
        return RadioClient::BW::BW31;
    case LoraSettings::Bandwidth::BW41:
        return RadioClient::BW::BW42;
    case LoraSettings::Bandwidth::BW62:
        return RadioClient::BW::BW62;
    case LoraSettings::Bandwidth::BW125:
        return RadioClient::BW::BW125;
    case LoraSettings::Bandwidth::BW250:
        return RadioClient::BW::BW250;
    case LoraSettings::Bandwidth::BW500:
        return RadioClient::BW::BW500;
        break;
    }
    return RadioClient::BW::BW125;
}
RadioClient::CR cr_to_rcr(LoraSettings::CodingRate cr)
{
    switch (cr) {
    case LoraSettings::CodingRate::CR4_5:
        return RadioClient::CR::CR4_5;
    case LoraSettings::CodingRate::CR4_6:
        return RadioClient::CR::CR4_6;
    case LoraSettings::CodingRate::CR4_7:
        return RadioClient::CR::CR4_7;
    case LoraSettings::CodingRate::CR4_8:
        return RadioClient::CR::CR4_8;
        break;
    }
    return RadioClient::CR::CR4_5;
}

void RadioPacketParser::startedReceiving(QDateTime time,
                                         uint32_t freq_hz,
                                         RadioClient::SF sf,
                                         RadioClient::BW bw,
                                         RadioClient::CR cr,
                                         RadioClient::LDR ldr)
{
    currentStableRadioSettings.setFrequency(freq_hz);
    currentStableRadioSettings.setSpreadingFactor(rsf_to_sf(sf));
    currentStableRadioSettings.setBandwidth(rbw_to_bw(bw));
    currentStableRadioSettings.setCodingRate(rcr_to_cr(cr));
    qDebug("radio confirmed new Lora settings %d %s %s %s %s",
           freq_hz,
           RadioClient::SF_Str(sf),
           RadioClient::BW_Str(bw),
           RadioClient::CR_Str(cr),
           (ldr == RadioClient::LDR_On) ? "LDRON" : "LDROFF");

    emit loraSettingsChanged();
}

Q_INVOKABLE void RadioPacketParser::setLocalLoraParams(uint32_t freq_hz,
                                                       LoraSettings::SpreadingFactor sf,
                                                       LoraSettings::Bandwidth bw,
                                                       LoraSettings::CodingRate cr)
{
    bool ldr = compute_ldro(sf, bw);

    payload_client->startReceiving(freq_hz,
                                   sf_to_rsf(sf),
                                   bw_to_rbw(bw),
                                   cr_to_rcr(cr),
                                   ldr ? RadioClient::LDR_On : RadioClient::LDR_Off);
}

// SpreadingFactor sf_to_lsf(LoraSettings::SpreadingFactor sf)
// {
//     switch (sf) {};
// }
// Bandwidth bw_to_lbw(LoraSettings::Bandwidth bw)
// {
//     switch (bw) {};
// }
// CodingRate cr_to_lcr(LoraSettings::CodingRate cr)
// {
//     switch (cr) {};
// }

void RadioPacketParser::negotiateLoraParams(uint32_t freq_hz,
                                            LoraSettings::SpreadingFactor sf,
                                            LoraSettings::Bandwidth bw,
                                            LoraSettings::CodingRate cr,
                                            int8_t remoteDbm)
{
    // LoraLinkChange settings{2, remoteDbm, freq_hz, sf_to_lsf(sf), sf_to_lsf(bw), cr_to_lcr(cr)};
}

void RadioPacketParser::sendArmTarget() {}

void RadioPacketParser::sendToPhase(FlightPhaseQML phase)
{
    CommandAndData cmd;
    switch (phase) {
    case FlightPhaseQML::Pad:
        cmd.command = Command_BackToPad;
        break;
    case FlightPhaseQML::Expecting:
        cmd.command = Command_ExpectFlight;
        break;
    case FlightPhaseQML::Flight:
        cmd.command = Command_ForceFlight;
        break;
    case FlightPhaseQML::LandedManual:
        cmd.command = Command_ForceManual;
        break;
    default:
        qWarning("Not a phase we can go to");
        return;
    }
    sendCommand(&cmd);
}

void RadioPacketParser::sendCallsign()
{
    CommandAndData cmd{};
    cmd.command = Command_Callsign;
    memcpy(cmd.callsign.buf, "KC1TPR", 6);
    sendCommand(&cmd);
}

void RadioPacketParser::takeStillPicture(PhotoTransformQ xform)
{
    CommandAndData cmd;
    cmd.command = Command_TakePicture;
    cmd.take_picture = PhotoTransform{xform.left,
                                      xform.right,
                                      xform.top,
                                      xform.bottom,
                                      xform.encodedWidth,
                                      xform.encodedQuality};

    sendCommand(&cmd);
}

void RadioPacketParser::askForBlocks(uint8_t image_id, const std::vector<uint16_t> &block_ids)
{
    ImageBlockRequest req;
    req.image_id = image_id;
    req.num = std::min(block_ids.size(), (size_t) MAX_BLOCKS_PER_REQUEST);
    qInfo("Requesting %d blocks for image %d", req.num, image_id);

    for (size_t i = 0; i < req.num; i++) {
        req.block_ids[i] = block_ids[i];
    }

    uint8_t buf[256] = {0};
    G2PLinkHeader header{G2PPacketType_ImageControl, 0};
    pack_g2p_link_header(&header, buf);
    size_t size = pack_image_block_request(&req, buf + 1);
    qInfo("%lu bytes worth of packet after header", size);
    sendPacket(size + 1, buf);
}

void RadioPacketParser::sendCommand(CommandAndData *cmd)
{
    uint8_t buf[256] = {0};
    G2PLinkHeader header{G2PPacketType_Command, 0};
    pack_g2p_link_header(&header, &buf[0]);
    size_t len = pack_command_and_data(cmd, &buf[1]);
    sendPacket(len + 1, &buf[0]);
}

void RadioPacketParser::sendPacket(size_t len, uint8_t *buf)
{
    auto arr = QByteArray::fromRawData((const char *) buf, len);
    bool ldro = compute_ldro(currentStableRadioSettings.spreadingFactor(),
                             currentStableRadioSettings.bandwidth());
    payload_client->transmit(currentStableRadioSettings.frequency(),
                             sf_to_rsf(currentStableRadioSettings.spreadingFactor()),
                             bw_to_rbw(currentStableRadioSettings.bandwidth()),
                             cr_to_rcr(currentStableRadioSettings.codingRate()),
                             ldro ? RadioClient::LDR_On : RadioClient::LDR_Off,
                             14,
                             arr);
}

LoraSettings *RadioPacketParser::loraSettings()
{
    return &currentStableRadioSettings;
}

void RadioPacketParser::askForTelemetryInt(uint8_t typ)
{
    askForTelemetry((TelemetryType) typ);
}

void RadioPacketParser::askForFlightHeartbeat()
{
    askForTelemetry(TelemetryType_FlightHeartbeat);
}

void RadioPacketParser::askForLandedHeartbeat()
{
    askForTelemetry(TelemetryType_LandedHeartbeat);
}

void RadioPacketParser::askForMetadata(uint8_t image_id)
{
    CommandAndData cmd;
    cmd.command = Command_ImageMetadata;
    cmd.metadata_ask_image_id = image_id;
    sendCommand(&cmd);
}

void RadioPacketParser::askForTelemetry(TelemetryType typ)
{
    CommandAndData cmd;
    cmd.command = Command_TelemetryRequest;
    cmd.telem_request.telem_type = typ;
    sendCommand(&cmd);
}

void RadioPacketParser::askToGoToPosition(int8_t syaw, int8_t spitch, int8_t epitch, int8_t wpitch)
{
    qDebug("Asking to go to arm target");
    CommandAndData cmd;
    cmd.command = Command_SendArmTarget;
    cmd.send_arm_to_target.shoulder_yaw = syaw;
    cmd.send_arm_to_target.shoulder_pitch = spitch;
    cmd.send_arm_to_target.elbow_pitch = epitch;
    cmd.send_arm_to_target.wrist_pitch = wpitch;
    sendCommand(&cmd);
}

void RadioPacketParser::askToGoToPositionAndComeBack(int8_t syaw,
                                                     int8_t spitch,
                                                     int8_t epitch,
                                                     int8_t wpitch)
{
    qDebug("Asking to go to arm target and come back");
    CommandAndData cmd;
    cmd.command = Command_SendArmTargetAndComeBack;
    cmd.send_arm_to_target.shoulder_yaw = syaw;
    cmd.send_arm_to_target.shoulder_pitch = spitch;
    cmd.send_arm_to_target.elbow_pitch = epitch;
    cmd.send_arm_to_target.wrist_pitch = wpitch;
    sendCommand(&cmd);
}

void RadioPacketParser::emitTelemetry(QDateTime time, const Telemetry *telem)
{
    qDebug("Got Telem %d", telem->telem_type);
    switch (telem->telem_type) {
    case TelemetryType_FlightHeartbeat:
        printf("Emitting telem: PHASE   %d\n", (int) (telem->flight_heartbeat_stats.state.phase));
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
        qInfo("New landed heartbeat. Image id %d",
              (int) telem->landed_heartbeat_stats.next_image_id);
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
        if (telem->landed_heartbeat_stats.next_image_id > last_image_id) {
            last_image_id = telem->landed_heartbeat_stats.next_image_id;
            emit numImagesIncreased(time, last_image_id);
        }
        emit armAnglesUpdated(time,
                              telem->landed_heartbeat_stats.arm_position.shoulder_yaw,
                              telem->landed_heartbeat_stats.arm_position.shoulder_pitch,
                              telem->landed_heartbeat_stats.arm_position.elbow_pitch,
                              telem->landed_heartbeat_stats.arm_position.wrist_pitch);
        break;

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
        qDebug("Unhandled emitTelemetry of type", telem->telem_type);
        break;
    }
}

void RadioPacketParser::emitCommandResponse(QDateTime time, const CommandResponse *resp)
{
    switch (resp->cmd) {
    case Command_ForceManual:
    case Command_ForceFlight:
    case Command_ExpectFlight:
    case Command_BackToPad:
    case Command_StartVideo:
    case Command_StopVideo:
    case Command_TakePicture:
    case Command_ReCrop:
    case Command_SendArmTargetAndComeBack:
    case Command_SendArmTargetForPhotoAndComeBack:
    case Command_SendIdlePosition:
    case Command_ZeroShoulder_AssumeOpen:
    case Command_RunOpenSequence:
    case Command_ShellExec:
    case Command_NewFlightDanger:
    case Command_Callsign:
    case Command_SendArmTarget:
    case Command_MaxCommand:
        qDebug("COMMAND RESPONSERECEIVED WITH NO EMIT HANDLER");
        break;
    case Command_ShellExecInfo:
        break;
    case Command_ShellReadStdout:
        break;
    case Command_ShellReadStderr:
        break;
    case Command_ImageMetadata:
        qDebug("Radio packet parser got metadata for id %d", resp->image_metadata.image_id);
        if (resp->image_metadata.image_id > last_image_id) {
            emit numImagesIncreased(time, resp->image_metadata.image_id);
            last_image_id = resp->image_metadata.image_id;
        }
        emit ImageMetadataReceived(time, resp->image_metadata);
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
        return "Flip";
    case LandedUnfolding:
        return "Unfold";
    case LandedAutomaticCamera:
        return "AutoCamera";
    case LandedManual:
        return "LManual";
    }
    return QString{"?%d?"}.arg((int) phase);
}

PayloadFlags RadioPacketParser::statusBitsToFlags(uint16_t bits)
{
    return PayloadFlags::fromBits(bits);
}
