# 3-Axis FFT Gyroscopic Vibration Analysis Rig

A high-performance embedded systems and signal processing platform designed to capture real-time spatial motion and analyze structural vibration dynamics using a custom Fast Fourier Transform (FFT) pipeline. This project bridges precision mechanical design, high-frequency sensor integration, and optimized low-level C++ firmware to execute real-time frequency-domain analysis.

---

## System Architecture & Engineering Highlights

The system is engineered to isolate, capture, and compute frequency components from a rotating gyroscopic frame to characterize mechanical harmonics and structural resonance.

### 1. Mechanical Design & Signal Integrity
*   **Dual Octagon Nested Gimbal:** Engineered a large-scale, precision-balanced dual octagon nested frame using 25mm aluminum rods and synchronized bearings to minimize structural misalignment.
*   **Anti-Fatigue Routing Architecture:** Integrated a custom slip-ring and routing topology to eliminate wire fatigue, signal attenuation, and electromagnetic interference (EMI) during high-frequency rotation testing.

### 2. High-Performance Firmware (`firmware/src/main.cpp`)
*   **Deterministic Fixed-Interval Sampling:** Utilized low-level hardware timers to sample the gyroscopic data at a rigid frequency (e.g., 1 kHz / 2 kHz), entirely eliminating sampling interval jitter that would skew Fourier calculations.
*   **In-Place Radix-2 Cooley-Tukey FFT:** To bypass the heavy processing and memory overhead of generic math libraries, a custom, lightweight Radix-2 FFT function was written from scratch in optimized C++.
*   **Fast Math Look-Up Tables (LUTs):** Replaced expensive runtime trigonometric operations (`std::sin()`, `std::cos()`) inside the tight execution loop with pre-computed Sine/Cosine Look-Up Tables to minimize calculation latency.
*   **Resource-Constrained Memory Management:** Maximized strict 2 KB SRAM limitations on the 8-bit microcontroller architecture, implementing specialized circular buffers to stream synchronized 12-bit datasets via UART interrupts without data dropouts or signal jitter[cite: 3].

---

## Key Applications

*   **Vibration Characterization:** Mapping motor harmonics, bearing defects, and structural degradation.
*   **Flight Controller Optimization:** Utilizing frequency spectrum peaks to assist in tuning flight controller PID attenuation filters.

---
# Repository Structure
```text
├── .gitignore               # Excludes compiler outputs and local IDE settings
├── README.md                # Master system documentation and project catalog
├── firmware/
│   └── src/
│       └── main.cpp         # Multi-threaded, interrupt-driven sensor tracking firmware
└── hardware/
    ├── cad/                 # STEP/STL files for structural joints and encoder mounts
    └── schematics/
        └── README.md        # Pin allocation matrix and I2C/Interrupt routing maps
