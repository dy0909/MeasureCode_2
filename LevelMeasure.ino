// Bluetooth module
#include <ArduinoBLE.h>
// Absolute pressure range
#include <Arduino_LPS22HB.h>
// Temperature & Humidity
#include <Arduino_HS300x.h>

BLEService MeasureService("19B10000-E8F2-537E-4F6C-D104768A1214");
//BLEStringCharacteristic MeasureChar("19B10000-E8F2-537E-4F6C-D104768A1214", BLEWrite | BLERead | BLENotify, 10);
BLECharCharacteristic MeasureChar_1("19B10000-E8F2-537E-4F6C-D104768A1214", BLEWrite | BLERead | BLENotify);
BLECharCharacteristic MeasureChar_2("19B10000-E8F2-537E-4F6C-D104768A1214", BLEWrite | BLERead |BLENotify);
BLECharCharacteristic MeasureChar_3("19B10000-E8F2-537E-4F6C-D104768A1214", BLEWrite | BLERead |BLENotify);

void setup()
{
    Serial.begin(115200);
    // Temperature & Humidity sensor
    if(!HS300x.begin()) Serial.println("HS300x Fail");
    // Absolute pressure sensor
    if(!BARO.begin()) Serial.println("LPS22HB Fail");
    // BEL module
    if(!BLE.begin()) Serial.println("BLE Fail");

    // Set local name
    BLE.setLocalName("Measure");
    // Set UUID for service
    BLE.setAdvertisedService(MeasureService);

    // Add characteristic to service
    MeasureService.addCharacteristic(MeasureChar_1);
    MeasureService.addCharacteristic(MeasureChar_2);
    MeasureService.addCharacteristic(MeasureChar_3);

    // Add service
    BLE.addService(MeasureService);

    MeasureChar_1.writeValue(0);
    MeasureChar_2.writeValue(0);
    MeasureChar_3.writeValue(0);

    // Start Advertising
    BLE.advertise();
}

void loop()
{
    // HS3200x
    int temperature = 0;
    int humidity = 0;
	temperature = HS300x.readTemperature();
    humidity = HS300x.readHumidity();

    Serial.print("Temperature : ");
    Serial.println(temperature);

    Serial.print("Humidity : ");
    Serial.println(humidity);

    // LPS22HB
    int pressure = 0;
	pressure = BARO.readPressure();

    Serial.print("Pressure : ");
    Serial.println(pressure);

    /*
    String measureText = String(temperature,1);//  sprintf("%f,%f,%f\n", temperature, humidity, pressure);
    measureText += ',';
    measureText += String(humidity,1);
    measureText += ',';
    measureText += String(pressure,1);
    */

    // byte byteArray[30] = {0,};
    // measureText.getBytes(byteArray, sizeof(byteArray));
    //strcpy((char *)byteArray, measureText);

    // BLE loop
    BLE.poll();

    MeasureChar_1.writeValue(temperature);
    MeasureChar_2.writeValue(humidity);
    MeasureChar_3.writeValue(pressure);


    if(MeasureChar_1.written())
    {
        Serial.print("BLE Sended Temp");
        Serial.println(temperature);
    }
    if(MeasureChar_2.written())
    {
        Serial.print("BLE Sended Humid");
        Serial.println(humidity);
    }
    if(MeasureChar_3.written())
    {
        Serial.print("BLE Sended Press");
        Serial.println(pressure);
    }

    delay(500);
}
