# Boost Converter

> **Development Documentation**
> Complete hardware design, electrical loss analysis, and embedded firmware implementation for a dsPIC-based open-loop boost converter prototype. Features high-speed PWM actuation, potentiometer-driven variable duty cycle control, LM358 signal conditioning, timer soft-start, and real-time $I^2C$ character display instrumentation.

---

## Table of Contents

1. [Overview](#1-overview)
2. [Key Features](#2-key-features)
3. [Hardware Specifications](#3-hardware-specifications)
4. [Theoretical Analysis](#4-theoretical-analysis)
    * [4.1 CCM Transfer Function & Ripple Dynamics](#41-ccm-transfer-function--ripple-dynamics)
    * [4.2 Comprehensive Power Loss Breakdown](#42-comprehensive-power-loss-breakdown)
    * [4.3 System Efficiency & Thermal Model](#43-system-efficiency--thermal-model)


5. [System Architecture & Signal Flow](#5-system-architecture--signal-flow)

6. [Subsystem Specifications & Firmware Implementation](#6-subsystem-specifications--firmware-implementation)
    * [6.1 Variable Duty Cycle & Soft-Start Sequencing](#61-variable-duty-cycle--soft-start-sequencing)
    * [6.2 LM358 Signal Conditioning & Dual ADC Acquisition](#62-lm358-signal-conditioning--dual-adc-acquisition)
    * [6.3 $I^2C$ Character LCD Interface](#63--character-lcd-interface)


7. [Control Topology](#7-control-topology)
8. [Repository Layout](#8-repository-layout)
9. [Development Environment & Hardware Toolchain](#9-development-environment--hardware-toolchain)
10. [Verification & Testing](#10-verification--testing)
11. [Feature Implementation Status](#11-feature-implementation-status)
12. [Future Improvements](#12-future-improvements)
13. [License](#13-license)

---

## 1. Overview

This repository contains the hardware design, loss modeling, and dsPIC embedded firmware implementation for an **Open-Loop Boost Converter Prototype**. Built around Microchip's **dsPIC** Digital Signal Controller architecture, the platform provides high Speed PWM signal generation and dynamic duty cycle adjustment via inbuilt ADC monitoring and analog potentiometer.

The system converts a nominal $12\text{ V}$ DC input to $24\text{ V}$ DC at $50\%$ duty cycle, while enabling real-time manual sweeps across the duty cycle range. Output telemetry is acquired via synchronized ADC channels and displayed on a local $I^2C$ character LCD.

---

## 2. Key Features

* **dsPIC Core & MCC Toolchain:** Built with Microchip MPLAB Code Configurator (MCC) for low-level peripheral driver generation and precise timing control.
* **Potentiometer Duty Cycle Control:** Manual real-time PWM duty cycle manipulation via analog potentiometer sampling.
* **Controlled Soft-Start Sequence:** Ramp-up initialization sequence managed by hardware timers to eliminate switch-on inrush current and voltage spikes.
* **LM358 Signal Conditioning:** Op-amp differential filtering and voltage/current attenuation for precise analog measurement.
* **Synchronized ADC Acquisition:** Dual acquisition mode supporting both software-initiated execution and cycle-matched PWM hardware interrupts.
* **$I^2C$ LCD Instrumentation:** Real-time visual display of electrical parameters ($V_{\text{IN}}$, $V_{\text{OUT}}$, $I_{\text{IN}}$, and $I_{\text{OUT}}$).
* **Detailed Analytical Loss Modeling:** Integrated mathematical expressions for MOSFET conduction/switching losses, inductor DCR, capacitor ESR, and diode drops.

---

## 3. Hardware Specifications

| Component / Parameter | Value / Part Number | Absolute Stress Rating / Specifications | Description |
| --- | --- | --- | --- |
| **Input Voltage ($V_{\text{IN}}$)** | $12\text{ V DC}$ | Range: $10\text{ V}$ to $15\text{ V DC}$ | Primary DC power supply input |
| **Output Voltage ($V_{\text{OUT}}$)** | $24\text{ V DC}$ (at $D = 0.5$) | Maximum design target: $60\text{ V DC}$ | Boosted output voltage level |
| **Switching Frequency ($f_{\text{sw}}$)** | $50\text{ kHz}$ | Hardware PWM timer clock setting | Power stage switching rate |
| **Energy Storage Inductor ($L$)** | $100\,\mu\text{H}$ | $I_{\text{sat}} = 5\text{ A}$, $\text{DCR} \approx 35\,\text{m}\Omega$ | Power stage inductor |
| **Input Filter Capacitor ($C_{\text{IN}}$)** | $220\,\mu\text{F} / 25\text{ V}$ | $\text{ESR} \approx 120\,\text{m}\Omega$ | Input ripple suppression capacitor |
| **Output Filter Capacitor ($C_{\text{OUT}}$)** | $220\,\mu\text{F} / 63\text{ V}$ | $\text{ESR} \approx 95\,\text{m}\Omega$ | High-voltage output decoupling capacitor |
| **Power Switch ($Q_1$)** | IRF540N N-Channel MOSFET | $V_{\text{DSS}} = 100\text{ V}$, $I_D = 33\text{ A}$, $R_{\text{DS(on)}} = 44\,\text{m}\Omega$ | Switching transistor |
| **Freewheel Diode / Switch** | IRF540N / High-Speed Diode | $V_{\text{RRM}} = 100\text{ V}$, $V_F \approx 0.7\text{ V}\text{--}1.1\text{ V}$ | Output rectification element |
| **Signal Conditioning Op-Amp** | LM358 Dual Op-Amp | $V_{\text{CC}} = 5\text{ V}$, Gain Bandwidth $= 1\text{ MHz}$ | Sensor buffer and differential amplifier |
| **Analog Controller** | $10\,\text{k}\Omega$ Potentiometer | Linear taper | Connected to dsPIC ADC for manual duty adjustment |

---

## 4. Theoretical Analysis

### 4.1 CCM Transfer Function & Ripple Dynamics

In Continuous Conduction Mode (CCM), the output voltage $V_{\text{OUT}}$ is defined by the input voltage $V_{\text{IN}}$ and the PWM duty ratio $D$:

$$V_{\text{OUT}} = \frac{V_{\text{IN}}}{1 - D}$$

* **At $50\%$ Duty Cycle ($D = 0.5$):**

$$V_{\text{OUT}} = \frac{12\text{ V}}{1 - 0.5} = \frac{12\text{ V}}{0.5} = 24\text{ V}$$

#### Peak-to-Peak Inductor Ripple Current ($\Delta I_L$)

$$\Delta I_L = \frac{V_{\text{IN}} \cdot D}{f_{\text{sw}} \cdot L}$$

*For $V_{\text{IN}} = 12\text{ V}$, $D = 0.5$, $f_{\text{sw}} = 50\text{ kHz}$, and $L = 100\,\mu\text{H}$:*

$$\Delta I_L = \frac{12 \cdot 0.5}{50 \times 10^3 \cdot 100 \times 10^{-6}} = 1.2\text{ A}$$

#### Output Voltage Ripple ($\Delta V_{\text{OUT}}$)

$$\Delta V_{\text{OUT}} = \frac{I_{\text{OUT}} \cdot D}{f_{\text{sw}} \cdot C_{\text{OUT}}} + I_{\text{OUT}} \cdot \text{ESR}_{\text{Cout}}$$

---

### 4.2 Comprehensive Power Loss Breakdown

Practical conversion efficiency is degraded by semiconductor, magnetic, and capacitive parasitic losses:

* MOSFET Conduction Loss ($P_{\text{cond,FET}}$)

    Occurs due to the channel resistance $R_{\text{DS(on)}}$ during the $D \cdot T_{\text{sw}}$ conduction interval:

    $$I_{\text{FET,rms}} = \frac{I_{\text{OUT}}}{1 - D} \sqrt{D}$$

    $$P_{\text{cond,FET}} = I_{\text{FET,rms}}^2 \cdot R_{\text{DS(on)}} = \left( \frac{I_{\text{OUT}}}{1 - D} \right)^2 \cdot D \cdot R_{\text{DS(on)}}$$

- MOSFET Switching Loss ($P_{\text{sw,FET}}$)

    Occurs during the rise time ($t_r$) and fall time ($t_f$) transitions:

    $$P_{\text{sw,FET}} = \frac{1}{2} \cdot V_{\text{OUT}} \cdot I_{\text{IN}} \cdot (t_r + t_f) \cdot f_{\text{sw}}$$

- Output Capacitance Drain Loss ($P_{\text{Coss}}$)

    Energy stored in $C_{\text{oss}}$ discharged to ground at each turn-on event:

    $$P_{\text{Coss}} = \frac{1}{2} \cdot C_{\text{oss}} \cdot V_{\text{OUT}}^2 \cdot f_{\text{sw}}$$

- Inductor Copper Loss ($P_{\text{DCR}}$)

    Conduction loss through the internal DC resistance ($\text{DCR}$) of the $100\,\mu\text{H}$ winding:

    $$P_{\text{DCR}} = I_{\text{IN}}^2 \cdot \text{DCR} = \left( \frac{I_{\text{OUT}}}{1 - D} \right)^2 \cdot \text{DCR}$$

- Freewheeling Diode Losses ($P_{\text{Diode}}$)

    Conduction and reverse recovery losses incurred during the off-state $(1 - D) \cdot T_{\text{sw}}$:

    $$P_{\text{cond,D}} = I_{\text{OUT}} \cdot V_F$$

    $$P_{\text{rr}} = Q_{\text{rr}} \cdot V_{\text{OUT}} \cdot f_{\text{sw}}$$

- Output Capacitor ESR Loss ($P_{\text{ESR,out}}$)

    Heating generated by high RMS ripple currents passing through $C_{\text{OUT}}$ ESR:

    $$I_{\text{Cout,rms}} = I_{\text{OUT}} \sqrt{\frac{D}{1 - D}}$$

$$P_{\text{ESR,out}} = I_{\text{Cout,rms}}^2 \cdot \text{ESR}_{\text{Cout}} = I_{\text{OUT}}^2 \left( \frac{D}{1 - D} \right) \cdot \text{ESR}_{\text{Cout}}$$

---

### 4.3 System Efficiency & Thermal Model

Summing all individual loss terms yields the total converter power dissipation:

$$P_{\text{loss,total}} = P_{\text{cond,FET}} + P_{\text{sw,FET}} + P_{\text{Coss}} + P_{\text{DCR}} + P_{\text{core}} + P_{\text{cond,D}} + P_{\text{rr}} + P_{\text{ESR,out}} + P_{\text{aux}}$$

#### Total System Efficiency ($\eta$)

$$\eta = \frac{P_{\text{OUT}}}{P_{\text{IN}}} = \frac{P_{\text{OUT}}}{P_{\text{OUT}} + P_{\text{loss,total}}} \times 100\%$$

#### Thermal Junction Temperature Calculation ($T_J$)

For the IRF540N MOSFET operating with total semiconductor loss $P_{\text{FET,total}} = P_{\text{cond,FET}} + P_{\text{sw,FET}}$:

$$T_J = T_A + P_{\text{FET,total}} \cdot (\theta_{JC} + \theta_{CS} + \theta_{SA})$$

*Where:*

* $T_A$: Ambient Temperature ($25^\circ\text{C}$)
* $\theta_{JC}$: Junction-to-Case Thermal Resistance ($1.15^\circ\text{C/W}$)
* $\theta_{CS}$: Case-to-Sink Thermal Resistance ($0.5^\circ\text{C/W}$)
* $\theta_{SA}$: Heatsink-to-Ambient Thermal Resistance

---

## 5. System Architecture & Signal Flow

```text
 ┌─────────────────┐
 │ Potentiometer   ├──────────────┐
 └─────────────────┘              │
                                  ▼
 ┌─────────────────┐    ┌──────────────────┐    ┌─────────────────┐    ┌─────────────────┐
 │ Power Supply    ├───►│ Boost Power Stage├───►│ LM358 Op-Amp    ├───►│ dsPIC ADC       │
 │ (12V DC Input)  │    │ (100uH, IRF540N) │    │ Conditioning    │    │ Measurement     │
 └─────────────────┘    └────────┬─────────┘    └─────────────────┘    └────────┬────────┘
                                 ▲                                              │
                                 │                                              ▼
                        ┌────────┴─────────┐                           ┌─────────────────┐
                        │ dsPIC High-Speed │                           │ I²C LCD Display │
                        │ PWM Generation   │                           │ (Real-Time V/I) │
                        └──────────────────┘                           └─────────────────┘

```

---

## 6. Subsystem Specifications & Firmware Implementation

### 6.1 Variable Duty Cycle & Soft-Start Sequencing

The dsPIC generates a high-frequency PWM signal via its internal power conversion PWM module. During initial system start-up, the firmware enforces a timer-based ramp sequence before passing control to the user potentiometer.

```text
       [ System Reset / MCC Initialization ]
                         │
                         ▼
             [ Set Soft-Start Ramp ]
                         │
                         ▼
        ┌─────────────────────────────────┐
        │ Timer Interrupt Ramp Engine     │
        │ • Increment Duty Cycle: ΔD      │
        │ • Compare Duty against Target D │
        └────────────────┬────────────────┘
                         │
               ( Target D Reached )
                         │
                         ▼
    ┌─────────────────────────────────────────┐
    │ Open-Loop Potentiometer Control Engine  │
    │ • Read ADC (Potentiometer Pin)          │
    │ • Update PWM Duty Register (0% - 80%)   │
    └─────────────────────────────────────────┘

```

---

### 6.2 LM358 Signal Conditioning & Dual ADC Acquisition

The LM358 operational amplifier scales high-side output voltages and low-side current shunt signals down to the $0\text{--}3.3\text{ V}$ or $0\text{--}5\text{ V}$ dsPIC analog input limits.

* **Software Triggered Acquisition:** Continuous background loop sampling for potentiometer position updates and display refresh routines.
* **PWM-Synchronized Acquisition:** Hardware-triggered ADC sampling synchronized to the center/period match of the PWM waveform to eliminate switching noise artifacts during MOSFET turn-on and turn-off transients.

```text
  Signal Source ──► LM358 Op-Amp Gain/Filter ──► dsPIC ADC Pin ──► Digital Scaling ──► LCD Display

```

---

### 6.3 $I^2C$ Character LCD Interface

The display driver handles $I^2C$ communication protocol abstraction, providing non-blocking output formatting for real-time system monitoring.

* **Layer Responsibilities:**
* Low-level $I^2C$ bus control and address handling (PCF8574 I/O expander target).
* Command and display register management.
* Real-time numeric formatting for voltage ($V_{\text{IN}}, V_{\text{OUT}}$), current ($I_{\text{IN}}$), and duty cycle ($D$) readings.



---

## 7. Control Topology

The baseline implementation operates in **Open Loop with Dynamic Reference Input**. The user adjusts the duty cycle via the potentiometer, while the dsPIC measures system parameters purely for display, characterization, and safety bounds checking.

```text
  Potentiometer Analog Voltage
              │
              ▼
    ┌───────────────────┐
    │  dsPIC ADC Unit   │
    └─────────┬─────────┘
              │
              ▼
    ┌───────────────────┐         ┌──────────────────────┐         ┌──────────────────────┐
    │ dsPIC PWM Module  ├────────►│ IRF540N Power Switch ├────────►│ Output 24V Load      │
    └───────────────────┘         └──────────────────────┘         └──────────┬───────────┘
                                                                              │
                                                                              ▼
    ┌───────────────────┐         ┌──────────────────────┐         ┌──────────────────────┐
    │  I²C LCD Display  │◄────────┤ dsPIC Monitoring ADC │◄────────┤ LM358 Op-Amp Buffer  │
    └───────────────────┘         └──────────────────────┘         └──────────────────────┘

```

---

## 8. Repository Layout

```text
Boost-Converter/
├── docs/                      # Circuit schematics, loss modeling sheets, and calculations
├── firmware/
│   ├── BoostConverter.X/      # MPLAB X IDE Project Directory
│   │   ├── mcc_generated_files/ # MCC auto-generated drivers (PWM, ADC, I2C, Timers)
│   │   ├── drivers/
│   │   │   ├── lcd_i2c.c      # Custom I²C LCD library implementation
│   │   │   └── lcd_i2c.h      # LCD driver header file
│   │   └── main.c             # Potentiometer loop, soft-start, and execution engine
├── hardware/                  # Eagle/KiCAD files, BOM list, and PCB Layouts
└── README.md                  # System technical documentation

```

---

## 9. Development Environment & Hardware Toolchain

| Item | Specification |
| --- | --- |
| **Microcontroller Architecture** | Microchip dsPIC Digital Signal Controller |
| **Development Environment** | MPLAB X IDE |
| **Code Generation Engine** | MPLAB Code Configurator (MCC) |
| **Compiler Toolchain** | Microchip XC16 Compiler |
| **Gate Driver / Switching Element** | IRF540N N-Channel MOSFET |
| **Amplifier Hardware** | LM358 Operational Amplifier |
| **User Input Instrumentation** | $10\,\text{k}\Omega$ Linear Potentiometer |
| **Display Hardware** | $16\times2$ Character LCD with $I^2C$ Backpack (PCF8574) |

---

## 10. Verification & Testing

### Verification

* [x] **PWM Verification:** Verify switching frequency ($100\text{ kHz}$), duty cycle accuracy, and gate waveform integrity using an oscilloscope.
* [x] **Soft-Start Verification:** Observe initial PWM ramping curve at power-up to confirm absence of inrush current overshoot.
* [x] **Potentiometer Response:** Confirm smooth step response between $0\%$ and target upper duty limit ($D_{\text{max}} \approx 80\%$).
* [x] **LM358 Op-Amp Gain Accuracy:** Verify signal linear scaling from power rail to dsPIC ADC input pins.
* [x] **$I^2C$ Bus Verification:** Confirm proper bus timing and screen updates without display freezing.

### Power Stage Tests

- [x] **$12\text{ V} \to 24\text{ V}$ Step-Up Validation:** Confirm nominal $24\text{ V}$ output at $50\%$ duty cycle ($D = 0.5$).
- [x] **Thermal Analysis:** Monitor IRF540N MOSFET and $100\,\mu\text{H}$ inductor heat under resistive loads.
- [x] **Inductor Saturation Check:** Ensure current spikes remain below the $5\text{ A}$ saturation limit of the power inductor.
- [x] **Efficiency Characterization:** Compare measured efficiency against theoretical loss models across varying loads ($0.5\text{ A}$ to $3\text{ A}$).

---

## 11. Feature Implementation
| Feature / Subsystem | Hardware / Driver Stack | Status |
| --- | --- | --- |
| **High-Speed PWM** | dsPIC MCC PWM Driver | **Implemented** |
| **Timer Soft-Start** | dsPIC MCC Timer Engine | **Implemented** |
| **Potentiometer Input** | dsPIC ADC Channel | **Implemented** |
| **Op-Amp Conditioning** | LM358 Circuitry | **Implemented** |
| **PWM ADC Synchronization** | MCC Hardware Triggering | **Implemented** |
| **Display Interface** | Custom $I^2C$ LCD Library | **Implemented** |
| **Analytical Loss Model** | Mathematical Loss Framework | **Implemented** |
| **Closed-Loop Feedback** | Digital PID Algorithm | *Planned (Phase 2)* |
| **Hardware Protection** | Software OCP/OVP Shutdown | *Planned (Phase 2)* |

---

## 12. Future Improvements

1. **Closed-Loop Control Implementation:**
* Transition from manual potentiometer open-loop operation to automated digital PID voltage regulation.
* Implementation of Peak Current Mode Control (PCMC) utilizing dsPIC high-speed analog comparators.


2. **Protection Systems:**
* Over-Voltage Protection (OVP) and Over-Current Protection (OCP) fast shutdown routines.
* Thermal shutdown integration utilizing temperature sensing on the IRF540N heatsink.


3. **Telemetry & Efficiency Analytics:**
* Real-time calculation of converter efficiency ($\eta = \frac{P_{\text{OUT}}}{P_{\text{IN}}}$) directly inside the dsPIC application loop.
* High-speed UART output logging for MATLAB/Python plot generation.



---

## 13. License

This project is licensed under the [MIT License](https://www.google.com/search?q=LICENSE&utm_source=gemini). Feel free to adapt and expand this codebase for research, educational, and commercial power electronics projects.