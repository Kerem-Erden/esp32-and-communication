#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint32_t id;           // 11 bit veya 29 bit ID → ikisi de 32 bite sığar
    uint8_t  dlc;          // 0..8 → 1 byte yeter
    uint8_t  data_length;
    bool     is_extended;
    bool     is_remote;
    uint8_t  data[8];      // CAN verisi byte byte gelir
    int64_t  timestamp_us; // mikrosaniye cinsinden zaman
} can_bus_frame_t;

int main()
{
    uint8_t sayac = 255;
    sayac++;
     
    printf("sayac: %d\n", sayac);

    return 0;


}