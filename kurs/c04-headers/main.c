#include "rf_sample.h"
#include <stdio.h>

int main()
{
    rf_sample_t sample = {
        .node_id = 11,
        .distance_m = 90,
        .rssi = -60,
        .timestamp_us = 199999

    };

    int temp;
    print_sample(&sample);
    set_rssi(&sample, -77);
    temp = set_rssi(&sample, 50);
    print_sample(&sample);
    printf("temp: %d", temp);

    return 0;
}