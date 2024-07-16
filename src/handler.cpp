#include "SyncUART.h"

buff_param p;

uint8_t fail_count = 0;

void SyncUART::init_buff_data_sens(){
    strlcpy(p.buffer, CODES_SUCCESS::TRANSF_BUFF_DATA_SENS, sizeof(CODES_SUCCESS::TRANSF_BUFF_DATA_SENS));

    strlcpy(&p.buffer[strlen(CODES_SUCCESS::TRANSF_BUFF_DATA_SENS)], DELIMITERS::MARK_START_BUFF_DATA_POLL_SENS,
                        sizeof(DELIMITERS::MARK_START_BUFF_DATA_POLL_SENS));

    p.offset = strlen(p.buffer);
}

void SyncUART::reset_buff_data_sens(){
    p.offset = strlen(CODES_SUCCESS::TRANSF_BUFF_DATA_SENS) + strlen(DELIMITERS::MARK_START_BUFF_DATA_POLL_SENS);                                              
    p.buffer[p.offset] = '\0';
    p.str_val_sen[0] = '\0';
}

void SyncUART::init_buff_unk_addr_sens(char* buff_err_msg){
    char msg[] = "UNK_ADDR:";

    uint8_t index = 0;
    
    strlcpy(buff_err_msg, CODES_FAILS::INIT_PERH, sizeof(CODES_FAILS::INIT_PERH));

    index += strlen(CODES_FAILS::INIT_PERH);
    strlcpy(&buff_err_msg[index], DELIMITERS::MARK_START_BUFF_ERR_DATA, sizeof(DELIMITERS::MARK_START_BUFF_ERR_DATA));

    index += strlen(DELIMITERS::MARK_START_BUFF_ERR_DATA);
    strlcpy(&buff_err_msg[index], msg, sizeof(msg));
}

bool SyncUART::wait_command(char* buffer, char* recv_cmd, char* last_code_cmd){
    bool is_ready = false;
    read_serial(last_code_cmd, buffer);

    if(check_start_byte(buffer) != true){
        send_command(CODES_FAILS::UNK_CMD);
        return false;
    } else {
        strlcpy(recv_cmd, buffer + 1, PARAM_BUFFERS::SIZE_BUFF_RECV_CMD);
        is_ready = true;
    }
    return is_ready;
}

bool SyncUART::ping_master(const char* id_slave, const char* type_slave, void (*handler_find_perph)(bool*)){
    char* last_code_cmd = (char*) CODES_SUCCESS::CONFM_PRESENCE;

    char buf_inc_data[PARAM_BUFFERS::SIZE_BUFF_RECV_DATA] = {0};
    char code_recv_cmd[PARAM_BUFFERS::SIZE_BUFF_RECV_CMD] = {0};

    bool is_ready = false;
    
    while(wait_command(buf_inc_data, code_recv_cmd, last_code_cmd) != true);

    if(strcmp(code_recv_cmd, CODES_MANAGE::PRESENCE) == 0){
        print(buf_inc_data, true);
        send_command(CODES_SUCCESS::CONFM_PRESENCE);

    } else if(strcmp(code_recv_cmd, CODES_MANAGE::CHECK_ID_SLAVE) == 0){
        print(buf_inc_data, true);
        handler_id_slv(id_slave, buf_inc_data, type_slave);
        
        // send_command("564643");
        // Serial.print("564564564\n");
        
    } else if(strcmp(code_recv_cmd, CODES_MANAGE::INIT_PERH) == 0){
        print(buf_inc_data, true);
        handler_find_perph(&is_ready);

    } else {
        check_valid_data(code_recv_cmd, buf_inc_data);
    }

    return is_ready;
}

