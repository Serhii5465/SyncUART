#include "SyncUART.h"

void SyncUART::reset_board(){
    delay(150);
    asm volatile ("jmp 0x0");
}

void SyncUART::reset_serial(){
    DEBUG_PRINT("BLOCK FAILED. Connection closed");
    Serial.end();
    while(1);
}