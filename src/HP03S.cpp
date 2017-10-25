#include "HP03S.h"

#include <Arduino.h>
#include <Wire.h>


HP03S::HP03S(int xclr, int mclk)
{
    _xclr = xclr;
    _mclk = mclk;
}


void HP03S::begin()
{
    // init pins
    pinMode(_xclr, OUTPUT);
    pinMode(_mclk, OUTPUT);
#if defined(ESP8266)
    analogWriteFreq(32768); // set freq of pwm to 32.768 kHz
#endif

    _setMasterClock(false);
    _getParameters();
}


void HP03S::printParameters()
{
   Serial.print("c1: ");
    Serial.println(String(_c1));
    Serial.print("c2: ");
    Serial.println(String(_c2));
    Serial.print("c3: ");
    Serial.println(String(_c3));
    Serial.print("c4: ");
    Serial.println(String(_c4));
    Serial.print("c5: ");
    Serial.println(String(_c5));
    Serial.print("c6: ");
    Serial.println(String(_c6));
    Serial.print("c7: ");
    Serial.println(String(_c7));

    Serial.print("a: ");
    Serial.println(String(_a));
    Serial.print("b: ");
    Serial.println(String(_b));
    Serial.print("c: ");
    Serial.println(String(_c));
    Serial.print("d: ");
    Serial.println(String(_d)); 
}


void HP03S::printAllValues()
{
    Serial.println("\nStatic parameters:\n");
    printParameters();
    Serial.println("\nOther values:\n");
    Serial.print("dut: ");
    Serial.println(_dut);
    Serial.print("D1: ");
    Serial.println(_d1);
    Serial.print("D2: ");
    Serial.println(_d2);  
}


double HP03S::getPressure()
{
    return _calcPressure();
}


double HP03S::getPressureAtSeaLevel(double height)
{
    double gardient = 0.0065;
    double tempAtSea = 15.0;
    double p = getPressure();
    tempAtSea += 273.15;  // °C to K
    return p / pow((1 - gardient * height / tempAtSea), (0.03416 / gardient));
}


double HP03S::getTemperature()
{
    return _calcTemperature();
}


void HP03S::measureTemperature()
{
    uint8_t buf[2];
    _setMasterClock(true);

    Wire.beginTransmission(0xee >> 1);
    Wire.write(0xff);
    Wire.write(0xe8);
    Wire.endTransmission();
    delay(50);

    Wire.beginTransmission(0xee >> 1);
    Wire.write(0xfd);
    Wire.endTransmission();

    Wire.beginTransmission(0xee >> 1);
    Wire.requestFrom(0xee >> 1, 2);
    for (uint8_t i = 0; i < 2; i++)
    {
        buf[i] = Wire.read();
    }
    Wire.endTransmission();

    _d2 = word(buf[0], buf[1]);
    _setMasterClock(false);   
}


void HP03S::measurePressure()
{
    uint8_t buf[2];
    _setMasterClock(true);

    Wire.beginTransmission(0xee >> 1);
    Wire.write(0xff);
    Wire.write(0xf0);
    Wire.endTransmission();
    delay(50);

    Wire.beginTransmission(0xee >> 1);
    Wire.write(0xfd);
    Wire.endTransmission();

    Wire.beginTransmission(0xee >> 1);
    Wire.requestFrom(0xee >> 1, 2);
    for (uint8_t i = 0; i < 2; i++)
    {
        buf[i] = Wire.read();
    }
    Wire.endTransmission();

    _d1 = word(buf[0], buf[1]);
    _setMasterClock(false);  
}


void HP03S::measure()
{
    measurePressure();
    measureTemperature();
    _dut = _calcDUT();
}


double HP03S::_calcPressure()
{
    long off = (long(_c2) + (long(_c4) - 1024) * _dut / 16384) * 4;
    long sens = long(_c1) + long(_c3) * _dut / 1024;
    long x = sens * (long(_d1) - 7168) / 16384 - off;

#ifdef HP03S_H_DEBUG
    Serial.print("OFF: ");
    Serial.println(off);
    Serial.print("SENS: ");
    Serial.println(sens);
    Serial.print("X: ");
    Serial.println(x);
#endif
    return (x * 10 / 32 + long(_c7)) / 10;
}
    

double HP03S::_calcTemperature()
{
    return (250 + _dut * long(_c6) / 65536 - _dut / pow(2, long(_d))) / 10;
}


long HP03S::_calcDUT()
{
    if (_d2 >= _c5)
    {
        return long(_d2) - long(_c5) - ((long(_d2) - long(_c5)) / 128) * ((long(_d2) - long(_c5)) / 128) * long(_a) / pow(2, long(_c));
    }
    else
    {
        return long(_d2) - long(_c5) - ((long(_d2) - long(_c5)) / 128) * ((long(_d2) - long(_c5)) / 128) * long(_b) / pow(2, long(_c));
    }   
}


void HP03S::_setMasterClock(bool state)
{
    if (state)
    {
#if defined(ESP8266)
        analogWrite(_mclk, 127);
#else
        tone(_mclk, 32768);  // set freq of pwm to 32.768 kHz
#endif
        digitalWrite(_xclr, HIGH);
    }
    else
    {
#if defined(ESP8266)
        analogWrite(_mclk, 0);
#else
        noTone(_mclk);
#endif
        digitalWrite(_xclr, LOW);
    } 
}


void HP03S::_getParameters()
{
    byte buf[18];  // buffer for I2C response
    _setMasterClock(true);

    Wire.begin();
    delay(50);
    Wire.beginTransmission(0x50);
    Wire.write(0x10);
    Wire.endTransmission();
    delay(50);  // has to be more than 40ms

    Wire.beginTransmission(0x50);
    Wire.requestFrom(0x50, 18);
    for (uint8_t i = 0; i < 18; i++) // read 18 bytes
    {
        buf[i] = Wire.read();
    }
    Wire.endTransmission();

    _setMasterClock(false);

    // set parameters
    _c1 = word(buf[0], buf[1]);
    _c2 = word(buf[2], buf[3]);
    _c3 = word(buf[4], buf[5]);
    _c4 = word(buf[6], buf[7]);
    _c5 = word(buf[8], buf[9]);
    _c6 = word(buf[10], buf[11]);
    _c7 = word(buf[12], buf[13]);

    _a = buf[14];
    _b = buf[15];
    _c = buf[16];
    _d = buf[17];  
}
