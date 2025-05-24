# RPiCAN

This project aims to establish CAN communication between a Raspberry Pi and an STM32 microcontroller, integrating sensor data monitoring and LED control within a home automation system using Domoticz.

## STM32 (Bluepill)

The project utilizes an STM32F103C8 ("Bluepill") microcontroller, connected via I2C to:
- **MCP9808** – Temperature sensor
- **TSL2851** – Luminosity sensor  
Additionally, an LED is connected to the STM32.  

Every 10 seconds, the STM32:
- Collects measurements from the sensors.
- Transmits these measurements via CAN frames over the CAN bus to the Raspberry Pi.
- Determines the LED state based on the detection of a specific CAN frame on the bus.

### CAN Frame Data Field Format
| Data Type   | DLC | Format                                    |
|-------------|-----|--------------------------------------------|
| Temperature | 3   | Byte1: `0x11`, Byte2-3: Temperature data  |
| Luminosity  | 3   | Byte1: `0x22`, Byte2-3: Luminosity data   |
| LED State   | 2   | Byte1: `0x33`, Byte2: LED State           |

The STM32 communicates over CAN using pins:
- A11: `CANRx`
- A12: `CANTx`  
These connect to a CAN transceiver providing differential CANH and CANL signals.

## Raspberry Pi with MCP2515

Since the Raspberry Pi lacks a native CAN interface, we use the **MCP2515** SPI-to-CAN controller. The **HW-184** module integrates an MCP2515 and a CAN transceiver, converting SPI frames to CAN differential signals and vice versa.

⚠️ **Voltage Adjustment:**  
The HW-184 module is powered at 5V, but the Raspberry Pi’s GPIO operates at 3.3V. Direct connection risks damaging the Pi. To resolve this:
- Disconnect the Vcc connection between MCP2515 and the transceiver.
- Power the MCP2515 with 3.3V and the transceiver with 5V.  
The transceiver interprets 3.3V logic levels from the MCP2515 as valid "high" signals.

📖 **Setup Guide**: [Detailed Instructions](https://forums.raspberrypi.com/viewtopic.php?t=141052)

## Software

- The provided Raspberry Pi code listens to incoming CAN frames and can transmit messages.
- Integration with **Domoticz** allows data visualization and LED control via a home automation interface.

## Domoticz Integration

[Domoticz](https://www.domoticz.com/) is a home automation platform that supports monitoring and control of various devices:
- Lights, switches, sensors (temperature, rain, wind, UV, electricity, gas, water, etc.)

In this project:
- Domoticz displays temperature and luminosity readings from the sensors.
- LED control is handled through specific CAN frames sent by Domoticz via MQTT to the Raspberry Pi.

---

### 📡 Overview
- **Hardware:** Raspberry Pi + MCP2515 CAN module, STM32F103C8 ("Bluepill"), MCP9808, TSL2851, CAN transceiver
- **Communication:** CAN bus between Raspberry Pi and STM32
- **Software:** Python/C code for CAN communication, MQTT for Domoticz integration
- **Functionality:** Temperature and luminosity monitoring, LED control, home automation integration with Domoticz

----
## Architecture
![alt text](https://github.com/jeremynguyenn/CAN-Communication-Between-Raspberry-Pi-and-STM32/blob/main/CAN%20communication%20a%20Raspberry%20Pi%20and%20an%20STM32%20device/image.png)

## Wiring
### MCP9808:
- Vdd <-> 3v3
- GND <-> GND
- SCL <-> B6
- SDA <-> B7
- A0,A1,A2 <-> GND (I2CADDR=0x18)
### TSL2561:
- Vin <-> NC
- GND <-> GND
- 3Vo <-> 3v3
- Addr <-> NC
- Int <-> NC
- SDA <-> B7
- SCL <-> B6

![alt text](https://github.com/jeremynguyenn/CAN-Communication-Between-Raspberry-Pi-and-STM32/blob/main/CAN%20communication%20a%20Raspberry%20Pi%20and%20an%20STM32%20device/wiring.png)

----
## Realistic model
![Modelimage](https://github.com/user-attachments/assets/3673ad14-af76-496f-bc96-0fe689c73629)

----
## Analyzing Frames with Analyzer
Frame I2C
- Luminosity TSL2561 0x39:
![image](https://github.com/user-attachments/assets/bad25f50-248a-4f7d-8b79-0b5b60d419c9)

- Temperature MCP9808 0x18:
![image](https://github.com/user-attachments/assets/072993a5-2c2e-4cf9-9d2f-4b7411a64679)

Frame CAN
- Luminosity ID 0x22 DATA: 2Bytes
![image](https://github.com/user-attachments/assets/9e8a32ea-7266-4edb-aec2-b4aed1adab32)

- Temperature ID 0x11 DATA: 2Bytes
![image](https://github.com/user-attachments/assets/3d2c8bc3-68fb-4796-8273-c480dfab3ca8)

- CAN raspberry pi:
![image](https://github.com/user-attachments/assets/0ccc5568-7ab9-40de-8132-9ea288159410)

- LED ID 0x33 DATA:{1/ON : 0/OFF}
![image](https://github.com/user-attachments/assets/82aa4d7d-03a7-4f69-95fe-a4194b69b217)

----
## Result interface:
![image](https://github.com/user-attachments/assets/292d7f35-9f0b-405b-b464-19a70f986ec7)

## Domoticz Interface:
- Temperature ID 0x11
![image](https://github.com/user-attachments/assets/b6dd7184-c417-4fdb-b43e-c7655dbc1fc4)


- Luminosity ID 0x22
![image](https://github.com/user-attachments/assets/c62213cf-5492-4fc7-9dae-46fc9f7cf45f)
