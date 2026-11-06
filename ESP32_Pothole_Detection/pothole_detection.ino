#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

Adafruit_MPU6050 mpu;
TinyGPSPlus gps;

HardwareSerial gpsSerial(1);

#define GPS_RX 16
#define GPS_TX 17

float threshold = 15.0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  if (!mpu.begin()) {
    Serial.println("MPU6050 not detected!");
    while (1) delay(10);
  }

  Serial.println("Smart Road Pothole Detection Started");
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float acceleration =
    sqrt(a.acceleration.x * a.acceleration.x +
         a.acceleration.y * a.acceleration.y +
         a.acceleration.z * a.acceleration.z);

  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  Serial.print("Acceleration: ");
  Serial.println(acceleration);

  if (acceleration > threshold) {
    Serial.println("Potential road impact detected!");

    if (gps.location.isValid()) {
      Serial.print("Latitude: ");
      Serial.println(gps.location.lat(), 6);

      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);
    } else {
      Serial.println("Waiting for GPS location...");
    }
  }

  delay(200);
}