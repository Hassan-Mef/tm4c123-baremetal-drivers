# TM4C123 Bare-Metal Drivers

A bare-metal embedded systems project for the Texas Instruments TM4C123GXL LaunchPad.

The objective of this repository is to implement common peripheral drivers from scratch without
using vendor libraries such as TivaWare or any Hardware Abstraction Layer (HAL).

This project is being developed incrementally, beginning with the startup code and expanding
towards a complete driver library and simple applications.

---

## Features

- ARM Cortex-M4 startup code
- Custom linker script
- Register-level programming
- GPIO Driver
- Timer Driver *(Coming Soon)*
- UART Driver *(Coming Soon)*
- LIN Driver *(Coming Soon)*
- LIN Application *(Coming Soon)*

---

## Current Progress

- [x] Startup
- [x] Linker Script
- [x] Makefile
- [x] GPIO Driver
- [ ] Timer Driver
- [ ] UART Driver
- [ ] LIN Driver
- [ ] LIN Application

---

## Project Structure

```
.
├── build/
├── docs/
├── inc/
├── obj/
├── platform/
│   ├── linker.ld
│   └── startup.s
├── src/
│   ├── gpio.c
│   └── main.c
└── Makefile
```

---

## Build

```bash
make
```

Clean

```bash
make clean
```

---

## Hardware

- Texas Instruments TM4C123GXL LaunchPad
- ARM Cortex-M4F

---

## Toolchain

- arm-none-eabi-gcc
- GNU Make
- OpenOCD
- LM Flash Programmer

---

## Future Work

- Timer Driver
- UART Driver
- LIN Driver
- Interrupt handling
- Example applications

---

## License

MIT License
