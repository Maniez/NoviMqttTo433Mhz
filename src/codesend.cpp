/**
 * Based on https://github.com/ninjablocks/433Utils
 *
 * PIN Schemas : https://projects.drogon.net/raspberry-pi/wiringpi/pins/
 */
#include "codesend.hpp"

NovyControl::NovyControl() {

}

void NovyControl::init(int Pin) {

  options.PIN = Pin;
  options.PULSE_LENGTH = 370;
  options.CODE[OnOff] =           (char*) "1011001011001011001011001011001001001011001011011001001";
  options.CODE[Fan_up] =          (char*) "1011001011001011001011001011001011001";
  options.CODE[Fan_down] =        (char*) "1011001011001011001011001011001001011";
  options.CODE[Ambientlight_on] = (char*) "1011001011001011001011001011001001011001001001001001011";
  options.CODE[Ambientlight_off] =(char*) "1011001011001011001011001011001001011001001001001011011";
  options.CODE[Light_on] =        (char*) "1011001011001011001011001011001001011001001011001001011";
  options.CODE[Light_off] =       (char*) "1011001011001011001011001011001001011001001011001011011";

  mySwitch.enableTransmit(options.PIN);
  mySwitch.setPulseLength(options.PULSE_LENGTH);

}

int NovyControl::codesend(int command) {

  //Send the code
  mySwitch.send(options.CODE[command]);
  
  return true;

}
