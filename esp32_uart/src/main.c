#include "driver/gpio.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "driver/uart.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_http_client.h"

// WIFI CONSTANTS
#define WIFI_SSID "Your Own Wifi"    
#define WIFI_PASS "Your Own Passwd" 

// Thingspeak API key
#define WRITE_API_KEY "Api Key" 

// UART PINS
#define TX_PIN GPIO_NUM_17   
#define RX_PIN GPIO_NUM_16
#define BAUD_RATE 115200   // This value has to be the same in STM32F4XX  
#define BUF_SIZE 1024       

static const char *TAG = "ThingSpeak_UART";
EventGroupHandle_t wifi_event_group;
const int WIFI_CONNECTED_BIT = BIT0;

float temperature = 0.0, voltage = 0.0, current = 0.0;

// Wifi Handler events
static void event_handler(esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGW(TAG, "Wi-Fi disconnected, trying to reconnect...");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "Wi-Fi cconnected successfully!");
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

// WIFI CONECTION FUNCTION
void connect_to_wifi() {
    wifi_event_group = xEventGroupCreate();

    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "Connecting to Wi-Fi...");
    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    ESP_LOGI(TAG, "Connection successful!");
}

// SEND DATA TO THINGSPEAKER FUNCTION
void send_to_thingspeak(float temp, float volt, float curr) {
    char url[256];
    snprintf(url, sizeof(url), 
             "http://api.thingspeak.com/update?api_key=%s&field1=%.2f&field2=%.2f&field3=%.2f",
             WRITE_API_KEY, temp, volt, curr);

    esp_http_client_config_t config = {
        .url = url,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Data sent successfully to ThingSpeak");
    } else {
        ESP_LOGE(TAG, "Error sending the data!\nError: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}

// Split the data received from the UART 2
bool split_data(const char *data, float *values) {
    int index = 0;
    char *token = strtok((char *)data, " ");
    while (token != NULL && index < 3) {
        values[index++] = atof(token);
        token = strtok(NULL, " ");
    }
    return index == 3;
}

// UART TASK
void uart_task() {
    uint8_t *data = (uint8_t *)malloc(BUF_SIZE);

    while (1) {
        int len = uart_read_bytes(UART_NUM_2, data, BUF_SIZE, 100 / portTICK_PERIOD_MS);
        if (len > 0) {
            data[len] = '\0';

            float values[3];
            if (split_data((char *)data, values)) {
                temperature = values[0];
                voltage = values[1];
                current = values[2];

                // Show the data on the terminal
                ESP_LOGI(TAG, "Temperature: %.2f °C", temperature);
                ESP_LOGI(TAG, "Voltage: %.2f V", voltage);
                ESP_LOGI(TAG, "Current: %.2f A", current);

                // Send data to ThingSpeak
                send_to_thingspeak(temperature, voltage, current);
            } else {
                ESP_LOGW(TAG, "Error: Datos incompletos o mal formateados.");
            }
        }
        // Wait 5 sec
        vTaskDelay(5000 / portTICK_PERIOD_MS); 
    }

    free(data);
}

// MAIN FUNCTION

void app_main(void) {
    // Init NVS (Non-Volatile Storage): allows data to be saved even if the ESP32 is turned off or restarted.
    esp_err_t ret = nvs_flash_init();


    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Connect  Wi-Fi
    connect_to_wifi();

    // Config UART
    uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_param_config(UART_NUM_2, &uart_config);
    uart_set_pin(UART_NUM_2, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, 0);

    // Call the uart function
    uart_task();
}
