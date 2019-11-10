#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp; // I2C
float newZero = 0;
float pres = 0;
float mini = 9999999999;
float maxi = -999999999;
float sum = 0;
float altMax = 0;
float alt = 0;
float firstAlt = 0;
float prevAlt = 0;
bool desc = false;
unsigned long timeData;
unsigned long timeApog;


void setup() {

  tone (8, 1000, 200);
  Serial.begin(9600);
  Serial.println(F("BMP280 test"));
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  /*
     two registers are used :

      0xF4 consists of:

    3 bits osrs_t (measure temperature 0, 1, 2, 4, 8 or 16 times);
    3 bits osrs_p (measure pressure 0, 1, 2, 4, 8 or 16 times); and
    2 bits Mode (Sleep, Forced (ie Single Shot), Normal (ie continuous).

    0xF5 consists of:

    3 bits t_sb (standby time, 0.5ms to 4000 ms);
    3 bits filter (see below); and
    1 bit spiw_en which selects SPI = 1 or I2C = 0.
  */

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode.       */
                  Adafruit_BMP280::SAMPLING_NONE,   /* Temp. oversampling    */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,       /* Filtering.            */
                  Adafruit_BMP280::STANDBY_MS_1);   /* Standby time.         */
  delay(2000);
  for (int i = 0; i <= 9; i++) {
  newZero = bmp.readPressure() + newZero;
  }
  newZero = newZero / 1000;
}





void loop() {
  for (int i = 0; i <= 9; i++) {
    pres = bmp.readPressure();
    sum = pres + sum;
    if (pres < mini) {
      mini = pres;
    }
    if (pres > maxi) {
      maxi = pres;
    }
  }
  pres = (sum - (mini + maxi)) / 800;
  alt = 44330 * (1.0 - pow(pres / newZero, 0.1903));
  if (alt > altMax) {
    altMax = alt;
    timeApog = millis();
  }
  if ((alt - prevAlt < 0) && (alt - firstAlt < 0) && (alt > 1.20))
  {
    desc = true;
    tone(8, 1000, 100);
  }

  Serial.println(alt * 100);
  Serial.print(" ");
    Serial.println(altMax*100);
  Serial.print(" ");

 
  pres = 0;
  mini = 9999999999;
  maxi = -999999999;
  sum = 0;
  firstAlt = prevAlt;
  prevAlt = alt;


}
