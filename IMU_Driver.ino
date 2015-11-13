#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>

// Assign a unique ID to the sensors
Adafruit_LSM303_Accel_Unified A   = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   M   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       B   = Adafruit_BMP085_Unified(18001);
Adafruit_L3GD20_Unified       G   = Adafruit_L3GD20_Unified(20);

void displayOutputFormat(void){
  sensor_t sensor;

  // Define sensors
  A.getSensor(&sensor);
  G.getSensor(&sensor);
  M.getSensor(&sensor);
  B.getSensor(&sensor);

  // Format output
  Serial.println(F("A,M,G,B"));
  // ("Ax, Ay, Az, Mx, My, Mz, Gx, Gy, Gz, Bp, Bt, Ba")
  // ACCEL(m/s^2):x,y,z;
  // MAG(uT):x,y,z
  // GYRO(rad/s):x,y,z;
  // BARO:press(hPa),temp(C),alt(m)

  delay(500);
}

void setup(void){
  Serial.begin(9600);
  Serial.println(F("IMU Driver")); Serial.println("");

  /* Initialise the sensors & check connections*/
  A.begin();
  M.begin();
  B.begin();
  G.begin();

  displayOutputFormat();
}

void loop(void){
  // Get a new sensor event
  sensors_event_t A_event;
  sensors_event_t M_event;
  sensors_event_t G_event;
  sensors_event_t B_event;
  
  // Ambient temperature
  float temperature;
  // Sea level pressure used to convert pressure and temperature to altitude
  float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

  // Retrieve sensor data
  A.getEvent(&A_event);
  M.getEvent(&M_event);
  G.getEvent(&G_event);

  // Display Sensor Information: Accel, Mag, Gyro
  Serial.print(A_event.acceleration.x);   Serial.print(F(","));
  Serial.print(A_event.acceleration.y);   Serial.print(F(","));
  Serial.print(A_event.acceleration.z);   Serial.print(F(","));
  Serial.print(M_event.magnetic.x);       Serial.print(F(","));
  Serial.print(M_event.magnetic.y);       Serial.print(F(","));
  Serial.print(M_event.magnetic.z);       Serial.print(F(","));
  Serial.print(G_event.gyro.x);           Serial.print(F(","));
  Serial.print(G_event.gyro.y);           Serial.print(F(","));
  Serial.print(G_event.gyro.z);           Serial.print(F(","));

  // Display Sensor Information: Pressure, Temp, Alt
  B.getEvent(&B_event);
  if (B_event.pressure){
    Serial.print(B_event.pressure);       Serial.print(F(","));
    B.getTemperature(&temperature);
    Serial.print(temperature);              Serial.print(F(","));
    Serial.print(B.pressureToAltitude(seaLevelPressure,
                                        B_event.pressure,
                                        temperature));
  Serial.println(F(""));
  delay(1000);
  }
}

