# BioGUI

**Basic HMI framework for TFT LCD MCU touchscreens**

A lightweight C++ framework designed to create simple Human-Machine Interfaces (HMIs) for microcontroller-based TFT touchscreen displays — primarily developed for research equipment at the Institute of Biology, University of Brasília (UnB).

This project was created as part of a master's degree in Biomedical Engineering. It was used on real research equipment, enabling the work reported in [[1]](https://doi.org/10.3390/ph18020240) and [[2]](https://doi.org/10.3390/pharmaceutics16091177).

## Features

- Widget-based UI: Button, Slider, Label, Timer
- Screen management with easy navigation
- Router system that handles screen transitions and widget updates
- Designed to work with arduino shield TFT LCD 320x480 touchscreen 
- PlatformIO friendly project structure

## Supported Widgets

| Widget   | Description                                      | Interaction                              |
|----------|--------------------------------------------------|------------------------------------------|
| **Button**   | Simple touchable button with user-defined callback            | Visual press feedback + callback |
| **Slider**   | Horizontal slider to adjust numeric values       | Touch + drag                             |
| **Label**    | Text display                   | Programmatically updatable               |
| **Timer**    | Countdown/up timer with visual feedback          | Start/pause/reset + time display         |

## Screens & Navigation

A **Screen** is a logical grouping of widgets that serve a specific purpose (example: "Main Dashboard", "Settings", "Running Protocol").

The **Router** handles:
- Switching between screens
- Automatic drawing/updating of visible widgets
- Clean transitions (clear screen → draw new content)

## Requirements

- C++11 or later
- TFT touchscreen display library (e.g. TFT_eSPI, Adafruit_GFX + touch support)
- PlatformIO (recommended) or compatible build system for MCUs (ESP32, STM32, etc.)
