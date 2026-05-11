# MSP430 Embedded RC Car

## Overview

This project is a real-time embedded systems RC car developed using the MSP430FR2355 microcontroller. The system integrates motor control, timers, interrupts, sensor input, UART communication, and peripheral interfacing to create a multifunctional autonomous and user-controlled vehicle platform.

The RC car also supports IoT-based wireless control through a mobile phone interface. When both the RC car communication module and the user’s phone are connected to the same Wi-Fi network, the vehicle can receive and execute movement commands remotely through phone-based control.

The project was developed to strengthen understanding of embedded software design, hardware/software integration, interrupt-driven systems, wireless communication, and low-level microcontroller programming.

---

## Features

- MSP430FR2355-based embedded control

- DC motor control for directional movement

- Timer-driven motion control

- GPIO and interrupt-based input handling

- UART serial communication

- LCD display integration

- IR sensor support for line detection

- IoT-based wireless phone control

- Remote movement control over Wi-Fi

- Multiple movement modes and state handling

- Modular embedded software architecture

---

## Hardware Components

- MSP430FR2355 LaunchPad

- DC motors

- Motor driver module

- IR sensors

- LCD display

- Push buttons / switches

- Wi-Fi communication module

- Battery power supply

- RC car chassis

---

## Software and Tools

- C Programming Language

- Code Composer Studio (CCS)

- MSP430 Driver Libraries

- UART Communication

- Embedded Timers and Interrupts

- IoT Communication Interface

---

## Repository Structure

```text

Embedded-RC-Car-MSP430/

├── src/            # Main system initialization and program flow

├── include/        # Header files and macros

├── drivers/        # Low-level MSP430 hardware drivers

├── peripherals/    # Peripheral control modules

├── app/            # High-level application logic

├── linker/         # MSP430 linker configuration
