void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>

// I2C-Pins für den ESP32-C3 Super Mini
#define I2C_SDA 8
#define I2C_SCL 9

// Display-Größe (128x64 Pixel)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// BME280 Sensor
Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);

  // Custom I2C-Pins initialisieren
  Wire.begin(I2C_SDA, I2C_SCL);

  // Display-Start (I2C-Adresse 0x3C)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED nicht gefunden!"));
    for(;;);
  }

  // BME280-Start (I2C-Adresse 0x76 oder 0x77)
  if (!bme.begin(0x76, &Wire)) {
    if (!bme.begin(0x77, &Wire)) {
      Serial.println("BME280 nicht gefunden!");
      for(;;);
    }
  }

  // Startbildschirm
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(20, 25);
  display.println("Wetterstation");
  display.display();
  delay(1500);
}

void loop() {
  // Sensordaten auslesen
  float temp = bme.readTemperature();
  float hum = bme.readHumidity();
  float press = bme.readPressure() / 100.0F; // hPa

  display.clearDisplay();

  // -------------------------------------------------------------
  // 1. HEADER: Platzhalter für Datum & Uhrzeit (Textgröße 1)
  // -------------------------------------------------------------
  display.setTextSize(1);
  display.setCursor(16, 0); // Mittig ausgerichtet
  
  // Sobald du WLAN/NTP oder ein RTC-Modul einbaust, schreibst du hier Zeit & Datum hin:
  display.print("12:45 | 08.10."); 
  
  // Trennlinie unter dem Header
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  // -------------------------------------------------------------
  // 2. MITTE: Große Temperaturanzeige (Textgröße 3)
  // -------------------------------------------------------------
  display.setTextSize(3);
  
  // Dynamische X-Positionierung, damit der Text mittig bleibt
  if (temp < 10.0 && temp >= 0.0) {
    display.setCursor(18, 18); // Z.B. "5.2 C"
  } else if (temp < 0.0) {
    display.setCursor(2, 18);  // Z.B. "-2.5 C"
  } else {
    display.setCursor(8, 18);  // Z.B. "23.4 C" (Standard)
  }

  display.print(temp, 1); // 1 Nachkommastelle
  
  // Grad-Symbol zeichnen (kleiner Kreis)
  int16_t xPos = display.getCursorX();
  int16_t yPos = display.getCursorY();
  display.drawCircle(xPos + 3, yPos + 3, 2, SSD1306_WHITE); 
  
  // 'C' für Celsius
  display.setCursor(xPos + 8, yPos);
  display.print("C");

  // -------------------------------------------------------------
  // 3. FUSSZEILE: Luftfeuchtigkeit & Druck (Textgröße 1)
  // -------------------------------------------------------------
  display.setTextSize(1);
  display.setCursor(0, 52);
  display.print("H:");
  display.print((int)hum);
  display.print("%");

  display.setCursor(68, 52);
  display.print("P:");
  display.print((int)press);
  display.print("hPa");

  // Display aktualisieren
  display.display();

  delay(2000); // Alle 2 Sekunden aktualisieren
}