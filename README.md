# STM32_plane_game

This is a simple 2D plane game for undergraduate digital design coursework developed on an STM32F4 microcontroller using the STM32 HAL library. The project is written in C and uses direct hardware interaction via GPIO, DMA, TIM, USART, and ADC peripherals.

## 🚀 Features

- Player-controlled plane movement
- LED or display-based game output
- Collision detection logic
- Timer-based game refresh/update loop
- Uses multiple peripherals: GPIO, ADC, USART, DMA, I2C, etc.

## 🧰 Development Environment

- **MCU**: STM32F407 (or similar STM32F4xx series)
- **Toolchain**: STM32CubeIDE 
- **Library**: STM32Cube HAL Drivers

## 📁 File Structure

| File / Folder       | Description                                   |
|---------------------|-----------------------------------------------|
| `main.c`            | Main game logic, setup, and main loop         |
| `led_driver.c/h`    | LED control logic (possibly for scoring or effects) |
| `gpio.c/h`          | GPIO initialization and I/O                   |
| `adc.c/h`           | ADC input handling (maybe for joystick?)      |
| `dma.c/h`           | DMA configuration                             |
| `tim.c/h`           | Timer setup and interrupts                    |
| `usart.c/h`         | Serial communication                          |
| `stm32f4xx_it.c/h`  | IRQ handlers                                  |
| `syscalls.c`, `sysmem.c` | Runtime low-level support files           |
| `system_stm32f4xx.c`| System initialization                         |

## 🛠️ How to Build

1. Open project in **STM32CubeIDE** or import the sources into your IDE.
2. Connect your STM32 board via ST-Link.
3. Compile and flash to the board.
4. Use serial monitor or external display (e.g. OLED) for game interface.


## 📄 License

MIT License

---

## ✨ Acknowledgements

Developed as part of a microcontroller programming practice/project based on STM32.

