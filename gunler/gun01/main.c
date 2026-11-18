#include <stdio.h>
#include <stdint.h>

#define CAN_BITRATE 500000

int main(void)
{
    printf("Merhaba Can\n");

    uint32_t bitrate = CAN_BITRATE;

    printf("Bitrate: %d\n", bitrate);

    return 0;
}