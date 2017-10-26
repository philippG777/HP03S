# HP03S
An Arduino-library for the HP03S sensor.

**I'm not responsible for anything You do with this library. No warranty is provided.**


## Support
This library supports Arduino UNO, ESP8266 and propably some other Atmel-controllers.
It's tested on the ESP8266.


## Quick-Start
```C++
#include "HP03S.h"

HP03S hp03s(5, 9);

void setup()
{
  hp03s.begin();
  Serial.begin(9600);
}


void loop()
{
  hp03s.measure();
  double temperature = hp03s.getTemperature();
  double pressure = hp03s.getPressure();
  double pressureAtSea = hp03s.getPressureAtSeaLevel(300);

  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.print("Pressure: ");
  Serial.prinln(pressure);

  Serial.print("Pressure at the sea-level: "):
  Serial.println(pressureAtSea);

  delay(5000);
}
```


## Installation
### Arduino Library-Manager
In the Arduino-IDE select **Sketch > Include Library > Manage Libraries**. In the library manager type **HP03S** and click the resulting library to install it. The library will be available after restarting the Arduino IDE.


### Installing manually
Download the .zip-file from the [latest release](https://github.com/philippG777/HP03S/releases/latest), place it in **<Your-Arduino-projects-folder>/libraries/** and extract it.
Create the libraries-folder if it does not exist. The library will be available after restarting the Arduino IDE.


## API Documentation
### Init
```C++
HP03S hp03s(XCLR, MCLK);

void setup()
{
  hp03s.begin();
}
```

The XCLR-pin is a digital output and the MCLK-pin has to be a PWM-pin if You are using the ESP8266.

### .measure()
The measure-method measures and stores the response from the sensor. It has no return value, it just (re)measures.

### .getTemperature()
This method returns the temperature from the last `.measure()`-call. It returns the temperature in °C as a double.

### .getPressure()
`.getPressure()` returns the absolute pressure in hPa as a double.

### .getPressureAtSeaLevel(height)
This method calculates the pressure at the sea-level, therefor it uses the height in meter as argument. The pressure at sea-level is usually used in weather-forecasts, its standart-value is 1013.25hPa.
