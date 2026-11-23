#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

typedef struct
{
    uint64_t timestamp_us;
    float distance_m;
    uint8_t node_id;
    int8_t rssi;
}rf_sample_t;

typedef void (*cmd_fn_t)(rf_sample_t *s);

typedef struct 
{
    const char *name;
    cmd_fn_t func;    
}table_t;

void print_sample(rf_sample_t *s)
{
     printf("Node id: %d\nRssi: %d\nTime: %" PRIu64 "us\nDistance: %f\n\n\n\n\n", s->node_id, s->rssi, s->timestamp_us, s->distance_m  );
}

void set_rssi_near(rf_sample_t *s)
{
    s->rssi = -40;
}

void set_rssi_far(rf_sample_t *s)
{
    s->rssi = -100;
}

table_t commands[] = {
    {"print", print_sample},
    {"near", set_rssi_near},
    {"far", set_rssi_far}
};

void dispatch(const char *name, rf_sample_t *s)
{
    size_t size = sizeof(commands) / sizeof(commands[0]);
    
    for (size_t i = 0; i < size; i++)
    {
        if (strcmp(commands[i].name, name) == 0)
        {
            commands[i].func(s);
            return;
        }
    }
    printf("bilinmeyen komut: %s\n", name);
}

int main()
{
    rf_sample_t sample = {
        .node_id = 244,
        .rssi = -67,
        .distance_m = 9.9f,
        .timestamp_us = 33333333
    };
    
    dispatch("print", &sample);
    dispatch("near", &sample);
    dispatch("far", &sample);
    dispatch("plane", &sample);
    dispatch("print", &sample);

    return 0;
}