void SyncUART::parse_UART(void (*fun_poll_sens)(), void (*get_data_sens)(buff_param*), void (*handl_buff_data_act_dev)(char*)){
    static char* s_last_code_cmd = (char*) CODES_SUCCESS::CONFM_INIT_PERH;

    char buf_inc_data[PARAM_BUFFERS::SIZE_BUFF_RECV_DATA] = {0};
    char code_recv_cmd[PARAM_BUFFERS::SIZE_BUFF_RECV_CMD] = {0};

    while(wait_command(buf_inc_data, code_recv_cmd, s_last_code_cmd) != true);

    if(strcmp(code_recv_cmd, CODES_MANAGE::POLL_SENS) == 0){
        print(buf_inc_data, true);
        // send_command("564643");
        // Serial.print("564564564\n");

        fun_poll_sens();
        send_command(CODES_SUCCESS::CONFM_POLL_SENS);
        s_last_code_cmd = (char*) CODES_SUCCESS::CONFM_POLL_SENS;

    } else if(strcmp(code_recv_cmd, CODES_MANAGE::GET_BUFF_DATA_SENS) == 0){
        print(buf_inc_data, true);
        get_data_sens(&p);
        send_command(p.buffer);
        s_last_code_cmd = (char*) CODES_SUCCESS::CONFM_INIT_PERH;
        reset_buff_data_sens();

    } else if(strcmp(code_recv_cmd, CODES_MANAGE::MANAGE_ACT_DEV) == 0){
        print(buf_inc_data, true);
        send_command(CODES_SUCCESS::CONFM_MANAGE_ACT_DEV);
        s_last_code_cmd = (char*) CODES_SUCCESS::CONFM_MANAGE_ACT_DEV;

        // send_command("564643");
        // Serial.print("564564564\n");

    } else if(strcmp(code_recv_cmd, CODES_MANAGE::TRANSF_BUFF_ACT_DEV) == 0){
        print(buf_inc_data, true);
        handl_buff_data_act_dev(buf_inc_data);
        send_command(CODES_SUCCESS::CONFM_RECV_BUF_DATA_ACT_DEV);
        s_last_code_cmd = (char*) CODES_SUCCESS::TRANSF_BUFF_DATA_SENS;

        // send_command("564643");
        // Serial.print("564564564\n");

    } else {
        check_valid_data(code_recv_cmd, buf_inc_data);
    }
}

void SyncUART::check_valid_data(const char* cmd, const char* inc_buff){
    if(strcmp(cmd, CODES_FAILS::INIT_BLOCK) == 0){
        print(inc_buff, true);
        reset_serial();
    } else {
        print(inc_buff, false);
        send_command(CODES_FAILS::UNK_CMD);
    }
}

bool SyncUART::check_start_byte(const char* buffer){
    if(buffer[0] != DELIMITERS::START_BYTE[0]){
        print(buffer, false);
        return false;
    } else {
        return true;
    }
}

void SyncUART::handler_id_slv(const char* stated_id, char* inc_buff, const char* type_slv){
    char *offer_id;

    strtok(inc_buff, DELIMITERS::MARK_START_BUF_INFO_SLV);
    offer_id = strtok(NULL, DELIMITERS::MARK_START_BUF_INFO_SLV);
    
    uint8_t index = 0;

    if(strcmp(stated_id, offer_id) == 0){ 
        char c[20] = {0};
        
        strlcpy(c, CODES_SUCCESS::TRANSF_TYPE_SLV, sizeof(CODES_SUCCESS::TRANSF_TYPE_SLV));

        index = strlen(CODES_SUCCESS::TRANSF_TYPE_SLV);
        strlcpy(&c[index], DELIMITERS::MARK_START_BUF_INFO_SLV, sizeof(DELIMITERS::MARK_START_BUF_INFO_SLV));
        
        index += strlen(DELIMITERS::MARK_START_BUF_INFO_SLV);
        strlcpy(&c[index], type_slv, sizeof(TYPE_SLAVE::POLL_SENS));

        send_command(c);
    } else {
        char err[60] = {0}; 
        char dcl_id_msg[] = "DECLR_ID_SLAVE: "; 
        char unk_id_msg[] = "UNK_ID_SLAVE: ";

        strlcpy(err, CODES_FAILS::CHECK_ID_SLV, sizeof(CODES_FAILS::CHECK_ID_SLV));

        index = strlen(CODES_FAILS::CHECK_ID_SLV);
        strlcpy(&err[index], DELIMITERS::MARK_START_BUFF_ERR_DATA, sizeof(DELIMITERS::MARK_START_BUFF_ERR_DATA));

        index += strlen(DELIMITERS::MARK_START_BUFF_ERR_DATA);
        strlcpy(&err[index], dcl_id_msg, sizeof(dcl_id_msg));

        index += strlen(dcl_id_msg);
        strlcpy(&err[index], stated_id, strlen(stated_id) + 1);

        index += strlen(stated_id);
        strlcpy(&err[index], ",", sizeof(","));

        index += strlen(",");
        strlcpy(&err[index], unk_id_msg, sizeof(unk_id_msg));

        index += strlen(unk_id_msg);
        strlcpy(&err[index], offer_id, strlen(offer_id) + 1);

        send_command(err);
    }
}