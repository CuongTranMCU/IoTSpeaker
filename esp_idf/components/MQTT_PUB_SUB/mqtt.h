#ifndef _MQTT_H
#define _MQTT_H

/* MQTT (over TCP) Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include <cJSON.h>

#define EXAMPLE_ESP_MQQT_BORKER_URI "mqtt://mqtt.flespi.io"
#define EXAMPLE_ESP_MQQT_BORKER_PORT 1883
#define EXAMPLE_ESP_MQQT_BORKER_TRANSPORT MQTT_TRANSPORT_OVER_TCP
#define EXAMPLE_ESP_MQQT_CREDENTIALS_USERNAME "oiGjHdBbBIvM0gOgrc0oLFTFt5ev1frmO6r8SOQURW1Gr7qYjFflB5IdeKutDcUk"
esp_mqtt_client_handle_t get_mqtt_client_handle(void);
extern uint32_t MQTT_CONNECTED;
typedef void (*mqtt_data_pt_t)(char *data, uint16_t length);
void mqtt_data_publish_callback(const char *data);
void mqtt_data_pt_set_callback(mqtt_data_pt_t mqtt_func_ptr);
char *convert_model_sensor_to_json(int temperature, int humidity);

void mqtt_app_start(void);

#endif