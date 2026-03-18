# 🚗 Self-Driving Vehicle — Arduino Robotics Suite

A hands-on embedded systems project built around three progressively complex autonomous vehicle modules, all written in C++ for Arduino-compatible hardware. This repo documents my journey from remote-controlled basics to full sensor-driven autonomy — each module is self-contained, well-commented, and designed to actually work on real hardware.

---

## 📌 Table of Contents

- [Overview](#overview)
- [Project Modules](#project-modules)
  - [1. WiFi Controlled Bot](#1-wifi-controlled-bot)
  - [2. Obstacle Avoiding Bot](#2-obstacle-avoiding-bot)
  - [3. Lane Follower Bot](#3-lane-follower-bot)
- [Hardware Requirements](#hardware-requirements)
- [Repository Structure](#repository-structure)
- [Getting Started](#getting-started)
- [Wiring Guide](#wiring-guide)
- [How It Works — Under the Hood](#how-it-works--under-the-hood)
- [Known Limitations & Future Work](#known-limitations--future-work)
- [Contributing](#contributing)
- [License](#license)

---

## Overview

This project started as a robotics experiment during my undergraduate studies — I wanted to understand autonomous navigation from scratch, without relying on high-level libraries that abstract away the interesting parts. The result is three distinct bot configurations that together cover the three fundamental challenges in mobile robotics: **manual control**, **obstacle perception**, and **path following**.

All three modules share a common hardware backbone (DC motors + L298N driver + Arduino) but differ significantly in their sensing and decision logic. You can build them independently or treat them as progressive stages of the same vehicle.

---

## Project Modules

### 1. WiFi Controlled Bot

**Folder:** `wifi_controlled_bot/`

The foundation module. This bot connects to a local WiFi network using an ESP8266/NodeMCU and exposes a simple HTTP interface so you can drive it from a browser or a custom app on your phone.

**Key features:**

- Hosts a lightweight web server directly on the microcontroller
- Accepts directional commands (forward, backward, left, right, stop) over HTTP GET requests
- Low-latency control — response time is typically under 50ms on a local network
- No external app needed; works straight from a browser

**Use case:** Good starting point if you want to understand how embedded web servers work, or if you just want a remotely controlled rover before adding autonomy on top.

---

### 2. Obstacle Avoiding Bot

**Folder:** `obstancle_avoiding_bot/`

This module adds a servo-mounted HC-SR04 ultrasonic sensor to the chassis and gives the bot the ability to navigate around objects without any human input. It's genuinely satisfying to watch it decide where to go on its own.

**Key features:**

- Ultrasonic ranging at ~10Hz — measures distances ahead in real time
- Servo sweeps left and right to compare obstacle distances on both sides
- Decision logic: if the path ahead is clear (> threshold distance), go forward; if blocked, stop, scan, and turn toward the clearer side
- Configurable distance thresholds directly in the sketch

**How the avoidance logic works:**

```
1. Measure front distance
2. If distance > MIN_DISTANCE → drive forward
3. If distance ≤ MIN_DISTANCE → stop
4. Sweep servo left → record left_dist
5. Sweep servo right → record right_dist
6. If left_dist > right_dist → turn left, else → turn right
7. Resume forward motion, repeat
```

**Use case:** Great for understanding reactive control — no mapping, no planning, just sensor-reflex loops.

---

### 3. Lane Follower Bot

**Folder:** `lane_follower_bot/`

The most interesting module from a controls perspective. This bot uses an array of IR sensors mounted low to the ground to detect a black line on a white surface and follow it at speed. The key here is the feedback loop — a basic proportional controller that keeps the bot centered on the line.

**Key features:**

- 5-sensor IR array for line detection (or compatible configuration)
- Proportional control: deviation from center → proportional motor speed correction
- Handles curves, S-bends, and sharp turns reasonably well
- Speed and sensitivity are tunable via constants at the top of the sketch

**Sensor reading logic:**

```
Sensor positions: [S1] [S2] [S3] [S4] [S5]
                   ←         ↑         →
                  (left)  (center)  (right)

Error = weighted sum of active sensors
If error < 0 → bot is drifting left → increase right motor speed
If error > 0 → bot is drifting right → increase left motor speed
If error = 0 → go straight at base speed
```

**Use case:** Classic line-following is a rite of passage in robotics — and the proportional control here is directly analogous to what you'd find in real steering controllers.

---

## Hardware Requirements

| Component                   | Quantity | Notes                       |
| --------------------------- | -------- | --------------------------- |
| Arduino Uno / Nano          | 1        | Any AVR-based board works   |
| ESP8266 / NodeMCU           | 1        | For WiFi module only        |
| L298N Motor Driver          | 1        | Shared across all modules   |
| DC Gear Motors (TT motors)  | 2–4      | Depends on chassis          |
| HC-SR04 Ultrasonic Sensor   | 1        | Obstacle avoider module     |
| SG90 / MG90S Servo          | 1        | For servo pan on ultrasonic |
| IR Sensor Array (5-channel) | 1        | Lane follower module        |
| LiPo / Li-ion Battery Pack  | 1        | 7.4V recommended            |
| Chassis + wheels + casters  | 1 set    | 2WD or 4WD                  |
| Jumper wires, breadboard    | —        | Standard prototyping stuff  |

> **Power note:** Run motors and Arduino from separate power rails where possible. Motor inrush current on the same 5V line tends to cause resets at the worst moments.

---

## Repository Structure

```
self-driving-vehicle/
│
├── lane_follower_bot/
│   └── lane_follower_bot.ino       # IR sensor line following with P-control
│
├── obstancle_avoiding_bot/
│   └── obstancle_avoiding_bot.ino  # Ultrasonic + servo obstacle avoidance
│
└── wifi_controlled_bot/
    └── wifi_controlled_bot.ino     # ESP8266 WiFi HTTP server control
```

Each folder is a standalone Arduino sketch. Open the `.ino` file directly in the Arduino IDE and you're good to go.

---

## Getting Started

### Prerequisites

- [Arduino IDE](https://www.arduino.cc/en/software) (1.8.x or 2.x)
- For the WiFi bot: **ESP8266 board support** installed via Board Manager
  - Add this URL in Preferences → Additional Board URLs:
    ```
    http://arduino.esp8266.com/stable/package_esp8266com_index.json
    ```
- For the WiFi bot: **ESP8266WiFi** library (comes bundled with the ESP8266 board package)

### Uploading a Sketch

1. Clone the repo:

   ```bash
   git clone https://github.com/Suke2004/self-driving-vehicle.git
   cd self-driving-vehicle
   ```

2. Open the `.ino` file for the module you want in the Arduino IDE.

3. Select the correct board:
   - Arduino Uno/Nano → for lane follower and obstacle avoider
   - NodeMCU 1.0 (ESP-12E) → for WiFi bot

4. Set your COM port, hit **Upload**.

5. Open Serial Monitor at **9600 baud** for debug output.

---

## Wiring Guide

### Common Motor Driver (L298N) Connections

```
L298N      →   Arduino
IN1        →   D2
IN2        →   D3
IN3        →   D4
IN4        →   D5
ENA (PWM)  →   D6
ENB (PWM)  →   D9
GND        →   GND (shared with battery -)
12V        →   Battery +
5V out     →   Arduino Vin (optional — check your regulator)
```

### Obstacle Avoider — Additional Wiring

```
HC-SR04    →   Arduino
VCC        →   5V
GND        →   GND
TRIG       →   D10
ECHO       →   D11

SG90 Servo →   Arduino
Signal     →   D7
VCC        →   5V
GND        →   GND
```

### Lane Follower — IR Sensor Array

```
IR Array   →   Arduino
S1 (left)  →   A0
S2         →   A1
S3 (center)→   A2
S4         →   A3
S5 (right) →   A4
VCC        →   5V
GND        →   GND
```

### WiFi Bot — ESP8266 / NodeMCU

The motor driver connects to the NodeMCU's GPIO pins instead of an Arduino Uno in this configuration. Update the pin definitions in the sketch to match NodeMCU's GPIO numbering (D1, D2, etc. map to GPIO5, GPIO4, etc.).

Don't forget to set your credentials before uploading:

```cpp
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

---

## How It Works — Under the Hood

### Motor Control

All three modules use the L298N dual H-bridge to drive two DC motors. Speed is controlled via PWM (`analogWrite`) on the enable pins, and direction via the IN1–IN4 logic pins. Forward, backward, left, and right are just combinations of these four signals.

### Ultrasonic Distance Measurement

The HC-SR04 works by sending a 10μs pulse on TRIG and timing how long the ECHO pin stays HIGH. Distance in cm is:

```
distance = (echo_duration_μs × 0.034) / 2
```

The divide-by-2 accounts for the round-trip travel time of the sound pulse.

### IR Line Detection

IR sensors reflect light differently off black (absorbs) vs. white (reflects) surfaces. The analog voltage on each sensor pin tells you which sensors are "seeing" the line. A weighted average of active sensors gives you the lateral error, which drives the motor speed correction.

### Proportional Control (Lane Follower)

```cpp
int error = (weighted_sensor_sum) - CENTER_SETPOINT;
int leftSpeed  = BASE_SPEED + (Kp * error);
int rightSpeed = BASE_SPEED - (Kp * error);
```

`Kp` is your proportional gain. Too low and the bot wanders; too high and it oscillates. Tune it empirically — start around 0.3–0.5 and adjust based on your motor characteristics and track width.

---

## Known Limitations & Future Work

A few things I'm aware of that could be improved:

- **No PID on lane follower** — currently uses P-only control. Adding an integral and derivative term would improve high-speed stability significantly.
- **Obstacle avoider has no memory** — it's purely reactive. It can get stuck in corners or oscillate between two equidistant obstacles. A simple wall-following state machine would help.
- **WiFi bot has no authentication** — anyone on the same network can control it. Fine for a home lab, not for anything else.
- **No encoder feedback** — motor speeds are set open-loop. On uneven surfaces or with mismatched motors, the bot will drift. Rotary encoders + closed-loop control would fix this.
- **Battery voltage sag** — as the battery discharges, motor speed drops noticeably. A voltage compensation factor in the PWM calculation would help consistency.

If you pick any of these up and want to contribute, PRs are welcome.

---

## Contributing

This is a learning project, so contributions that improve clarity, fix bugs, or extend functionality are genuinely appreciated. If you port this to a different microcontroller platform (STM32, RP2040, ESP32), I'd love to see it.

1. Fork the repo
2. Create a branch: `git checkout -b feature/your-feature`
3. Commit your changes with a clear message
4. Open a pull request

Please keep sketches well-commented — the goal is that someone new to embedded systems can read the code and understand what's happening without needing a separate tutorial.

---

## License

This project is open source under the [MIT License](LICENSE). Use it, modify it, build on it — just don't remove the attribution.

---

_Built as part of ongoing embedded systems and robotics work during undergrad. If you're working on something similar and want to swap notes, feel free to reach out._
