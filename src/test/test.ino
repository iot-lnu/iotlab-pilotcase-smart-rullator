#include "M5Atom.h"
#include <math.h>

// sudo chmod a+rw /dev/ttyUSB0

float acc0[] = {0.0f, 0.0f, 0.0f};

float acceleration[] = {0.0f, 0.0f, 0.0f};
float velocity[] = {0.0f , 0.0f, 0.0f};
float distance[] = {0.0f, 0.0f, 0.0f};

unsigned long previousMillis = 0;
int interval = 50;
float G = 9.82f;
int p = 0;

void setup(){
  M5.begin(true, false, true);
  delay(100);
  M5.IMU.Init();
  delay(100);
  M5.IMU.SetAccelFsr(M5.IMU.AFS_16G);
  delay(100);

  for (int i = 0; i < 100; i++) {
    float j;
    float k;
    float l;
    M5.IMU.getAccelData(&j, &k, &l);
    acc0[0] = acc0[0] + j;
    acc0[1] = acc0[1] + k;
    acc0[2] = acc0[2] + l;    
    delay(10);
  }

  acc0[0] = acc0[0] / 100;
  acc0[1] = acc0[1] / 100;
  acc0[2] = acc0[2] / 100;     
}

void loop(){
  unsigned long currentMillis = micros();
  unsigned long interval = currentMillis - previousMillis;
  // Serial.printf("\n%lu ms\n\n", interval);    
  
  M5.IMU.getAccelData(&acceleration[0], &acceleration[1], &acceleration[2]);
  if (interval > 0) {
    previousMillis = currentMillis;
  
    for (int i = 0; i < 3; i++) {
      // acceleration[i] = ((acceleration[i] - acc0[i]) * G);
      acceleration[i] = round((acceleration[i] - acc0[i]) * G * 100) / 100;
      // velocity[i] = velocity[i] + acceleration[i] * (interval / 1000000.0f);
      // distance[i] = distance[i] + velocity[i] * (interval / 1000000.0f);       
      velocity[i] = round(velocity[i] + acceleration[i] * (interval / 1000000.0f) * 100) / 100;
      distance[i] = round(distance[i] + velocity[i] * (interval / 1000000.0f) * 100) / 100;         
    }

    if (p == 5000) {
      Serial.printf("Acceleration: X: %f m/s² Y: %f m/s² Z: %f m/s²\n", acceleration[0], acceleration[1], acceleration[2]);
      Serial.printf("Velocity: X: %f m/s Y: %f m/s Z: %f m/s\n", velocity[0], velocity[1], velocity[2]);
      Serial.printf("Distans: X: %f m Y: %f m Z: %f m\n\n", distance[0], distance[1], distance[2]);  
      p = 0;
    }

    p++;
    
    calculateDistance(distance[0], distance[1], distance[2]);
  }

}

void calculateDistance(float x, float y, float z) {
  float distance = sqrt(x * x + y * y + z * z);

  if (distance > 10) {
    M5.dis.fillpix(0x00ff00);
  }
}

