# LoRa-TX : Transmitter with ESP32-S3
## Module use :
### 1. ESP32-S3-N16R8
![WhatsApp Image 2025-05-11 at 23 16 03_c43a9188](https://github.com/user-attachments/assets/7a96e4a9-bcb0-421e-bf54-0e8a605342a3)
### 2. LoRa Ra-01 (433mhz)
![LORA](https://github.com/user-attachments/assets/5f3af303-5f3d-44d7-9a77-7d96ffbe4a60)

This project implements a simple LoRa transmitter using the ESP32-S3 microcontroller and the LoRa module (typically an SX127x based module). The transmitter sends messages in a loop, with a 20-second delay between each message. The system also utilizes a built-in LED to indicate the transmission status.

## Features

- Custom pin mapping for the ESP32-S3 GPIO pins.
- Sends a list of predefined messages via LoRa at regular intervals.
- Built-in LED (GPIO2) flashes to indicate transmission status.
- Supports LoRa communication at 433 MHz.
- Simple setup with basic SPI and LoRa library integration.

## Hardware Requirements

- **ESP32-S3** (e.g., ESP32-S3-N16R8).
- **LoRa Module** (e.g., SX1278 or SX1276) connected via SPI.
- **LED** connected to GPIO2 (or another available GPIO) to indicate the transmission status.

### LoRa Pin Mapping

This project uses the following custom pin mappings for the LoRa module:

| LoRa Pin    | ESP32 Pin  |
|-------------|------------|
| **SCK**     | GPIO48     |
| **MISO**    | GPIO45     |
| **MOSI**    | GPIO35     |
| **SS**      | GPIO47     |
| **RST**     | GPIO21     |
| **DIO0**    | GPIO20     |

## Software Requirements

- **Arduino IDE** with support for ESP32 boards.
- **LoRa Library**: Install the [LoRa library](https://github.com/sandeepmistry/arduino-LoRa) via the Arduino Library Manager.
- **SPI Library**: This library is part of the standard Arduino setup.

## Setup Instructions

1. **Install the ESP32 Board in Arduino IDE**:
   - Go to `File > Preferences` and add the following URL to the "Additional Boards Manager URLs":
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Go to `Tools > Board > Boards Manager` and install the **esp32** platform.

2. **Install the LoRa Library**:
   - Go to `Sketch > Include Library > Manage Libraries`.
   - Search for `LoRa` and install the library by **Sandeep Mistry**.

3. **Upload the Code**:
   - Connect your ESP32-S3 to your computer.
   - Open the `LoRa Transmitter` sketch in Arduino IDE.
   - Select the correct board and port under `Tools > Board` and `Tools > Port`.
   - Upload the sketch to your ESP32-S3.

4. **Testing**:
   - Once the code is uploaded, open the Serial Monitor at a baud rate of 115200 to see the transmission status.
   - The LED on GPIO2 should blink every time a message is sent, and you should see the "Sending" messages in the Serial Monitor.

## How it Works

- The program initializes the LoRa module with custom pin mapping.
- It sends messages from the predefined list every 20 seconds.
- The LED on GPIO2 blinks to indicate that a message is being transmitted.
- You can easily modify the list of messages by editing the `messages[]` array in the code.

## License

This project is open-source and released under the [MIT License](https://mit-license.org/).
