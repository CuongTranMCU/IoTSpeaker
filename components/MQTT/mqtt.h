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
#include "nvs_flash.h"
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

#define EXAMPLE_ESP_MQQT_BORKER_URI "mqtt://mqtt.flespi.io"
#define EXAMPLE_ESP_MQQT_BORKER_PORT 1883
#define EXAMPLE_ESP_MQQT_BORKER_TRANSPORT MQTT_TRANSPORT_OVER_TCP
#define EXAMPLE_ESP_MQQT_CREDENTIALS_USERNAME "NLbaNpU1SUgaBhQSS9duq2LMgV4JaCCdRIDNBRz6b3eElk0iGfBUULPU9E52R1AC"

typedef void (*mqtt_data_pt_t)(char *data, uint16_t length);
void mqtt_data_pt_set_callback(void *cb);
void mqtt_data_publish_callbak(const char *data, size_t len);

void mqtt_data_callback(char *data, uint16_t length);
void mqtt_app_start(void);

#endif