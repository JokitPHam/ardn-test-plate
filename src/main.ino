#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <ModbusMaster.h>
#include "config.h"

// --- Objektinstanzen ---
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
ModbusMaster modbus;

// --- Globale Variablen ---
float currentTemp = 0.0;
float currentHum = 0.0;
bool heatingActive = false;
bool coolingActive = false;

// --- Modbus Callbacks ---
void preTransmission() {
  digitalWrite(MAX485_DE, HIGH);
}

void postTransmission() {
  digitalWrite(MAX485_DE, LOW);
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("R-UCO Controller startet..."));

  // Sensoren initialisieren
  dht.begin();

  // Relais-Pins
  pinMode(SSR_HEAT, OUTPUT);
  pinMode(SSR_COOL, OUTPUT);
  pinMode(SSR_HUMID, OUTPUT);
  pinMode(SSR_LIGHT, OUTPUT);
  digitalWrite(SSR_HEAT, LOW);
  digitalWrite(SSR_COOL, LOW);
  digitalWrite(SSR_HUMID, LOW);
  digitalWrite(SSR_LIGHT, LOW);

  // Display initialisieren
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 Initialisierung fehlgeschlagen!"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("R-UCO Controller"));
  display.println(F("System bereit..."));
  display.display();

  // Modbus initialisieren
  pinMode(MAX485_DE, OUTPUT);
  digitalWrite(MAX485_DE, LOW);
  Serial1.begin(9600, SERIAL_8E1);
  modbus.begin(1, Serial1);
  modbus.preTransmission(preTransmission);
  modbus.postTransmission(postTransmission);

  delay(2000);
}

void loop() {
  // Sensorwerte lesen
  currentTemp = dht.readTemperature();
  currentHum = dht.readHumidity();

  if (isnan(currentTemp) || isnan(currentHum)) {
    Serial.println(F("Fehler beim Lesen des DHT-Sensors!"));
    return;
  }

  // Regelung (einfache Hysterese)
  if (currentTemp < TEMP_SETPOINT - HYSTERESIS) {
    digitalWrite(SSR_HEAT, HIGH);
    heatingActive = true;
  } else if (currentTemp > TEMP_SETPOINT + HYSTERESIS) {
    digitalWrite(SSR_HEAT, LOW);
    heatingActive = false;
  }

  if (currentHum > HUM_SETPOINT + HUM_HYSTERESIS) {
    digitalWrite(SSR_HUMID, HIGH);
  } else if (currentHum < HUM_SETPOINT - HUM_HYSTERESIS) {
    digitalWrite(SSR_HUMID, LOW);
  }

  // Display aktualisieren
  updateDisplay();

  // Modbus-Daten senden (alle 5 Sekunden)
  static unsigned long lastModbus = 0;
  if (millis() - lastModbus > 5000) {
    sendModbusData();
    lastModbus = millis();
  }

  delay(500);
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(F("Temp: "));
  display.print(currentTemp, 1);
  display.println(F(" C"));
  display.print(F("Hum:  "));
  display.print(currentHum, 1);
  display.println(F(" %"));
  display.println();
  display.print(F("Heizung: "));
  display.println(heatingActive ? F("AN") : F("AUS"));
  display.display();
}

void sendModbusData() {
  // Beispiel: Sende Temperatur als 16-bit Integer (x10)
  uint16_t tempRaw = (uint16_t)(currentTemp * 10);
  modbus.setTransmitBuffer(0, tempRaw);
  uint8_t result = modbus.writeMultipleRegisters(0x0000, 1);
  if (result == modbus.ku8MBSuccess) {
    Serial.println(F("Modbus Daten gesendet."));
  } else {
    Serial.println(F("Modbus Fehler beim Senden!"));
  }
}
