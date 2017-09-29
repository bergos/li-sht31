#include <LinkedIoTPushSensor.h>
#include <LinkedIoTRadioRFM69.h>
#include <ArduinoJson.h>

#include <Adafruit_SHT31.h>

#define CLIENT_ADDRESS 3
#define SERVER_ADDRESS 1

LinkedIoTRadioRFM69 radio(CLIENT_ADDRESS, SERVER_ADDRESS);
LinkedIoTPushSensor device(radio);

Adafruit_SHT31 sht31;

void setup() {
  Serial.begin(115200);

  while (!Serial) {
    delay(1);
  }

  Serial.print("init device...");
  if (!device.begin()) {
    Serial.println("failed");
  } else {
    Serial.println("done");
  }

  Serial.print("init SHT31...");
  if (!sht31.begin(0x44)) {
    Serial.println("failed");
  } else {
    Serial.println("done");
  }

  ////device.power.disableTimer(0);
  //device.power.disableTimer(1);
  //device.power.disableTimer(3);
  //device.power.disableTimer(4);
  //device.power.disableUSART(1);
  ////device.power.disableUSB();
}

void loop() {
  if (device.loop()) {
    float batteryVoltage;
    uint8_t batteryLevel;
  
    device.power.getBatteryStatus(&batteryVoltage, &batteryLevel);

    StaticJsonBuffer<200> jsonBuffer;
  
    JsonObject& json = jsonBuffer.createObject();
    json["batteryLevel"] = batteryLevel;
    json["temperature"] = sht31.readTemperature();
    json["humidity"] = sht31.readHumidity();
  
    json.printTo(Serial);
    Serial.println("");

    Serial.print("send message to: 0x");
    Serial.print(SERVER_ADDRESS, HEX);
    Serial.print("...");

    if (!device.push(json)) {
      Serial.println("failed");
    } else {
      Serial.println("done");
    }
  
    /*power.disableADC();
    power.disableSPI();
    sleep(5000);
    power.enableSPI();
    power.enableADC();*/
    delay(1000);
  }     
}

