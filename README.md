# avr-io-macros

Arduino-style I/O manipulation macros for AVR microcontrollers.

[![ci](https://github.com/efthymios-ks/avr-io-macros/actions/workflows/ci.yml/badge.svg)](https://github.com/efthymios-ks/avr-io-macros/actions/workflows/ci.yml)

## Features

- Pin-oriented macros that take `PORT_LETTER, BIT_NUMBER` as a single named token.
- Register-oriented bit macros that work on any `uint8_t` register.
- Header-only,  
  no globals,  
  no state,  
  no runtime overhead.
- Resolves to single `sbi`/`cbi` instructions when the pin and bit are compile-time constants.
- Builds clean at `-Wall -Wextra -Werror` on both `-Os` and `-O0`.
- Host-side unit tests with fake AVR registers.

## Supported MCUs and toolchain

Verified on ATmega328P (default demo, DIP-28) and ATmega32.  
Any AVR with the standard `PORTx`/`DDRx`/`PINx` register layout should work.  
Toolchain is avr-gcc with `-std=gnu99`.

## Wiring (default demo, ATmega328P DIP-28)

The library itself has no fixed pins — the demo circuit uses a button and an LED.

| Signal | Device pin           | AVR pin | DIP-28 | Config                       | Description |
|--------|----------------------|---------|--------|------------------------------|-------------|
| BUTTON | Push button, leg 1   | PD2     | 4      | `#define BUTTON D, 2` (demo) | Input with internal pull-up; pressed = low |
| GND    | Push button, leg 2   | GND     | 8      | —                            | Button connects the input to ground |
| LED    | LED anode (via 330 Ω)| PB5     | 19     | `#define LED B, 5` (demo)    | Output; high = LED on |
| GND    | LED cathode          | GND     | 22     | —                            | LED return |

## Quick start

```c
#include "io_macros.h"

#define LED    B, 5
#define BUTTON D, 2

int main(void)
{
    IO_MODE(LED, IO_OUTPUT);
    IO_MODE(BUTTON, IO_INPUT);
    IO_WRITE(BUTTON, IO_HIGH); // Enable the internal pull-up.

    while (1) {
        IO_WRITE(LED, IO_READ(BUTTON) ? IO_LOW : IO_HIGH);
    }
}
```

A pin is written as two tokens: the port letter and the bit number.  
The macros concatenate `PORT`/`DDR`/`PIN` with the letter at compile time,  
which is why pin arguments do not take parentheses.

## API

### IO_MODE
- `IO_MODE(pin, mode)`
- Does: configures the pin as input or output by setting its DDR bit.
- Params: `mode` is `IO_INPUT` or `IO_OUTPUT`.

### IO_WRITE
- `IO_WRITE(pin, level)`
- Does: drives the pin high or low by writing its PORT bit.
- Params: `level` is `IO_LOW` or `IO_HIGH`.
- Notes: when the pin is configured as input, this toggles the internal pull-up.

### IO_READ
- `IO_READ(pin)`
- Does: reads the current state of the pin's PIN register bit.
- Returns: `true` when the pin reads high, `false` otherwise.

### IO_TOGGLE
- `IO_TOGGLE(pin)`
- Does: flips the pin's PORT bit.

### IO_MODE_TOGGLE
- `IO_MODE_TOGGLE(pin)`
- Does: flips the pin's DDR bit, swapping input/output.

### IO_BIT_SET
- `IO_BIT_SET(reg, bit)`
- Does: sets a bit in an 8-bit register (`reg |= (1 << bit)`).

### IO_BIT_CLEAR
- `IO_BIT_CLEAR(reg, bit)`
- Does: clears a bit in an 8-bit register (`reg &= ~(1 << bit)`).

### IO_BIT_TOGGLE
- `IO_BIT_TOGGLE(reg, bit)`
- Does: toggles a bit in an 8-bit register (`reg ^= (1 << bit)`).

### IO_BIT_IS_SET
- `IO_BIT_IS_SET(reg, bit)`
- Does: tests whether a bit is set.
- Returns: `true` when the bit is set, `false` otherwise.
- Notes: safe with expression arguments — the register expression is parenthesised before the mask.

## Literals

- `IO_INPUT` = 0
- `IO_OUTPUT` = 1
- `IO_LOW` = 0
- `IO_HIGH` = 1

## Memory usage

Flash and RAM sizes for the demo are produced by `Build.ps1` and written to `build/size.txt`.  
Before/after comparison on ATmega32 (v1's historical target),  

| Build | Flash / RAM |
|-------|------------:|
| v1 (ATmega32, -Os) | 128 B / 0 B |
| v2 (ATmega32, -Os) | 128 B / 0 B |

## Build, test, simulate

```powershell
.\Build.ps1
.\Build.ps1 -AllMcus -DebugBuild
.\Build.ps1 -Test
.\Build.ps1 -Clean
.\Simulate.ps1          # launches SimulIDE
.\Simulate.ps1 -NoLaunch # smoke-test without GUI
```

`Build.ps1` and `Simulate.ps1` install the AVR toolchain,  
host gcc for `-Test`,  
and SimulIDE on first run,  
into a shared per-user cache folder — no admin rights,  
no system-wide `PATH` changes.  
Add `-RemoveTools` to uninstall what the script installed when the run ends.  
Pass `-NoInstall` to fail loudly instead of installing.

## Limitations

- Pin argument syntax (`B, 5`) relies on token pasting.  
  It will not work inside another macro that expects a single value — assign it to a `#define` and pass that.
- Variable bit indices (`IO_BIT_SET(reg, index)` where `index` is a runtime variable) compile to a shift loop rather than a single `sbi`.  
  Constant bit indices stay single-instruction.

## Changelog and license

See [CHANGELOG.md](CHANGELOG.md).  
MIT — see [LICENSE](LICENSE).
