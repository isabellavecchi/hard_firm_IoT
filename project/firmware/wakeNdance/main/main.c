#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "softap_sta.h"

void app_main(void)
{
    wifi_setup();

    while (true) {
        printf("Hello from app_main!\n");
        sleep(1);
    }
}
