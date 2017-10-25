#include "HP03S.h"

/* This example uses an Arduino UNO.
 Make sure that You use an logic-level-converter, because the sensor
 works with 3.3V.

 Connections:
 Arduino            SENSOR
 =======            ======
 GND ---------------- GND (VSS)
 3.3V --------------- VCC (VDD)
 9 ------- LLC ------ MCLK
 5 ------- LLC ------ XCLR
 A4 ------ LLC ------ SDA
 A5 ------ LLC ------ SCL

  LLC stands for Logic-Level-Converter

*/

// XCLR      MCLK
//   |______   |
//          |  |
HP03S hp03s(5, 9);

void setup()
{
  hp03s.begin();
  Serial.begin(9600); // feel free to use another baud-rate
}


void loop()
{
  hp03s.measure(); // You've to call this method every time You want to get new values.
  Serial.print("Temperature: ");
  Serial.print(hp03s.getTemperature());
  Serial.println("°C");

  Serial.print("Pressure: ");
  Serial.print(hp03s.getPressure());
  Serial.println("hPa");

  Serial.print("Pressure at sea-level: ");
  Serial.print(hp03s.getPressureAtSeaLevel(300)); // the argument is the height of Your location
  Serial.println("hPa");
  delay(5000);
}
