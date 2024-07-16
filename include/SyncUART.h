#ifndef SYNC_UART_H
#define SYNC_UART_H

#include <Arduino.h>

#include "code_cmd.h"
#include "delim.h"
#include "params.h"

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    #define DEBUG_PRINT(x) Serial1.print(x)
    #define DEBUG_PRINTLN(x) Serial1.println(x)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
#endif

/**
 * @struct sens_data
 * @brief Structure representing sensor data.
 * 
 * The sens_data structure contains information about the temperature sensor, including its ID, address, 
 * threshold values, current temperature value, and a code indicating if the temperature is within a defined range.
 */
typedef struct {
    char id_sens[PARAM_BUFFERS::SIZE_BUFF_ID_SENS];
    byte addr[8];
    float low_threshold;
    float high_threshold;
    float current_value;
    int8_t code;
} sens_data;

/**
 * @struct buff_param
 * @brief Structure representing buffer parameters.
 *
 * The buff_param structure contains a buffer, a string value sensor, and an offset.
 * It is used to hold parameters for buffer-related operations.
 */
typedef struct {
    char buffer[PARAM_BUFFERS::SIZE_BUF_PACK_VAL_SENS]; /**< Buffer for storing packed sensor values */
    char str_val_sen[PARAM_BUFFERS::SIZE_BUF_STR_VAL_SENS]; /**< String representation of the sensor value */
    uint8_t offset; /**< Offset value */
} buff_param;

class SyncUART{
    public:
        // handler.cpp
        /**
         * @brief Initialize the buffer for sensor data.
         *
         * The init_buff_data_sens function initializes the buffer used for storing sensor data.
         */
        void init_buff_data_sens();

        /**
         * @brief Ping the master device and perform appropriate actions based on received commands.
         *
         * The ping_master function pings the master device using the provided ID and type of slave.
         * It takes the ID and type of the slave, as well as a function pointer to a handler function, as input.
         * This function used to establish communication with the master device and check its presence.
         * It waits for commands from the master, performs slave ID verification and initialisation of peripherals
         * It returns a boolean value indicating if the ping to the master was successful and peripheral devices are ready.
         *
         * @param id_slave Pointer to a null-terminated character array representing the ID of the slave.
         * @param type_slave Pointer to a null-terminated character array representing the type of slave.
         * @param handler_find_perph Function pointer to the function responsible for finding peripheral devices.
         * @return Boolean value indicating if the ping to the master was successful (true) and peripheral devices are ready.
         */
        bool ping_master(const char* id_slave, const char* type_slave, void (*handler_find_perph)(bool*));

        /**
         * @brief Parse UART data.
         *
         * The parse_UART function parses UART data using provided function pointers for polling sensors, getting sensor data,
         * and handling buffer data for active devices.
         *
         *
         * @param fun_poll_sens Pointer to the function responsible for polling sensors.
         * @param get_data_sens Pointer to the function responsible for getting sensor data.
         * @param handl_buff_data_act_dev Pointer to the function responsible for handling buffer data for actuators, which receiving from master module.
         */
        void parse_UART(void (*fun_poll_sens)(), void (*get_data_sens)(buff_param*), void (*handl_buff_data_act_dev)(char*));

        /**
         * @brief Initialize the buffer for unknown address sensors.
         *
         * The init_buff_unk_addr_sens function initializes the buffer used for unknown address sensors.
         * It takes a character array buffer as input and initializes it with an error message.
         * This function is commonly used to prepare the buffer for reporting errors related to unknown address sensors.
         *
         * @param buff_err_msg Pointer to a character array representing the buffer for error messages.
         */
        void init_buff_unk_addr_sens(char* buff_err_msg);
        // end handler.cpp

        // sender.cpp
        /**
         * @brief Confirmation initialization of periphery.
         *
         * Send an acknowledgement signal for the initial initialisation of the peripherals of microcontroller.
         */
        void send_cfm_init();
        /**
         * @brief Send a command.
         *
         * The send_command function sends a command through the serial communication.
         *
         * @param command Pointer to a null-terminated character array representing the command to send.
         */
        void send_command(const char* command);
        // end sender.cpp

        // formatter.cpp
        uint8_t get_width_field(float val);
        // end formatter.cpp

        // checker.cpp
        /**
         * @brief Checks if the value for a sensor is within the defined range.
         * 
         * This function evaluates if value of a sensor, represented by the sens_data structure, 
         * falls within the defined range indicated by the low_threshold and high_threshold fields.
         * 
         * @param args The sens_data structure representing the sensor data.
         * @return An int8_t value indicating if it is within the defined range:
         *         - -1 if the value is below the low threshold.
         *         - 0 if the value is within the defined range.
         *         - 1 if the value is above the high threshold.
         */
        int8_t is_in_scopes(sens_data args);
        // end checker.cpp

