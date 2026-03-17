#include "packets_p2g.h"
#include <string.h>

enum UnpackResult unpack_p2g_link_header(const uint8_t *buf, uint32_t len, struct P2GLinkHeader *header){
  if(len < 1){
    return UnpackResult_TooShort;
  }
  
  enum P2GPacketType typ = 0b11 & (buf[0] >> 6);
  uint8_t left = buf[0] & 0b111111; 
  header->packet_type = typ;
  header->expected_packets_before_response = left;

  return UnpackResult_AllGood;

}
int pack_p2g_link_header(uint8_t *buf);


enum UnpackResult unpack_image_data(const uint8_t *buf, uint32_t len, struct ImageData *header){
  if (len < IMAGE_DATA_SIZE){
    return UnpackResult_TooShort;
  }
  memcpy(header->buf, buf, IMAGE_DATA_SIZE);
  header->image_id = buf[IMAGE_ID_INDEX_IN_PACKET];
  uint8_t msb = buf[IMAGE_BLOCK_ID_INDEX_MSB_IN_PACKET];
  uint8_t lsb = buf[IMAGE_BLOCK_ID_INDEX_LSB_IN_PACKET];
  header->block_index = (msb << 8) | lsb;
  return UnpackResult_AllGood;
}





int pack_telemetry(const struct Telemetry *telem, uint8_t *buf){
  buf[0] = telem->telem_type;

  return 1;
}
enum UnpackResult unpack_telemetry(uint8_t *buf, uint32_t len, struct Telemetry *telem);

int pack_landed_heartbeat(const struct LandedHeartbeatStats *stats, uint8_t *buf){
  return 5;
}

enum UnpackResult unpack_landed_hearbeat(uint8_t *buf,
                                                uint32_t len,
                                                struct LandedHeartbeatStats *stats){
  if (len < 5){
    return UnpackResult_TooShort;
  }

  return UnpackResult_AllGood;
}


int pack_flight_heartbeat(const struct FlightHeartbeatStats *stats, uint8_t *buf){
  
  return 0;
}


enum UnpackResult unpack_flight_heartbeat(uint8_t *buf,
                                                uint32_t len,
                                                struct FlightHeartbeatStats *stats){

  return UnpackResult_AllGood;
}


enum UnpackResult unpack_command_response(const uint8_t *buf, uint32_t len, struct CommandResponse *resp){
  if (len < 1){
    return UnpackResult_TooShort;
  }

  resp->cmd = buf[0];
  switch (resp->cmd){
    
  default:
    return UnpackResult_UnknownCommand;

}


}
