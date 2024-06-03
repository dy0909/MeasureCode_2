// Bluetooth module
#include <ArduinoBLE.h>
// Absolute pressure range
#include <Arduino_LPS22HB.h>
// Temperature & Humidity
#include <Arduino_HS300x.h>
#include <ArduinoBLE.h>

BLEService tempService("1809");
BLEFloatCharacteristic tempCharacteristic("2A6E", BLERead | BLENotify);

void setup()
{
    Serial.begin(115200);
    // Temperature & Humidity sensor
	if(!HS300x.begin()) Serial.println("HS300x Fail");
    // Absolute pressure sensor
    if(!BARO.begin()) Serial.println("LPS22HB Fail");

    if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE!");
    while (1);
  }

  // BLE 장치 이름과 서비스 설정
  BLE.setLocalName("Nano33BLE");
  BLE.setAdvertisedService(tempService);

  // 특성 추가
  tempService.addCharacteristic(tempCharacteristic);
  BLE.addService(tempService);

  // 광고 시작
  BLE.advertise();
}

void loop()
{
    // HS3200x
    float temperature = 0.0;
    float humidity = 0.0;
	temperature = HS300x.readTemperature();
    humidity = HS300x.readHumidity();

    Serial.print("Temperature : ");
    Serial.println(temperature);

    Serial.print("Humidity : ");
    Serial.println(humidity);

    // LPS22HB
    float pressure = 0.0;
	pressure = BARO.readPressure();

    Serial.print("Pressure : ");
    Serial.println(pressure);


    BLEDevice central = BLE.central();

    if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    // 중앙 장치가 연결된 동안 온도 측정 및 전송
    while (central.connected()) {
      Serial.print("BLE Temperature: ");
      Serial.println(temperature);
      
      // 온도 값 특성에 전송
      tempCharacteristic.writeValue(temperature);

      // 잠시 대기
      delay(1000);
    }
    }

    delay(500);
}






#include <ArduinoBLE.h>
#include <Arduino_LPS22HB.h> // Pressure sensor
#include <Arduino_HTS221.h>  // Humidity and Temperature sensor

BLEService sensorService("180C"); // Custom service

// BLE Characteristics
BLEStringCharacteristic sensorDataCharacteristic("2A56", BLERead | BLENotify, 32);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize the sensors
  if (!BARO.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
    while (1);
  }

  if (!HTS.begin()) {
    Serial.println("Failed to initialize temperature and humidity sensor!");
    while (1);
  }

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  // Set BLE device name
  BLE.setLocalName("Nano33BLE_Sense");
  BLE.setAdvertisedService(sensorService);

  // Add the characteristic to the service
  sensorService.addCharacteristic(sensorDataCharacteristic);

  // Add the service
  BLE.addService(sensorService);

  // Start advertising
  BLE.advertise();

  Serial.println("BLE device active, waiting for connections...");
}

void loop() {
  // Listen for BLE connections
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      // Read sensor values
      float temperature = HTS.readTemperature();
      float humidity = HTS.readHumidity();
      float pressure = BARO.readPressure();

      // Create a string with the sensor values separated by commas
      char sensorData[32];
      snprintf(sensorData, sizeof(sensorData), "%.2f,%.2f,%.2f", temperature, humidity, pressure);

      // Update BLE characteristic
      sensorDataCharacteristic.writeValue(sensorData);

      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C, Humidity: ");
      Serial.print(humidity);
      Serial.print(" %, Pressure: ");
      Serial.print(pressure);
      Serial.println(" hPa");

      delay(1000); // Update every second
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

