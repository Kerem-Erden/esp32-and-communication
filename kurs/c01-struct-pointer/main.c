#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

typedef struct 
{
    uint64_t timestamp_us; // buyuk deger buyzuden &&\ zaman > 0 2 ussu 64 llu esp32*
    float distance_m; // kesirli oldugu icin 
    uint8_t node_id; //0-255 unsigned oldugu icin 2 ussu 8 yeter
    int8_t rssi; // - oldugu icin unsigned degil
    
    
}rf_sample_t;

void set_rssi(rf_sample_t *s, int new_rssi)
{
   
    if (new_rssi > 0 || new_rssi < -120)
        {
            printf("%d is not higher than -120 and lower than 0\n\n", new_rssi);
        }
        else
            s->rssi = (int8_t)new_rssi;
    
}

void print_sample(const rf_sample_t *s)    
{
    printf("Node id: %d\nRssi: %d\nTime: %" PRIu64 "us\nDistance: %f\n\n\n\n\n", s->node_id, s->rssi, s->timestamp_us, s->distance_m  );        
}

int main()
{
    rf_sample_t sample = {
        .node_id = 244,
        .rssi = -67,
        .distance_m = 9.9f,
        .timestamp_us = 33333333
    };

    

    print_sample(&sample);
    set_rssi(&sample, -50);
    print_sample(&sample);
    set_rssi(&sample, 100);
    set_rssi(&sample, 200);
    print_sample(&sample);        
    
    printf("%u\n", (unsigned)sizeof(rf_sample_t));
}