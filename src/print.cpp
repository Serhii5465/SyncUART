#include "SyncUART.h"

void SyncUART::print(const char* buf, bool is_cmd_recv){
    if(is_cmd_recv){
        DEBUG_PRINT("Command received: ");
        DEBUG_PRINTLN(buf); 
    } else {
        DEBUG_PRINTLN("Wrong code command");
        DEBUG_PRINT("Value: ");
        DEBUG_PRINTLN(buf);
    }
}