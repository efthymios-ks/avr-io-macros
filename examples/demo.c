#include "io_macros.h"

// Button on PD2, LED on PB5 (ATmega328P DIP-28). See README for wiring.
#define BUTTON D, 2
#define LED B, 5

int main(void)
{
    IO_MODE(BUTTON, IO_INPUT);
    IO_WRITE(BUTTON, IO_HIGH); // Enable internal pull-up.
    IO_MODE(LED, IO_OUTPUT);
    IO_WRITE(LED, IO_LOW);

    while (1) {
        // Button is active-low (pressed = GND).
        bool is_pressed = !IO_READ(BUTTON);
        IO_WRITE(LED, is_pressed ? IO_HIGH : IO_LOW);
    }
}
