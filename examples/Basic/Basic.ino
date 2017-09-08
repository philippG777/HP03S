#include "HP03S.h"

/* This example uses an esp8266. If You want to use another board,
 keep in mind that this sensor requires 3.3V.
 Connections:
 Connect the I2C bus and power the right way.
 Make sure that You connect the master-clock-pin(MCLK) to a PWN-pin.
 Connect the XCLR to a digital-pin.
*/

// XCLR       MCLK
//   |______    |
//          |   |
HP03S hp03s(0, 13);

void setup()
{
  hp03s.begin();
  Serial.begin(115200); // feel free to use another baud-rate
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
