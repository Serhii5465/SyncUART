#include <SyncUART.h>

uint8_t SyncUART::get_width_field(float val){
    if(val >= 100 || val <= -10){
        return 6;
    } else if ((val < 100 && val >= 10) || (val < 0 && val > -10)){
        return 5;
    } else {
        return 4;
    }
}