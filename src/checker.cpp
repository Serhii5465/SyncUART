#include "SyncUART.h"

int8_t SyncUART::is_in_scopes(sens_data args){
    if (args.low_threshold >= args.current_value){
        return -1;
    } else if (args.high_threshold <= args.current_value){
        return 1;
    } else {
        return 0;
    }
}