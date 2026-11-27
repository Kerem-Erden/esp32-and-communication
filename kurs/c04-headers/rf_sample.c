#include "rf_sample.h"
#include <stdio.h>
#include <inttypes.h>

void print_sample(const rf_sample_t *s)
{
    printf("Node id: %d\nRssi: %d\nTime: %" PRIu64 "us\nDistance: %f\n\n\n\n\n", s->node_id, s->rssi, s->timestamp_us, s->distance_m  );
}

static int is_valid_rssi(int value)
{
    if (value >= -120 && value <= 0)
    {
        return 1;
    }
    else
        return 0;
}

int set_rssi(rf_sample_t *s, int new_rssi)
{
    if (!is_valid_rssi(new_rssi))
    {
        return -1;
    }
    s->rssi = new_rssi;
    return 0;

}
