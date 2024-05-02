#include <stdio.h>
#include "DFPlayer.h"
#include "driver/gpio.h"
static const int RX_BUF_SIZE = 1024;

#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)
#define UART UART_NUM_2
DFPLAYER_Name MP3;
void init_uart(DFPLAYER_Name *MP3) 
{
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    // We won't use a buffer for sending data.
    ESP_ERROR_CHECK( uart_driver_install(MP3->DFP_UART, RX_BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(MP3->DFP_UART, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(MP3->DFP_UART, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}
static void rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(10);
    while (1) {
        const int rxBytes = uart_read_bytes(UART, data, 10, 500 / portTICK_PERIOD_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%02X'", rxBytes, data[0]);
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%02X'", rxBytes, data[1]);
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%02X'", rxBytes, data[2]);
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%02X'", rxBytes, data[3]);
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%02X'", rxBytes, data[4]);
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%02X'", rxBytes, data[5]);
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%02X'", rxBytes, data[6]);
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%02X'", rxBytes, data[7]);
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%02X'", rxBytes, data[8]);
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%02X'", rxBytes, data[9]);
        }
}
    free(data);

}

void app_main(void)
{
    DFPLAYER_Init(&MP3,UART_NUM_2);
    init_uart(&MP3);
    // DFPLAYER_SetVolume(&MP3,10);
    // vTaskDelay(100/portTICK_PERIOD_MS);
    DFPLAYER_Play(&MP3);
    // DFPLAYER_PlayFileInFolder(&MP3,1,1);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    xTaskCreate(rx_task, "rx_task", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    

}
