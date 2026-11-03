# ardn-test-plate
Der **R-UCO Controller** ist ein kompakter, universeller Mikrocontroller-basierter Regler, der speziell für den Einsatz in Umgebungs- und Klimakammern sowie für die Steuerung von Laborinkubatoren entwickelt wurde.

Dieses Repository enthält die Firmware, die Hardware-Spezifikationen und Beispielskripte für die Inbetriebnahme.

![R-UCO Board](https://via.placeholder.com/800x400?text=R-UCO+Controller+Rev+2.1)

## 🚀 Features

*   **Mikrocontroller:** ATmega328P (Arduino Nano kompatibel)
*   **Schnittstellen:** I2C, SPI, UART (RS-485 Modbus RTU Support)
*   **Sensoreingänge:** 
    *   2x DHT22 (Temperatur & Luftfeuchtigkeit)
    *   1x PT100 (Präzisionstemperatur über MAX31865)
    *   2x Analogeingänge (0-10V für Industriesensoren)
*   **Ausgänge:**
    *   4x Solid-State-Relais (SSR) Ausgänge (bis 2A)
    *   1x PWM-Ausgang für Lüftersteuerung
*   **Display:** 0.96" OLED (I2C) Support für lokale Statusanzeige
*   **Stromversorgung:** 12V DC (Hohlstecker) mit integriertem 5V LDO-Regler

## 📦 Pinbelegung (Pinout)

| Pin (Nano) | Funktion | Beschreibung |
| :--- | :--- | :--- |
| D2 | OneWire | Datenleitung für DS18B20 / PT100 (via MAX31865) |
| D3 | PWM | Lüftersteuerung (Ausgang) |
| D4 | Digital Out | SSR 1 (Heizung) |
| D5 | Digital Out | SSR 2 (Kühlung) |
| D6 | Digital Out | SSR 3 (Befeuchtung) |
| D7 | Digital Out | SSR 4 (Beleuchtung) |
| A4 (SDA) | I2C | Display (OLED) |
| A5 (SCL) | I2C | Display (OLED) |
| D10, D11, D12 | SPI | RS-485 Modul (z.B. MAX485) |

## 🛠 Installation & Inbetriebnahme

1.  **Hardware vorbereiten:** Stelle sicher, dass die Stromversorgung (12V) korrekt gepolt anliegt. Die 5V-Logik wird über den onboard-Regler versorgt.
2.  **Firmware flashen:** Verbinde die Platine über ein USB-TTL-Kabel (FTDI) mit deinem Computer.
3.  **Arduino IDE öffnen:** Wähle das Board `Arduino Nano` und den entsprechenden Prozessor (`ATmega328P (Old Bootloader)` bei älteren Clones).
4.  **Bibliotheken installieren:** Du benötigst folgende Bibliotheken (via Library Manager):
    *   `Adafruit SSD1306` & `Adafruit GFX`
    *   `DHT sensor library` (Adafruit)
    *   `ModbusMaster`

## 💻 Beispielcode (Auszug)

Hier ist ein kurzer Auszug aus dem `main.ino`, der die Initialisierung der Sensoren und die grundlegende Regelung zeigt:

```cpp
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define SSR_HEAT 4
#define SSR_COOL 5

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Display initialisieren
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  pinMode(SSR_HEAT, OUTPUT);
  pinMode(SSR_COOL, OUTPUT);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("R-UCO Controller");
  display.println("System bereit...");
  display.display();
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  
  if (isnan(t) || isnan(h)) {
    Serial.println("Fehler beim Lesen des DHT-Sensors!");
    return;
  }
  
  // Einfache Hysterese-Regelung
  if (t < 22.0) {
    digitalWrite(SSR_HEAT, HIGH); // Heizen ein
  } else if (t > 24.0) {
    digitalWrite(SSR_HEAT, LOW);  // Heizen aus
  }
  
  delay(2000);
}
