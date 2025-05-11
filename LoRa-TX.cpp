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

// Custom pin mapping for transmitter, depend on your pinouts
#define LORA_SCK    48  // GPIO48
#define LORA_MISO   45  // GPIO45
#define LORA_MOSI   35  // GPIO35
#define LORA_SS     47  // GPIO47
#define LORA_RST    21  // GPIO21
#define LORA_DIO0   20  // GPIO20

#define STATUS_LED  2   // Built-in LED (GPIO2)
#define TEST_SPI_DATA 0xAA

int messageIndex = 0;

// List of messages to send
String messages[] = {"Hello ESP32-S3Central"};

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println(".");
  delay(1000);
  Serial.println("..");
  delay(1000);
  Serial.println("...");
  delay(2000);
  Serial.println("================================");
  Serial.println("=== Booting LoRa Transmitter ===");
  Serial.println("================================");
  delay(2000);
  Serial.println("Loading ....");
  delay(10000);
  diagnostic_gpio();     // GPIO state check
  test_spi();            // Optional SPI communication check

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

  Serial.println("===================================================");
  Serial.println("Transmitting, the message will send each 20 second.");
  Serial.println("===================================================");
  delay(2000);
  Serial.println("Data send from a list ....");
  delay(1000);
  Serial.println("");
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

// GPIO Diagnostic
void diagnostic_gpio() {
  Serial.println("");
  Serial.println("Performing GPIO diagnostic...");
  delay(3000);
  check_gpio(LORA_SCK, "SCK");
  delay(750);
  check_gpio(LORA_MISO, "MISO");
  delay(750);
  check_gpio(LORA_MOSI, "MOSI");
  delay(750);
  check_gpio(LORA_SS, "NSS");
  delay(750);
  check_gpio(LORA_RST, "RST");
  delay(750);
  check_gpio(LORA_DIO0, "DIO0");
  delay(1000);
  Serial.println("GPIO diagnostic completed.");
  Serial.println("");
  delay(2000);
}

void check_gpio(int pin, const char* pinName) {
  pinMode(pin, INPUT);
  int state = digitalRead(pin);
  Serial.print(pinName);
  Serial.print(" pin is ");
  Serial.println(state == LOW ? "LOW (correct)" : "HIGH (unexpected)");
}

// SPI Communication Test
void test_spi() {
  Serial.println("Testing SPI communication...");
  delay(5000);
  uint8_t received = 0;

  pinMode(LORA_SS, OUTPUT);
  digitalWrite(LORA_SS, HIGH);

  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));  // Slow for stability
  digitalWrite(LORA_SS, LOW);
  received = SPI.transfer(TEST_SPI_DATA);
  digitalWrite(LORA_SS, HIGH);
  SPI.endTransaction();

  Serial.print("Sent: 0x");
  Serial.print(TEST_SPI_DATA, HEX);
  Serial.print(" | Received: 0x");
  Serial.println(received, HEX);

  if (received != 0x00 && received != TEST_SPI_DATA) {
    delay(5000);
    Serial.println("SPI communication looks active.");
  } else {
    delay(5000);
    Serial.println("SPI communication failed. Check 'Unexpected' wiring Issue.");
    delay(3000);
    Serial.println("Diagnose completed, Press Reset Button to Re-Diagnose.");
    Serial.println("======================================================");
    delay(5000);
    Serial.println("Transmitting is starting in 5 second .....");
    Serial.println("5");
    delay(1000);
    Serial.println("4");
    delay(1000);
    Serial.println("3");
    delay(1000);
    Serial.println("2");
    delay(1000);
    Serial.println("1");
    Serial.println("");
    delay(1000);

  }
}
