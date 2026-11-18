#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.H"
#include "freertos/task.h"
#include "esp_random.h"

#define TAG "DICE"

int dice_role()
{
    int random = esp_random();
    int positiveNum = abs(random);
    int diceNum = (positiveNum % 6) + 1;

    return diceNum;
}


void app_main(void)
{
    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "random number %d", dice_role());
    }
}