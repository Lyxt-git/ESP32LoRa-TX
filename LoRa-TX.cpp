/*MIT License

Copyright (c) 2025 Lyxt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.*/

#include <SPI.h>
#include <LoRa.h>

// Custom pin mapping for transmitter, depend onyour pinouts
#define LORA_SCK    48  // GPIO48
#define LORA_MISO   45  // GPIO45
#define LORA_MOSI   35  // GPIO35
#define LORA_SS     47  // GPIO47
#define LORA_RST    21  // GPIO21
#define LORA_DIO0   20  // GPIO20

#define STATUS_LED  2   // Built-in LED (GPIO2)

int messageIndex = 0;

// List of messages to send
String messages[] = {"Hello  ESP32-S3Central"};

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW);

  // Blink on boot
  for (int i = 0; i < 3; i++) {
    digitalWrite(STATUS_LED, HIGH);
    delay(200);
    digitalWrite(STATUS_LED, LOW);
    delay(200);
  }

  // Reset LoRa
  pinMode(LORA_RST, OUTPUT);
  digitalWrite(LORA_RST, LOW);
  delay(100);
  digitalWrite(LORA_RST, HIGH);
  delay(100);

  // Start SPI
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed.");
    while (true) {
      digitalWrite(STATUS_LED, HIGH);
      delay(100);
      digitalWrite(STATUS_LED, LOW);
      delay(100);
    }
  }

  Serial.println("LoRa Transmitter Ready, the message will send each 20  second.");
  digitalWrite(STATUS_LED, HIGH);
}

void loop() {
  // Send current message based on messageIndex
  String message = messages[messageIndex];
  messageIndex = (messageIndex + 1) % (sizeof(messages) / sizeof(messages[0]));  // Loop through the messages

  Serial.print("Sending: ");
  Serial.println(message);

  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();

  // Flash LED to indicate transmission
  digitalWrite(STATUS_LED, LOW);
  delay(100);
  digitalWrite(STATUS_LED, HIGH);

  delay(20000);  // Send every 20 seconds
}
