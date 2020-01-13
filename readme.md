# Only compatible with ESP32
# IMPORTANT : to make it work, you will need the help of an additionnal ADC converter because the one on the ESP32 isn't accurate enough. Here we used an ADS1115 from Adafruit
>You can find it here : https://www.adafruit.com/product/1085
>
>And here is the library you'll need to add to your sketch : https://github.com/GreenPonik/Adafruit_ADS1X15 

# example how to use ESP EC read and ESP PH read libraries both simultaneous
>[DFRobot ESP EC BY GREENPONIK](https://github.com/GreenPonik/DFRobot_ESP_EC_BY_GREENPONIK)

>[DFRobot ESP PH WITH ADC BY GREENPONIK](https://github.com/GreenPonik/DFRobot_ESP_PH_WITH_ADC_BY_GREENPONIK)

## #1 clone the repo
> git clone https://github.com/GreenPonik/esp32-read-ph-and-ec-with-ADC-example.git
> 
> cd esp32-read-ph-and-ec-with-ADC-example

## #2 run platformio build and upload on esp32
> pio run -t upload

## #3 debug through serial monitor
> pio device monitor

# EC Tutorial here
[![Tutorial here](http://img.youtube.com/vi/n1EBzMDPI74/0.jpg)](https://www.youtube.com/watch?v=n1EBzMDPI74 "EC Meter with ESP32 and DFRobot EC module (DFR0300)")

# PH Tutorial here
[![Tutorial here](http://img.youtube.com/vi/EqFw561pO5k/0.jpg)](https://www.youtube.com/watch?v=EqFw561pO5k "PH Meter with ESP32 and DFRobot PH module (SEN0161-V2)")

### Suscribe on our newsletter here : 
en: http://bit.ly/2NuaKbN

fr: http://bit.ly/2XNf61R

https://www.greenponik.com