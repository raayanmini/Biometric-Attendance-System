# Biometric Attendance System – STM32F401 + GT-521F32

This repository contains a biometric attendance system built on the **Raayan Mini (STM32F401RBT6)** board.  
It uses a **GT-521F32 fingerprint sensor** over UART, a **16×2 character LCD**, a **4×4 matrix keypad**, and a few switches to provide **User** and **Admin** modes.

The code is written in bare-metal C and targets **Keil µVision (MDK-ARM)**.

## Features

- **Fingerprint-based attendance**
  - GT-521F32 sensor connected to USART1 (PA9/PA10)
- **Two operating modes**
  - **User mode** – mark attendance with fingerprint
  - **Admin mode** – manage users via keypad and LCD
- **Interrupt-based inputs**
  - Touch input (sensor wake/LED control)
  - Mode selection switches on EXTI lines
- **16×2 LCD user interface**
  - Status messages, prompts, and simple feedback
- **4×4 keypad support**
  - Numeric and basic menu inputs

## Hardware

- Raayan Mini board (STM32F401RBT6, Cortex-M4)
- GT-521F32 fingerprint sensor module
- 16×2 character LCD (HD44780-compatible)
- 4×4 matrix keypad
- Two push-buttons for **DOWN** and **ENTER**
- ST-Link V2 debugger/programmer
- External 5 V / 3.3 V supply as per board requirements


## Pin Configuration

| Function                       | Peripheral / Signal       | MCU Pin     | Port | Notes                                      |
|--------------------------------|---------------------------|------------|------|--------------------------------------------|
| Fingerprint UART TX (MCU → FP)| USART1_TX                 | PA9        | A    | Connect to GT-521F32 RX                    |
| Fingerprint UART RX (FP → MCU)| USART1_RX                 | PA10       | A    | Connect to GT-521F32 TX                    |
| Fingerprint touch input       | EXTI line 6               | PA6        | A    | Rising/falling edge, controls CMOS LED     |
| Keypad row 0                  | GPIO output               | PC0        | C    | Row lines: PC0–PC3                         |
| Keypad row 1                  | GPIO output               | PC1        | C    |                                            |
| Keypad row 2                  | GPIO output               | PC2        | C    |                                            |
| Keypad row 3                  | GPIO output               | PC3        | C    |                                            |
| Keypad column 0               | GPIO input (pull-down)    | PC4        | C    | Column lines: PC4, PC5, PC11, PC12         |
| Keypad column 1               | GPIO input (pull-down)    | PC5        | C    |                                            |
| Keypad column 2               | GPIO input (pull-down)    | PC11       | C    |                                            |
| Keypad column 3               | GPIO input (pull-down)    | PC12       | C    |                                            |
| DOWN switch                   | EXTI line 9               | PC9        | C    | Triggers **User mode**                     |
| ENTER switch                  | EXTI line 10              | PC10       | C    | Triggers **Admin mode**                    |
| LCD data bus (4-bit mode)     | D4–D7                     | PB0–PB3    | B    | High and low nibble sent on PB0–PB3        |
| LCD RS                        | Register Select           | PB4        | B    | 0 = command, 1 = data                      |
| LCD RW                        | Read/Write                | PB5        | B    | 0 = write (used), 1 = read (optional)      |
| LCD EN                        | Enable strobe             | PB8        | B    | Rising edge latch                          |

> If your board revision uses different pins, please update the corresponding GPIO/EXTI configuration functions.

## Project Structure

- `Src/`
  - `BAS_using_GT521F32.c` / `main.c` – main application and mode control
  - `FP_gt521f32.c` – GT-521F32 sensor driver and command handling
  - `UART1.c` – USART1 init/config and send/receive helpers
  - `km_lcd.c` – 16×2 LCD driver in 4-bit mode
  - `4x4_keypad.c` – keypad init and scanning
  - `Interrupt.c` – EXTI configuration and NVIC setup
  - `systemticktimer.c` – SysTick-based millisecond delays
  - `gpio.c`, `my_string.c` – basic GPIO and utility functions

---

## Build & Run (Keil µVision)
1. Download the repository as .zip
2. Extract the .zip file
3. Open the project `.uvprojx` file in **Keil µVision**.
4. Select the target for **STM32F401RBT6 / Raayan Mini**.
5. Build the project (`F7`).
6. Connect the board via **ST-Link V2**.
7. Flash the firmware (`Ctrl + F8`) and reset the board.

On startup, the firmware initializes USART1, LCD, keypad, SysTick, and interrupts, then waits for:

- **DOWN (PC9)** → enter **User mode** and capture attendance.
- **ENTER (PC10)** → enter **Admin mode** for management via keypad.

## 📸 Demo

[![Watch the Demo Video](https://img.youtube.com/vi/u7JY6go5ghE/maxresdefault.jpg)](https://www.youtube.com/watch?v=u7JY6go5ghE)

> 🎥 Click the image to watch the Smart Weather Monitor System in action!

