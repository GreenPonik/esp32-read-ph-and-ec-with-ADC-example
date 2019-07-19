#include "Arduino.h"

#include "DFRobot_ESP_EC.h"			 //https://github.com/GreenPonik/DFRobot_ESP_EC_BY_GREENPONIK.git
#include "DFRobot_ESP_PH_WITH_ADC.h" //https://github.com/GreenPonik/DFRobot_ESP_PH_WITH_ADC_BY_GREENPONIK.git
#include "Adafruit_ADS1015.h"		 //https://github.com/GreenPonik/Adafruit_ADS1X15.git

#include "OneWire.h"
#include "DallasTemperature.h"
#include "EEPROM.h"

#define ONE_WIRE_BUS 15
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DFRobot_ESP_PH_WITH_ADC ph;
DFRobot_ESP_EC ec;
Adafruit_ADS1115 ads;

unsigned long intervals[] = {
	1000U,	//0
	2000U,	//1
	3000U,	//2
	5000U,	//3
	10000U,   //4
	15000U,   //5
	20000U,   //6
	25000U,   //7
	60000U,   //8
	1800000U, //9
};			  //this defines the interval for each task in milliseconds
unsigned long last[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
bool calibrationIsRunning = false;

float voltage, ecValue, temperature = 25;

float lastTemperature;
float getWaterTemperature()
{
	sensors.requestTemperatures(); // Send the command to get temperatures
	float temperature = sensors.getTempCByIndex(0);

	if (temperature == 85.00 || temperature == -127.00) //take the last correct temperature value if getting 85 or -127 value
	{
		temperature = lastTemperature;
	}
	else
	{
		lastTemperature = temperature;
	}

	Serial.println("[getWaterTemperature]... temperature : " + String(temperature));
	return temperature;
}

/**
* Read Serial to launch PH or EC listener
*/
int i = 0;
bool readSerial(char result[])
{
	while (Serial.available() > 0)
	{
		char inChar = Serial.read();
		if (inChar == '\n')
		{
			result[i] = '\0';
			Serial.flush();
			i = 0;
			return true;
		}
		if (inChar != '\r')
		{
			result[i] = inChar;
			i++;
		}
		delay(1);
	}
	return false;
}

void setup()
{
	Serial.begin(115200);
	EEPROM.begin(64);
	ads.setGain(GAIN_ONE);
	ads.begin();
	ph.begin(10); //we store 2 value from the start address (10 & 14)
	ec.begin(20); //we store 2 value from the start address (20 & 24)
}

void loop()
{
	float ecVoltage, ecValue, phVoltage, phValue, temperature = 25;
	unsigned long now = millis();
	if (now - last[0] >= intervals[0]) //1000ms interval
	{
		last[0] = now;
		if (calibrationIsRunning)
		{
			Serial.println(F("[main]...>>>>>> calibration is running, to exit send exitph or exitec through serial <<<<<<"));
			//water temperature
			temperature = getWaterTemperature();
			//EC
			ecVoltage = ads.readADC_SingleEnded(0) / 10;
			Serial.print(F("[EC Voltage]... ecVoltage: "));
			Serial.println(ecVoltage);
			ecValue = ec.readEC(ecVoltage, temperature); // convert voltage to EC with temperature compensation
			Serial.print(F("[EC Read]... EC: "));
			Serial.print(ecValue);
			Serial.println(F("ms/cm"));
			//pH
			phVoltage = ads.readADC_SingleEnded(1) / 10;
			Serial.print(F("[pH Voltage]... phVoltage: "));
			Serial.println(phVoltage);
			phValue = ph.readPH(phVoltage, temperature);
			Serial.print(F("[pH Read]... pH: "));
			Serial.println(phValue);
		}

		char cmd[10];
		if (readSerial(cmd))
		{
			strupr(cmd);
			if (calibrationIsRunning || strstr(cmd, "PH") || strstr(cmd, "EC"))
			{
				calibrationIsRunning = true;
				Serial.println(F("[]... >>>>>calibration is now running PH and EC are both reading, if you want to stop this process enter EXITPH or EXITEC in Serial Monitor<<<<<"));
				if (strstr(cmd, "PH"))
				{
					ph.calibration(phVoltage, temperature, cmd); //PH calibration process by Serail CMD
				}
				if (strstr(cmd, "EC"))
				{
					ec.calibration(ecVoltage, temperature, cmd); //EC calibration process by Serail CMD
				}
			}
			if (strstr(cmd, "EXITPH") || strstr(cmd, "EXITEC"))
			{
				calibrationIsRunning = false;
			}
		}
	}
	if (now - last[3] >= intervals[3]) //5000ms interval
	{
		if (!calibrationIsRunning)
		{
			temperature = getWaterTemperature(); // read your temperature sensor to execute temperature compensation
			Serial.print("temperature:");
			Serial.print(temperature, 1);
			Serial.println("^C");

			ecVoltage = ads.readADC_SingleEnded(0) / 10;
			Serial.print("ecVoltage:");
			Serial.println(ecVoltage, 4);

			ecValue = ec.readEC(ecVoltage, temperature); // convert voltage to EC with temperature compensation
			Serial.print("EC:");
			Serial.print(ecValue, 4);
			Serial.println("ms/cm");

			phVoltage = ads.readADC_SingleEnded(1) / 10; // read the voltage
			Serial.print("phVoltage:");
			Serial.println(phVoltage, 4);
			phValue = ph.readPH(phVoltage, temperature); // convert voltage to pH with temperature compensation
			Serial.print("pH:");
			Serial.println(phValue, 4);
		}
	}
}
