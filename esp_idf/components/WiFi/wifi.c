#include "wifi.h"
static const char *TAG = "WIFI_EXAMPLE";
static int s_retry_num = 0;

void wifi_event_handler(void *arg, esp_event_base_t event_base,
                        int32_t event_id, void *event_data)
{
    // 3. Wi-Fi Start Phase
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        // 4. Wi-Fi Connect Phase
        esp_wifi_connect();
    }

    // 6. Wi-Fi Disconnect Phase
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
            ssd1306_clear_screen(&dev,false);
            ssd1306_display_text(&dev, 1, "WiFi Connecting", 15, false);
        }
        // else
        // {
        //     xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        // }
        ssd1306_clear_screen(&dev,false);
        ssd1306_display_text(&dev, 1, "Lost Connection", 15, false);
        ESP_LOGI(TAG, "connect to the AP fail");
    }

    // 5. Wi-Fi 'Got IP' Phase
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        // xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        ssd1306_clear_screen(&dev,false);
        ssd1306_display_text(&dev, 1, "WiFi Connected", 20, false);
        mqtt_app_start();
    }
}

void wifi_init_sta(void)
{

    // 1. Wi-Fi/LwIP Init Phase
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 2. Wi-Fi Configuration Phase
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS,
            /* Authmode threshold resets to WPA2 as default if password matches WPA2 standards (pasword len => 8).
             * If you want to connect the device to deprecated WEP/WPA networks, Please set the threshold value
             * to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and set the password with length and format matching to
             * WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.
             */
            // .threshold.authmode = ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD,
            // .sae_pwe_h2e = ESP_WIFI_SAE_MODE,
            // .sae_h2e_identifier = EXAMPLE_H2E_IDENTIFIER,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    // 3. Wi-Fi Start Phase
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "wifi_init_sta finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    // EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
    //                                        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
    //                                        pdFALSE,
    //                                        pdFALSE,
    //                                        portMAX_DELAY);

    // /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
    //  * happened. */
    // if (bits & WIFI_CONNECTED_BIT)
    // {
    //     ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
    //              EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    // }
    // else if (bits & WIFI_FAIL_BIT)
    // {
    //     ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
    //              EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    // }
    // else
    // {
    //     ESP_LOGE(TAG, "UNEXPECTED EVENT");
    // }
}