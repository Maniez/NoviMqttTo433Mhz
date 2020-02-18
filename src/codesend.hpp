#include "RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>

class NovyControl {
  public:
    NovyControl();
  
    void init(int Pin);
    int codesend(int command);

    const int OnOff = 0;
    const int Fan_up = 1;
    const int Fan_down = 2;
    const int Ambientlight_on = 3;
    const int Ambientlight_off = 4;
    const int Light_on = 5;
    const int Light_off = 6;
    
  private:

    RCSwitch mySwitch;;

    struct Options {
        int PIN;
        int PULSE_LENGTH;
        char* CODE[7];
    } options;
};