# Changelog

## v2.0.0 — 2026-10-04

Complete rewrite.  
**All public macros are renamed** — this release is a hard break with v1.  
Portfolio project,  
no backward-compatibility shims.  
See the migration plan §3.5 for the full rename table.

### Renames

| v1                           | v2                               |
|------------------------------|----------------------------------|
| `PinMode(LED, Output)`       | `IO_MODE(LED, IO_OUTPUT)`        |
| `DigitalWrite(LED, High)`    | `IO_WRITE(LED, IO_HIGH)`         |
| `DigitalRead(BUTTON)`        | `IO_READ(BUTTON)`                |
| `DigitalLevelToggle(LED)`    | `IO_TOGGLE(LED)`                 |
| `PinModeToggle(LED)`         | `IO_MODE_TOGGLE(LED)`            |
| `BitSet(reg, bit)`           | `IO_BIT_SET(reg, bit)`           |
| `BitClear(reg, bit)`         | `IO_BIT_CLEAR(reg, bit)`         |
| `BitToggle(reg, bit)`        | `IO_BIT_TOGGLE(reg, bit)`        |
| `BitCheck(reg, bit)`         | `IO_BIT_IS_SET(reg, bit)`        |
| `Input` / `Output`           | `IO_INPUT` / `IO_OUTPUT`         |
| `Low` / `High`               | `IO_LOW` / `IO_HIGH`             |
| `True` / `False`             | removed — use `<stdbool.h>`      |

### Fixes

- Parenthesize every macro argument.  
  `BitCheck(a | b, 3)` used to expand to `a | b & mask` (wrong precedence);  
  the new `IO_BIT_IS_SET` evaluates `(a | b)` correctly.
- Drop reserved identifiers (`_SET`, `_CLEAR`, `_TOGGLE`, `_GET`, `_PORT`, `_DDR`, `_PIN`).  
  Identifiers that start with an underscore and an uppercase letter are reserved by the C standard.
- Change `1UL << y` to `(uint8_t)(1u << (y))`.  
  Constant bit indices still generate `sbi`/`cbi`;  
  variable indices no longer force a 32-bit shift sequence.

### Project changes

- Repo renamed `AVR-IO-Macros` → `avr-io-macros`.
- Restructured to `src/ examples/ tests/ sim/ docs/ scripts/` (see migration plan §4).
- Added `Build.ps1`,  
  `Simulate.ps1`,  
  `scripts/Common.psm1` for Windows build/test automation with auto-installing toolchain.
- Added host-side Unity-style unit tests under `tests/` with fake AVR registers.
- Added GitHub Actions CI running the matrix build + tests on windows-latest.
- README documents public API only, flat section per function, no API tables.

## v1 — initial release

Original Arduino-style macros (`PinMode`, `DigitalWrite`, `BitSet`, …).