    private:
        // handler.cpp
        /**
         * @brief Reset the buffer for sensor data.
         *
         * The reset_buff_data_sens function resets the buffer used for storing sensor data.
         * It clears the buffer and prepares it for new data collection.
         */
        void reset_buff_data_sens();

        /**
         * @brief Check if the start byte is present in the buffer.
         *
         * The check_start_byte function checks if the start byte is present at the beginning of the buffer.
         *
         * @param buffer Pointer to a null-terminated character array representing the buffer to check.
         * @return Boolean value indicating if the start byte is present (true) or not (false).
         */
        bool check_start_byte(const char* buffer);

        /**
         * @brief Wait for a command.
         *
         * The wait_command function waits for a command to be received through the serial communication from master module..
         *
         * @param buffer Pointer to a character array where the received data will be stored.
         * @param recv_cmd Pointer to a character array where the received command will be stored.
         * @param last_code_cmd Pointer to a character array representing the last code command.
         * @return Boolean value indicating if a command was received successfully (true) or not (false).
         */
        bool wait_command(char* buffer, char* recv_cmd, char* last_code_cmd);

        /**
         * @brief Handle the ID of a slave device and update the incoming buffer.
         *
         * It compares the stated ID with the offered ID to determine the appropriate action.
         * If the IDs match, a success message with the transferred type of slave is sent.
         * If the IDs do not match, an error message with the declared and unknown IDs is sent.
         *
         * @param stated_id Pointer to a null-terminated character array representing the stated ID.
         * @param inc_buff Pointer to a character array representing the incoming buffer to update.
         * @param type_slv Pointer to a null-terminated character array representing the type of slave.
         */
        void handler_id_slv(const char* stated_id, char* inc_buff, const char* type_slv);

        /**
         * @brief Check the validity of data based on the command and incoming buffer.
         *
         * The check_valid_data function checks the validity of data based on the provided command and incoming buffer.
         * It compares the command with failure codes.
         * If the command matches the failure code initialization, the serial communication is reset.
         * Otherwise, sending command of error receiving of managing command.
         *
         * @param cmd Pointer to a null-terminated character array representing the command to check.
         * @param inc_buff Pointer to a null-terminated character array representing the incoming buffer to printing for in debug serial monitor.
         */
        void check_valid_data(const char* cmd, const char* inc_buff);
        // end handler.cpp

        // sender.cpp
        
        // reader.cpp
        /**
         * @brief Check if a timeout has occurred.
         *
         * The check_timeout function checks whether a timeout has occurred since the last time 
         * data was received from the master module. 
         *
         * @return Boolean value indicating if a timeout has occurred (true) or not (false).
         */
        bool check_timeout();

        /**
         * @brief Read data from the serial port.
         *
         * The read_serial function reads data from the serial communication and stores it in a buffer.
         * It takes as input the last command that will be used to reconnect to the master unit
         * The buffer parameter is used to store the read data.
         *
         * If it is the first initialization, the function waits for data from the master and reads it into the buffer.
         * Subsequent calls to the function check for a timeout. If a timeout has occurred, the function reads data into the buffer.
         * If a timeout has not occurred, the function attempts to reconnect with the master by sending the last code command
         * multiple times with a delay between each attempt. If the reconnection attempts fail, the board is reset.
         *
         * @param last_code_cmd Pointer to a null-terminated character array representing the last code command.
         * @param buffer Pointer to a character array where the read data will be stored.
         */
        void read_serial(const char* last_code_cmd, char* buffer);
        // end reader.cpp

        // print.cpp
        /**
         * @brief Print a character buffer.
         *
         * Function takes a null-terminated character array as input and prints its content to the debug serial monitor.
         * The is_cmd_recv parameter indicates whether the buffer contains a received command.
         *
         * @param buf Pointer to a null-terminated character array representing the buffer to print.
         * @param is_cmd_recv Boolean value indicating if the buffer contains a received command.
         */
        void print(const char* buf, bool is_cmd_recv);
        //end print.cpp

        //reset.cpp
        /**
         * @brief Reset the board.
         *
         * The reset_board function performs a reset of the board, restoring it to its initial state.
         */
        void reset_board();

        /**
         * @brief Reset the serial communication.
         *
         * The reset_serial function resets the serial communication, clearing any buffered data and 
         * resetting the communication state.
         */
        void reset_serial();
        // end reset.cpp
};

#endif