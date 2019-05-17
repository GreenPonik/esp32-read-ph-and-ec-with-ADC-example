#include "Arduino.h"
#include "Adafruit_ADS1015.h"
#include "DFRobot_ESP_EC.h"

DFRobot_ESP_EC ec;
Adafruit_ADS1115 ads;

float voltage, ecValue, temperature = 25;

void setup()
{
	Serial.begin(115200);
	ec.begin();
	ads.setGain(GAIN_ONE);
	ads.begin();
}

void loop()
{
	static unsigned long timepoint = millis();
	if (millis() - timepoint > 1000U) //time interval: 1s
	{
		timepoint = millis();
		voltage = ads.readADC_SingleEnded(0)/10;
		Serial.print("voltage:");
		Serial.println(voltage, 4);
		
		//temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
		Serial.print("temperature:");
		Serial.print(temperature, 1);
		Serial.println("^C");

		ecValue = ec.readEC(voltage, temperature); // convert voltage to EC with temperature compensation
		Serial.print("EC:");
		Serial.print(ecValue, 4);
		Serial.println("ms/cm");
	}
	ec.calibration(voltage, temperature); // calibration process by Serail CMD
}

float readTemperature()
{
	//add your code here to get the temperature from your temperature sensor
}
