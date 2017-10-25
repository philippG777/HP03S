# HP03S
An Arduino-library for the HP03S sensor.

**I'm not responsible for anything You do with this library. No warranty is provided.**

## Support
This library supports Arduino UNO, ESP8266 and propably some other Atmel-controllers.
It's tested on the ESP8266


## API
### Init
```C++
HP03S hp03s(XCLR, MCLK);

void setup()
{
  hp03s.begin();
}
```
