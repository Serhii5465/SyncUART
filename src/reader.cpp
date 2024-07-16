#include "SyncUART.h"

bool SyncUART::check_timeout(){
    unsigned long last_check = millis();

    while(millis() - last_check < PARAM_CONNECT::SERIAL_TO){
        if(Serial.available() > 0){
            return true;
        }
    }
    return false;
}

void SyncUART::read_serial(const char* last_code_cmd, char* buffer){
    static bool s_frst_init = true;
    if(s_frst_init == true){
        DEBUG_PRINTLN("Waiting for master...");
        while (true){
            if(Serial.available() > 0){
                break;
            }
            delay(100);
        }
        Serial.readBytesUntil('\n', buffer, PARAM_BUFFERS::SIZE_BUFF_RECV_DATA);
        s_frst_init = false;
        return;
    }

    bool res = check_timeout();

    if(res == true){
        Serial.readBytesUntil('\n', buffer, PARAM_BUFFERS::SIZE_BUFF_RECV_DATA);
    } else {
        for (size_t i = 1; i <= PARAM_CONNECT::COUNT_TRY_RECON; i++){
            DEBUG_PRINTLN("Command timeout exceeded");
            DEBUG_PRINTLN("Reconnection...");
            DEBUG_PRINTLN("Count reconections: ");
            DEBUG_PRINTLN(i);
            send_command(last_code_cmd);
            delay(PARAM_CONNECT::INTER_PING_MASTER);
        }
        reset_board();
    }
}