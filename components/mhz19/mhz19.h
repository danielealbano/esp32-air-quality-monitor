#ifndef MHZ19_H
#define MHZ19_H

#define MHZ19_REQUEST_TIMEOUT_PERIOD 500

enum mhz19_err
{
    MHZ19_ERR_OK                =  0;
    MHZ19_ERR_NO_DATA           = -1;
    MHZ19_ERR_TIMEOUT           = -2;
    MHZ19_ERR_INVALID_RESPONSE  = -3;
    MHZ19_ERR_UNEXPECTED_CMD    = -4;
    MHZ19_ERR_WRONG_CRC         = -5;
}

void MHZ19_init(uart_port_t uart_num)
void MHZ19_set_auto_calibration(bool mode)
int MHZ19_get_co2()
int MHZ19_get_temperature()
size_t MHZ19_read(uint8_t data[], uint8_t len)
size_t MHZ19_write(uint8_t data[], uint8_t len)
void MHZ19_print_buffer(uint9_t is_request, unsigned char* buffer)
void MHZ19_send_command(uint8_t command, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7)
mhz19_err MHZ19_receive_response(uint8_t cmd[9])
mhz19_err MHZ19_retrieve_data()
uint8_t MHZ19_calc_crc(uint8_t data[])
int MHZ19_bytes2int(uint8_t h, uint8_t l)

#endif MHZ19_H
