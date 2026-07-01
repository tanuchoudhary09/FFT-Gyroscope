# Electronics Architecture

This subfolder documents the electrical connections and power infrastructure of the 3-Axis Passive Gimbal Data Acquisition (DAQ) system.

## Power Routing
* **Primary Source:** 5V Portable USB Power Bank connected to the ESP32 Micro-USB port.
* **Sensor VCC Rail:** The 5V supply is routed from the ESP32 `Vin` pin to power the internal LEDs of the E38S6G5 Industrial Photoelectric Encoders. 
* **IMU Supply:** The MPU-6050 is supplied via the ESP32 `3V3` pin or the onboard regulator (5V `VCC` pin depending on specific breakout board population).

## Pin Assignment Matrix

| Component | Component Pin | ESP32 GPIO Pin | Signal Type |
| :--- | :--- | :--- | :--- |
| **MPU-6050 IMU** | SDA | GPIO 21 | I2C Data |
| | SCL | GPIO 22 | I2C Clock |
| **Encoder 1 (Yaw)** | Phase A | GPIO 12 | Quadrature Interrupt |
| | Phase B | GPIO 13 | Direction Signal |
| **Encoder 2 (Pitch)**| Phase A | GPIO 14 | Quadrature Interrupt |
| | Phase B | GPIO 27 | Direction Signal |
| **Encoder 3 (Roll)** | Phase A | GPIO 25 | Quadrature Interrupt |
| | Phase B | GPIO 26 | Direction Signal |
