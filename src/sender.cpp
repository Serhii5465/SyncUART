#include "SyncUART.h"

void SyncUART::send_cfm_init(){
    send_command(CODES_SUCCESS::CONFM_INIT_PERH);
}

void SyncUART::send_command(const char* command){
    Serial.write(DELIMITERS::START_BYTE);
    Serial.write(command);
    Serial.write(DELIMITERS::END_BYTE);
}