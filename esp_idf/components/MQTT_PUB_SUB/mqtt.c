#include "mqtt.h"

static const char *TAG = "MQTT";
static esp_mqtt_client_handle_t global_client;
static mqtt_data_pt_t mqtt_data_pt = NULL;

static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0)
    {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
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
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        MQTT_CONNECTED = 1;

        msg_id = esp_mqtt_client_subscribe(client, "esp_sub", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        MQTT_CONNECTED = 0;
        wifi_init_sta();
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
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        event->data[event->data_len] = '\0';
        mqtt_data_pt(event->data, event->data_len);// thực hiện hàm get_data_call_back
                                                    // mqtt_data_callback_: gán địa chỉ = get_data_call_back
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
        {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno", event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

esp_mqtt_client_handle_t get_mqtt_client_handle(void)
{
    return global_client;
}

char *convert_model_sensor_to_json(int temperature, int success)
{
    // create a new cJSON object
    cJSON *json = cJSON_CreateObject();
    if (json == NULL)
    {
        printf("Error: Failed to create cJSON object\n");
        return NULL;
    }
    // modify the JSON data
    cJSON_AddNumberToObject(json, "sucess", success);

    // convert the cJSON object to a JSON string
    char *json_str = cJSON_Print(json);

    // free the JSON string and cJSON object
    cJSON_Delete(json);

    return json_str;
}

void mqtt_data_publish_update(char * field_name)
{

    esp_mqtt_client_handle_t client = get_mqtt_client_handle();
    if (client != NULL)
    {
        char json_string[20]; // Đảm bảo bộ đệm đủ lớn
        sprintf(json_string, "{\"%s\": true}",field_name); // Sử dụng sprintf để định d
        // Gửi dữ liệu lên broker MQTT với chủ đề là "data"
        int msg_id = esp_mqtt_client_publish(client, "esp_pub", json_string, 0, 0, 0);
        if (msg_id < 0)
        {
            ESP_LOGE(TAG, "Failed to publish data to MQTT broker");
        }
        else
        {
            ESP_LOGI(TAG, "Published data to MQTT broker, msg_id=%d", msg_id);
        }
    }
    else
    {
        ESP_LOGE(TAG, "MQTT client not initialized");
    }
}

void mqtt_app_start()
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = EXAMPLE_ESP_MQQT_BORKER_URI,
        .broker.address.port = EXAMPLE_ESP_MQQT_BORKER_PORT,
        //.broker.address.transport = EXAMPLE_ESP_MQQT_BORKER_TRANSPORT,
        .credentials.username = EXAMPLE_ESP_MQQT_CREDENTIALS_USERNAME,
    };

    global_client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(global_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(global_client);
}
void mqtt_app_start_reconnect()
{
        esp_mqtt_client_start(global_client);
}
// callback:
void mqtt_data_pt_set_callback(mqtt_data_pt_t mqtt_func_ptr)
{
    mqtt_data_pt = mqtt_func_ptr;
}
