# Schaltplan und Verkabelung

Diese Dokumentation beschreibt die Verkabelung des R-UCO Controllers.

## Stromversorgung

| Komponente | Anschluss | Beschreibung |
| :--- | :--- | :--- |
| 12V DC Eingang | Hohlstecker 5.5/2.1mm | Hauptstromversorgung |
| GND | Gemeinsame Masse | Alle GND-Pins verbinden |

## Sensoranschlüsse

### DHT22 (Temperatur & Luftfeuchtigkeit)
- **VCC** → 5V
- **DATA** → D2 (mit 10kΩ Pull-Up Widerstand)
- **GND** → GND

### PT100 (via MAX31865)
- **CS** → D10
- **MOSI** → D11
- **MISO** → D12
- **CLK** → D13
- **VIN** → 3.3V
- **GND** → GND

## Aktoranschlüsse (SSR)

| SSR | Arduino Pin | Funktion |
| :--- | :--- | :--- |
| SSR 1 | D4 | Heizung |
| SSR 2 | D5 | Kühlung |
| SSR 3 | D6 | Befeuchtung |
| SSR 4 | D7 | Beleuchtung |

## OLED Display (SSD1306)

- **SDA** → A4
- **SCL** → A5
- **VCC** → 3.3V oder 5V
- **GND** → GND

## RS-485 Modul (MAX485)

- **DI** → D11 (MOSI)
- **RO** → D12 (MISO)
- **DE/RE** → D8 (Steuerung)
- **A/B** → Externe RS-485 Busleitung
