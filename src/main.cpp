#include <Homie.h>
#include "codesend.hpp"
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

HomieNode abzugshaubeNode("Abzug", "Abzugshaube", "switch");
NovyControl NovyAbzugshaube;

int _lastMeasurement = 0;

bool switchPowerHandler(const HomieRange& range, const String& value) {
  if(NovyAbzugshaube.codesend(NovyAbzugshaube.OnOff)){
    abzugshaubeNode.setProperty("toggle").send("Abzugshaube wurde an/aus geschaltet");
    Homie.getLogger() << "Sending on/off command" << endl;
    return true;
  } else {
    abzugshaubeNode.setProperty("toggle").send("ERROR: Abzugshaube wurde NICHT an/aus geschaltet");
    Homie.getLogger() << "Sending on/off command failed" << endl;
    return false;
  }
}

bool switchFanHandler_up(const HomieRange& range, const String& value) {
  if(value.equals("true")) {
    if(NovyAbzugshaube.codesend(NovyAbzugshaube.Fan_up)){
      abzugshaubeNode.setProperty("up").send("Fan-Level up");
      Homie.getLogger() << "Sending Fan-Level up command" << endl;
      return true;
    } else {
      abzugshaubeNode.setProperty("up").send("ERROR: Fan-Level up");
      Homie.getLogger() << "Sending Fan-Level up command failed" << endl;
      return false;
    }
  } else if(value.equals("false")) {
    return true;
  } else {
    return false;
  }
}
  
bool switchFanHandler_down(const HomieRange& range, const String& value) {
  if(value.equals("true")) {
    if(NovyAbzugshaube.codesend(NovyAbzugshaube.Fan_down)){
      abzugshaubeNode.setProperty("down").send("Fan-Level down");
      Homie.getLogger() << "Sending Fan-Level down command" << endl;
      return true;
    } else {
      abzugshaubeNode.setProperty("down").send("ERROR: Fan-Level down");
      Homie.getLogger() << "Sending Fan-Level dowm command failed" << endl;
      return false;
    }
  } else if(value.equals("false")) {
    return true;
  } else {
    return false;
  }
}

bool switchAmbientlightHandler(const HomieRange& range, const String& value) {
  if(value.equals("true")) {
    if(NovyAbzugshaube.codesend(NovyAbzugshaube.Ambientlight_on)){
      abzugshaubeNode.setProperty("up").send("Ambientlight brightness up");
      Homie.getLogger() << "Sending Ambientlight brightness up command" << endl;
      return true;
    } else {
      abzugshaubeNode.setProperty("up").send("ERROR: Ambientlight brightness up");
      Homie.getLogger() << "Sending Ambientlight brightness up command failed" << endl;
      return false;
    }
  } else if(value.equals("false")) {
    if(NovyAbzugshaube.codesend(NovyAbzugshaube.Ambientlight_off)){
      abzugshaubeNode.setProperty("down").send("Ambientlight brightness down");
      Homie.getLogger() << "Sending Ambientlight brightness down command" << endl;
      return true;
    } else {
      abzugshaubeNode.setProperty("down").send("ERROR: Ambientlight brightness down");
      Homie.getLogger() << "Sending Ambientlight brightness dowm command failed" << endl;
      return false;
    }
  } else {
    return false;
  }
}

bool switchLightHandler(const HomieRange& range, const String& value) {
  if(value.equals("true")) {
    if(NovyAbzugshaube.codesend(NovyAbzugshaube.Light_on)){
      abzugshaubeNode.setProperty("up").send("Light brightness up");
      Homie.getLogger() << "Sending Light brightness up command" << endl;
      return true;
    } else {
      abzugshaubeNode.setProperty("up").send("ERROR: Light brightness up");
      Homie.getLogger() << "Sending Light brightness up command failed" << endl;
      return false;
    }
  } else if(value.equals("false")) {
    if(NovyAbzugshaube.codesend(NovyAbzugshaube.Light_off)){
      abzugshaubeNode.setProperty("down").send("Light brightness down");
      Homie.getLogger() << "Sending Light brightness down command" << endl;
      return true;
    } else {
      abzugshaubeNode.setProperty("down").send("ERROR: Light brightness down");
      Homie.getLogger() << "Sending Light brightness dowm command failed" << endl;
      return false;
    }
  } else {
    return false;
  }
}


void loopHandler() {

  if (millis() - _lastMeasurement >= 300 * 1000UL || _lastMeasurement == 0) {
    _lastMeasurement = millis();

    Homie.getLogger() << "Sending ping 'hello': " << endl;

    abzugshaubeNode.setProperty("open").send("Hello Manu");
  }

}

void setup() {
// put your setup code here, to run once:
    Serial.begin(115200);
    Serial << endl << endl;

    Homie_setFirmware("NovyControl", "0.1.2");
    Homie.setLoopFunction(loopHandler);

    NovyAbzugshaube.init(D1);
    
    abzugshaubeNode.advertise("power").setName("Powertoggle").setRetained(true).setDatatype("boolean").settable(switchPowerHandler);
    abzugshaubeNode.advertise("fan_up").setName("Level_up").setRetained(true).setDatatype("boolean").settable(switchFanHandler_up);
    abzugshaubeNode.advertise("fan_down").setName("Level_down").setRetained(true).setDatatype("boolean").settable(switchFanHandler_down);
    abzugshaubeNode.advertise("ambientlight").setRetained(true).setName("Lighttoggle").setDatatype("boolean").settable(switchAmbientlightHandler);
    abzugshaubeNode.advertise("light").setName("Lighttoggle").setRetained(true).setDatatype("boolean").settable(switchLightHandler);  

    Homie.setup();

    ArduinoOTA.onStart([]() {
    Serial.println("Start");
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin(); 
}

void loop() {
// put your main code here, to run repeatedly:
    ArduinoOTA.handle();
    Homie.loop();
}