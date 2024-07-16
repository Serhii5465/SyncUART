#ifndef PARAMS_H
#define PARAMS_H

#include <Arduino.h>

/**
 * @namespace PARAM_CONNECT
 * @brief Namespace containing connection parameters.
 *
 * The PARAM_CONNECT namespace provides constants representing connection parameters.
 * The SERIAL_TO constant defines the timeout duration for serial communication.
 * The INTER_PING_MASTER constant defines the interval for pinging the master.
 * The COUNT_TRY_RECON constant represents the number of tries for reconnection.
 */
namespace PARAM_CONNECT{
    const uint16_t SERIAL_TO = 3000; 
    const uint16_t INTER_PING_MASTER = 500;
    const uint8_t COUNT_TRY_RECON = 4;
}

namespace PARAM_BUFFERS{
    const uint8_t SIZE_BUFF_ID_SENS = 25; /*Defines the size of the buffer used to store the ID of a senso*/
    const uint8_t SIZE_BUFF_RECV_DATA = 50; /**< Size of the buffer for received data */
    const uint8_t SIZE_BUFF_RECV_CMD = 3; /**< Size of the buffer for received commands */
    const uint8_t SIZE_BUF_PACK_VAL_SENS = 70; /**< Size of the buffer for packing sensor values */
    const uint8_t SIZE_BUF_STR_VAL_SENS = 15; /**< Size of the buffer for string sensor values */
    const uint8_t SIZE_BUFF_ERR_MSG = 150; /**< Size of the buffer for error messages */
}

/**
 * @namespace ID_SLAVES_MASTER_1
 * @brief Namespace containing ID definitions for slaves of Master module #1.
 */
namespace ID_SLAVES_MASTER_1{
    const char DEV_1[] = "MR1_SL01";
    const char DEV_2[] = "MR1_SL02";
    const char DEV_3[] = "MR1_SL03";
}

/**
 * @namespace TYPE_SLAVE
 * @brief Namespace containing definitions for slave types.
 *
 * The TYPE_SLAVE namespace provides constants representing different types of slaves.
 * The constant POLL_SENS represents the type slave, which only polling sensors,
 * and MNG_ACT_DEV represents the slave, which polling sensors and managing active devices.
 */
namespace TYPE_SLAVE{
    const char POLL_SENS[] = "PLL";
    const char MNG_ACT_DEV[] = "MNG";
}

/**
 * @namespace MOD_FORMAT_DATA_SENS
 * @brief Namespace containing formatting strings for sensor data.
 *
 * The MOD_FORMAT_DATA_SENS namespace provides constants representing formatting strings for sensor data.
 * The constants STR and INT store the formatting strings for string and integer sensor values, respectively.
 */
namespace MOD_FORMAT_DATA_SENS{
    const char STR[] = "%s^"; 
    const char INT[] = "%i\t";
}

#endif