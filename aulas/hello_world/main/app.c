#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

// PLEASE, CHECK THE FOLLOWING LINK:
// https://docs.espressif.com/projects/esp-idf/en/v5.2.3/esp32/api-reference/system/freertos_idf.html
// https://freertos.org/Documentation/00-Overview

#include "app.h"

#define STACK_SIZE 4096

enum app_events_e
{
    APP_TICK_TIMER_EVENT = 1 << 0,
    APP_RECONFIGURE_EVENT = 1 << 1,
    APP_ALL_EVENTS = APP_TICK_TIMER_EVENT | APP_RECONFIGURE_EVENT,
};

static uint32_t led_tick_timer_ms = 2000;
static TimerHandle_t led_timer_handler = NULL;
static EventGroupHandle_t evt_group;
static TaskHandle_t led_task_handler = NULL;
//static TaskHandle_t led_task_handler2 = NULL;

void app_led_task(void *arg)
{
    uint32_t id = *(uint32_t *)arg;
    //printf("Task parameter: %lu\n", id);

    while(true) 
    {
        // aguarda indefinidamente por qualquer um dos eventos 
        EventBits_t ev_bits = xEventGroupWaitBits(evt_group,APP_ALL_EVENTS,pdTRUE,pdFALSE,portMAX_DELAY);

        if(ev_bits & APP_TICK_TIMER_EVENT)
        {
            printf("[%ld] LED tick event\n",id);
        }

        if(ev_bits & APP_RECONFIGURE_EVENT)
        {
           printf("[%ld] LED reconfigure event\n",id);
        }
    }
}

void app_timer_callback(TimerHandle_t xTimer)
{
    static uint32_t cnt = 0;
    uint32_t id = ( uint32_t ) pvTimerGetTimerID( xTimer );
    
    printf("[%lu] Timer tick callback %ld\n",id,++cnt);

    // set tick timer event
    xEventGroupSetBits(evt_group,(EventBits_t)APP_TICK_TIMER_EVENT);

    // set reconfigure event
    if(cnt % 10 == 0)
    {
        led_tick_timer_ms = (led_tick_timer_ms == 2000) ? 500 : 2000;
        xTimerChangePeriod( xTimer, led_tick_timer_ms / portTICK_PERIOD_MS,0);
        xEventGroupSetBits(evt_group,(EventBits_t)APP_RECONFIGURE_EVENT);
    }
}

void app_init(void)
{
    static uint32_t thread_id = 10;
    //static uint32_t thread_id2 = 15;
    static uint32_t timer_id = 20;

    evt_group = xEventGroupCreate();
    configASSERT(evt_group);

    xTaskCreate( app_led_task, "APP_LED", STACK_SIZE, &thread_id, tskIDLE_PRIORITY, &led_task_handler);
    configASSERT(led_task_handler);

    //xTaskCreate( app_led_task, "APP_LED2", STACK_SIZE, &thread_id2, tskIDLE_PRIORITY, &led_task_handler2);
    //configASSERT(led_task_handler2);

    led_timer_handler = xTimerCreate("LED_TIMER",pdMS_TO_TICKS(led_tick_timer_ms), pdTRUE, (void *)timer_id, &app_timer_callback);
    configASSERT(led_timer_handler);
 
    xTimerStart(led_timer_handler,0);
}