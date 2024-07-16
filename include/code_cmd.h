#ifndef COMMAND_H
#define COMMAND_H

namespace CODES_MANAGE{
    const char PRESENCE[] = "10";
    const char CHECK_ID_SLAVE[] = "11";
    const char INIT_PERH[] = "12";
    const char POLL_SENS[] = "13";
    const char GET_BUFF_DATA_SENS[] = "14";
    const char MANAGE_ACT_DEV[] = "15";
    const char TRANSF_BUFF_ACT_DEV[] = "16";
}

namespace CODES_SUCCESS{
    const char CONFM_PRESENCE[] = "21";
    const char TRANSF_TYPE_SLV[] = "22";
    const char CONFM_INIT_PERH[] = "23";
    const char CONFM_POLL_SENS[] = "24";
    const char TRANSF_BUFF_DATA_SENS[] = "25";
    const char CONFM_MANAGE_ACT_DEV[] = "26";
    const char CONFM_RECV_BUF_DATA_ACT_DEV[] = "27";
}

namespace CODES_FAILS{
    const char UNK_CMD[] = "30|Wrong data received";
    const char INIT_BLOCK[] = "31";
    const char CHECK_ID_SLV[] = "32";
    const char INIT_PERH[] = "33";
}

#endif