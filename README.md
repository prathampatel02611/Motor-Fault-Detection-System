# IoT-Based Smart Motor Condition Monitoring and Fault Protection System

Embedded IoT system for real-time monitoring and fault protection of DC motors using multi-sensor condition monitoring.

---

## Overview

This project implements an **IoT-based motor condition monitoring and protection system** designed to detect abnormal operating conditions in a **DC motor** using real-time sensor data.

The system integrates **current sensing, temperature monitoring, and vibration analysis** to detect multiple fault conditions such as overload, overheating, excessive vibration, motor stall, and abnormal operating states.

An **ESP32 microcontroller** processes the sensor data, performs fault detection logic, and communicates with the **Blynk IoT cloud platform** to provide remote monitoring and control through a dashboard.

The system also includes an **automatic shutdown mechanism** to protect the motor in case of persistent critical faults.

---

## Key Features

* Real-time **motor condition monitoring**
* Multi-sensor **fault detection system**
* **Current monitoring** using ACS712 sensor
* **Temperature monitoring** using DS18B20 sensor
* **Vibration monitoring** using SW-420 vibration sensor
* **IoT remote monitoring** using Blynk cloud platform
* **Automatic motor shutdown for critical faults**
* Remote **motor ON/OFF control** via dashboard
* Fault classification system with **warning and critical levels**

---

## System Architecture

DC Motor

↓

Sensors (ACS712 Current Sensor, DS18B20 Temperature Sensor, SW420 Vibration Sensor)

↓

ESP32 Microcontroller

↓

Fault Detection Algorithm

↓

Relay Motor Control + Blynk Cloud

↓

Remote Monitoring Dashboard

---

## Hardware Components

| Component                  | Purpose                               |
| -------------------------- | ------------------------------------- |
| ESP32 Microcontroller      | Main processing and IoT communication |
| ACS712 Current Sensor      | Measures motor current                |
| DS18B20 Temperature Sensor | Monitors motor temperature            |
| SW420 Vibration Sensor     | Detects abnormal vibrations           |
| Relay Module               | Motor switching and protection        |
| DC Motor                   | System under monitoring               |
| Breadboard & Power Supply  | Prototype hardware setup              |

---

## Fault Detection Logic

The system continuously analyzes sensor data and classifies motor conditions into multiple states.

### Current-Based Faults

* Motor Stall Detection
* High Current Warning
* Elevated Current Detection
* Low Current Detection
* Motor Not Running Fault

### Temperature-Based Faults

* High Temperature Warning
* Critical Overheating Protection

### Vibration-Based Faults

* High Vibration Warning
* Excessive Vibration Fault

---

## Intelligent Protection Features

### Startup Fault Filtering

Fault detection is temporarily ignored during motor startup to prevent false alarms caused by inrush current.

### Fault Persistence Detection

Critical faults must persist for a defined duration before triggering motor shutdown, improving reliability and preventing false shutdown events.

### Automatic Motor Shutdown

If a **critical fault persists**, the system automatically disables the motor using the relay module and generates an alert on the IoT dashboard.

---

## IoT Dashboard

The Blynk IoT dashboard provides:

* Real-time current monitoring
* Real-time temperature monitoring
* Vibration activity tracking
* Motor ON/OFF control
* Auto-shutdown enable/disable control
* Fault status display and alerts

---

## System Demonstration

### Serial Monitor Output
Real-time sensor data and fault status are displayed on the serial monitor.

### IoT Dashboard
The Blynk dashboard provides live visualization of motor parameters including current, temperature, vibration, and fault status.

### Hardware Prototype
The system was implemented using a breadboard prototype consisting of an ESP32 microcontroller, sensors, relay module, and DC motor.

## Example Output (Serial Monitor)

```
========== SENSOR READINGS ==========
Motor: RUNNING
Current: 4.749 A
Temperature: 29.8 C
Vibration Count: 0
Fault Status: OK
=====================================
```

---

## Applications

* Industrial motor condition monitoring
* Predictive maintenance systems
* Smart factory monitoring systems
* Electrical equipment health monitoring
* Remote machine diagnostics

---

## Repository Structure

```
Motor-Fault-Detection-System
│
├── code
│   └── motor_fault_detection.ino
│
├── images
│   ├── prototype_setup.jpg
│   ├── dashboard.png
│   └── system_block_diagram.png
│
└── README.md
```

---

## Skills Demonstrated

- Embedded Systems Development
- IoT System Design
- Sensor Interfacing
- Fault Detection Algorithms
- Industrial Condition Monitoring
- Real-Time Data Processing
- ESP32 Microcontroller Programming

---
## Project Status

This project is currently under active development.

The core monitoring and fault detection system has been implemented, and further improvements are being developed to enhance reliability and predictive monitoring capabilities.

### Development Progress

- [x] Sensor integration (Current, Temperature, Vibration)
- [x] Real-time data monitoring
- [x] IoT dashboard integration (Blynk)
- [x] Motor fault detection logic
- [x] Automatic motor shutdown protection


## Author

Pratham Patel

Electronics and Communication Engineering
