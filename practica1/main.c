#include <stdint.h>

#define RESETS_BASE 0x4000C000
#define RESETS_RESET (*(volatile uint32_t *)(RESETS_BASE + 0x0))
#define RESETS_DONE (*(volatile uint32_t *)(RESETS_BASE + 0x8))

#define IO_BANK0_BASE 0x40014000
#define GPIO25_CTRL (*(volatile uint32_t *)(IO_BANK0_BASE + 0x0CC))

#define SIO_BASE 0xD0000000
#define SIO_GPIO_OE (*(volatile uint32_t *)(SIO_BASE + 0x020))
#define SIO_GPIO_OUT (*(volatile uint32_t *)(SIO_BASE + 0x010))

#define LED_PIN 25

int main() {
    RESETS_RESET &= ~((1 << 5) | (1 << 8));

    while ((RESETS_DONE & ((1 << 5) | (1 << 8))) != ((1 << 5) | (1 << 8)));

    GPIO25_CTRL = 5;

    SIO_GPIO_OE |= (1 << LED_PIN);

    while (1) {
        SIO_GPIO_OUT ^= (1 << LED_PIN);

        for (volatile int x = 0; x < 10; x++) {
            for (volatile int i = 0; i < 250000; i++);
        }
    }

    return 0;
}

