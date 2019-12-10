#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "esp_log.h"
#include "driver/uart.h"

#include "mhz19.h"

static const char* TAG = "MHZ19";

// TODO: use a struct
unsigned char _mhz19_response[9];
mhz19_err _mhz19_result = MHZ19_ERR_NO_DATA;
uint8_t _mhz19_cmd = 0;
uart_port_t _uart_num;

static uint32_t millis()
{
    return (uint32_t) (clock() * 1000 / CLOCKS_PER_SEC);
}

void MHZ19_init(uart_port_t uart_num)
{
    _uart_num = uart_num;
}

int MHZ19_bytes2int(uint8_t h, uint8_t l)
{
    return (256 * ((int)h)) + ((int)l);
}

void MHZ19_print_buffer(uint9_t is_request, unsigned char* buffer)
{
    char message[99];
    memset(message, 0, sizeof(message));

    sprintf(message, '%s ', is_request ? "Request >>", "Response <<");
    for(uint8_t i = 0; i < 9; i++)
    {
        sprintf((message + strlen(message)), '0x%02x ', buffer[i])
    }

    ESP_LOGI(TAG, message);
}

uint8_t MHZ19_calc_crc(uint8_t data[])
{
    uint8_t i;
    uint8_t crc = 0;
    for (i = 1; i < 8; i++)
    {
        crc += data[i];
    }
    crc = 255 - crc;
    crc++;

    return crc;
}

size_t MHZ19_write(uint8_t data[], uint8_t len)
{
    size_t length;
	char buf[20];

    // Discard all the data contained in the ring buffer or pending to be read
	do
	{
		ESP_ERROR_CHECK(uart_get_buffered_data_len(_uart_num, (size_t*)&length));
		if (length > 0)
		{
			uart_read_bytes(_uart_num, buf, length > sizeof(buf) ? sizeof(buf) : length, 100);
		}
	}
	while(length <= 0);
    
	length = uart_write_bytes(_uart_num, (const char*)toSend, 9);

 	ESP_ERROR_CHECK(uart_flush(_uart_num));

    return length;
}

void MHZ19_send_command(uint8_t command, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7)
{
    _mhz19_cmd = command;

    uint8_t cmd[9] = { 0xFF, 0x01, command, b3, b4, b5, b6, b7, 0x00 };
    cmd[8] = MHZ19_calc_crc(cmd);

    MHZ19_print_buffer(1, cmd);
    MHZ19_write(cmd, 9);  
}

size_t MHZ19_read(uint8_t data[], uint8_t len)
{
    size_t length;
	unsigned long start_millis = millis();

	memset(data, 0, len);

	do
	{
		ESP_ERROR_CHECK(uart_get_buffered_data_len(_uart_num, (size_t*)&length));

		if (millis() - timeStamp >= TIMEOUT_PERIOD) 
		{
			ESP_LOGW(TAG, "Timed out waiting for response");    
			return MHZ19_ERR_TIMEOUT;
		}
	}
	while(length <= 0);
    
	length = uart_read_bytes(_uart_num, data, len, 100);
    MHZ19_print_buffer(0, cmd);

    return length;
}

mhz19_err MHZ19_receive_response(uint8_t cmd[9])
{
    size_t res;
    res = MHZ19_read(cmd, 9);

    if (res < MHZ19_ERR_OK)
    {
        _mhz19_result = res;
    }
    else if (cmd[0] != 0xFF)
    {
        _mhz19_result = MHZ19_ERR_INVALID_RESPONSE;
    }
    else if (cmd[1] != _mhz19_cmd)
    {
        _mhz19_result = MHZ19_ERR_UNEXPECTED_CMD;
    }
    else if (cmd[8] != MHZ19_calc_crc(cmd))
    {
        _mhz19_result = MHZ19_ERR_WRONG_CRC;
    }
    else
    {
        _mhz19_result = MHZ19_ERR_OK;
    }
    
    return _mhz19_result;
}

mhz19_err MHZ19_retrieve_data()
{
    MHZ19_send_command(0x86, 0, 0, 0, 0, 0);
    return MHZ19_receive_response(_mhz19_response);
}

void MHZ19_set_auto_calibration(bool mode)
{
    uint8_t value = mode ? 0xA0 : 0x00;
    MHZ19_send_command(0x79, value, 0x00, 0x00, 0x00, 0x00);
}

int MHZ19_get_co2()
{
    if (_mhz19_result == MHZ19_ERR_OK)
    {
        // TODO: Cast the data to uint16_t instead of using an ad hoc function that doest the same
        return MHZ19_bytes2int(_mhz19_response[2], _mhz19_response[3]);
    }

    return _mhz19_result;
}

int MHZ19_get_temperature()
{
    if (_mhz19_result == MHZ19_ERR_OK)
    {
        int value = (int)_mhz19_response[4];
        return value - 40;
    }

    return _mhz19_result;
}
