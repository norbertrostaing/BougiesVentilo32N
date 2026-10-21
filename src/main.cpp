#include <Arduino.h>

#include "Olgalimex/_config.h"
#include "Olgalimex/_ethernet.h"
#include "Olgalimex/_wifi.h"
#include "Olgalimex/_dmx.h"
#include "Olgalimex/_artnet.h"
#include "Olgalimex/_osc.h"
#include "Olgalimex/_web.h"

int pinMosfet = 4;
bool isRunning = false;

float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
    const float run = in_max - in_min;
    if(run == 0) { return out_min;}
    const float rise = out_max - out_min;
    const float delta = x - in_min;
    return (delta * rise) / run + out_min;
}

void onArtnetFrame(uint16_t universeRcv, uint16_t length, uint8_t sequence, uint8_t *data, IPAddress remoteIP)
{
}

void onDmxFrame()
{
}


void triggerTriggered(String str) {
  if (str == "start") { 
    analogWrite(pinMosfet, config["power"].as<int>());
    isRunning = true;
  } else if (str == "stop") {
    analogWrite(pinMosfet, 0);
    isRunning = false;
  }
}


void configUpdatedMain(String key)
{
  if (key == "speed") {
    int power = config["power"].as<int>();
    analogWrite(pinMosfet, power);
    isRunning = power > 0;
  }
}

void setup() {
	// generic parameters
	oscPort = 9004;
	chipName = "VentilosFlame 32N";
	eepromMarker = 0x56;
	// config : read and write parameters
	// infos : read only parameters

  addIntConfig("power", 1024);
  addTrigger("start");
  addTrigger("stop");

	Serial.begin(115200);
	initConfig();

  pinMode(pinMosfet, OUTPUT);
  
	//setupEthernet();
	setupWifi();
	setupOSC();
	setupWebServer();
	//setupArtnet();
	receivePin = 36;
	//setupDMX();

  oscSubscribe("/power", [](const OscMessage& m) { if (m.size()>0) { analogWrite(pinMosfet, m.arg<float>(0)); } });

}


void loop()
{
	vTaskDelay(5);
}
