#ifndef RF_SAMPLE_H
#define RF_SAMPLE_H

#include <stdint.h>

typedef struct
{
    uint64_t timestamp_us;
    float distance_m;
    uint8_t node_id;
    int8_t rssi;
}rf_sample_t;

void print_sample(const rf_sample_t *s);

int set_rssi(rf_sample_t *s, int new_rssi);

#endif
