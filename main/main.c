#include <stdio.h>
#include "DFPlayer.h"
#include "driver/gpio.h"
// wifi
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

static const char *TAG = "MQTT_EXAMPLE";
#define  EXAMPLE_ESP_WIFI_SSID "A06.15 5G"
#define  EXAMPLE_ESP_WIFI_PASS "hoihuecon"
#define MAX_RETRY 10
static int retry_cnt = 0;

uint32_t MQTT_CONNEECTED = 0;

//
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
    ESP_ERROR_CHECK(uart_driver_install(MP3->DFP_UART, RX_BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(MP3->DFP_UART, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(MP3->DFP_UART, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
}
static void rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t *data = (uint8_t *)malloc(10);
    while (1)
    {
        const int rxBytes = uart_read_bytes(UART, data, 10, 500 / portTICK_PERIOD_MS);
        if (rxBytes > 0)
        {
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
// wifi and mqtt
static void mqtt_app_start(void);

static esp_err_t wifi_event_handler(void *arg, esp_event_base_t event_base,
                                   int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        esp_wifi_connect();
        ESP_LOGI(TAG, "Trying to connect with Wi-Fi\n");
        break;

    case WIFI_EVENT_STA_CONNECTED:
        ESP_LOGI(TAG, "Wi-Fi connected\n");
        break;

    case IP_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "got ip: startibg MQTT Client\n");
        mqtt_app_start();
        break;

    case WIFI_EVENT_STA_DISCONNECTED:
        ESP_LOGI(TAG, "disconnected: Retrying Wi-Fi\n");
        if (retry_cnt++ < MAX_RETRY)
        {
            esp_wifi_connect();
        }
        else
        ESP_LOGI(TAG, "Max Retry Failed: Wi-Fi Connection\n");
        break;

    default:
        break;
    }
    return ESP_OK;
}

void wifi_init(void)
{
    esp_event_loop_create_default();
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS,
	     .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    esp_netif_init();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();
}

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        MQTT_CONNEECTED=1;
        
        msg_id = esp_mqtt_client_subscribe(client, "humidity", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, "temperature", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        MQTT_CONNEECTED=0;
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC = %.*s\r\n", event->topic_len, event->topic);
        printf("DATA = %.*s\r\n", event->data_len, event->data);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

esp_mqtt_client_handle_t client = NULL;
static void mqtt_app_start(void)
{
    ESP_LOGI(TAG, "STARTING MQTT");
    esp_mqtt_client_config_t mqttConfig = {
        .broker.address.uri = "mqtt://mqtt.flespi.io",
        .credentials.username = "oiGjHdBbBIvM0gOgrc0oLFTFt5ev1frmO6r8SOQURW1Gr7qYjFflB5IdeKutDcUk"};
    client = esp_mqtt_client_init(&mqttConfig);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void Publisher_Task(void *params)
{
  while (true)
  {
    if(MQTT_CONNEECTED)
    {
        
        esp_mqtt_client_publish(client, "temperature","40", 0, 0, 0);
        esp_mqtt_client_publish(client, "humidity","80", 0, 0, 0);

    }
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}
void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    // wifi_init();
    DFPLAYER_Init(&MP3,UART_NUM_2);
    init_uart(&MP3);
    DFPLAYER_SetVolume(&MP3,15);
    DFPLAYER_Play(&MP3);
    // DFPLAYER_PlayFileInFolder(&MP3,1,1);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    xTaskCreate(rx_task, "rx_task", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    // xTaskCreate(Publisher_Task, "Publisher_Task", 1024 * 5, NULL, 5, NULL);
  
    while (true)
    {
        DFPLAYER_Next(&MP3);
        vTaskDelay(100000/portTICK_PERIOD_MS);
    }
    
}
