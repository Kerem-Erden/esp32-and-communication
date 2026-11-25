#include <stdio.h>
#include <stdint.h>

void print_binary(uint8_t value)
{
    for (int i = 7; i >= 0; i--)
    {     
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

uint8_t set_bit(uint8_t value, int n)
{
    return value | (1 << n);
}

uint8_t clear_bit(uint8_t value, int n)
{
    return value & ~(1 << n);
}

uint8_t toggle_bit(uint8_t value, int n)
{
    return value ^ (1 << n);
}

int test_bit(uint8_t value, int n)
{
    return (value >> n) & 1;
}

int main()
{
    print_binary(clear_bit(36, 2));    // 00100000
    print_binary(toggle_bit(36, 0));   // 00100101
    print_binary(toggle_bit(36, 2));   // 00100000
    printf("%d\n", test_bit(36, 5));   // 1
    printf("%d\n", test_bit(36, 4));   // 0

    uint8_t mask = (1 << 2) | (1 << 5);
    print_binary(mask);
    

    return 0;
}