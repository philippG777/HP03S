#ifndef HP03S_H
#define HP03S_H

#include <Arduino.h>
#include <Wire.h>

// #define HP03S_H_DEBUG

class HP03S
{
  public:
    HP03S(int xclr, int mclk);
    void begin();

    void printParameters();
    void printAllValues();

    double getPressure();
    double getPressureAtSeaLevel(double height);
    double getTemperature();

    void measureTemperature();
    void measurePressure();
    void measure();


  private:

    int _xclr;
    int _mclk;
    word _c1;
    word _c2;
    word _c3;
    word _c4;
    word _c5;
    word _c6;
    word _c7;

    byte _a;
    byte _b;
    byte _c;
    byte _d;

    word _d1;
    word _d2;

    long _dut;


    double _calcPressure();
    double _calcTemperature();
    long _calcDUT();

    void _setMasterClock(bool state);
    void _getParameters();
};

#endif

