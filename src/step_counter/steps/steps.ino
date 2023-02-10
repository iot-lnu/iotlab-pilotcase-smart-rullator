#include "M5Atom.h"
#include <math.h>

#define PI 3.14159265

// sudo chmod a+rw /dev/ttyUSB0

float acc0[] = {0.0f, 0.0f, 0.0f};
float acc[] = {1.0f, 0.0f, 0.0f};
float prevAcc[] = {-1.0f, 1.0f, 0.0f};
int p = 0;
int steps = 0;

void setup(){
  M5.begin(true, false, true);
  delay(100);
  M5.IMU.Init();
  delay(100);
  M5.IMU.SetAccelFsr(M5.IMU.AFS_16G);
  delay(100);

  M5.IMU.getAccelData(&acc[0], &acc[1], &acc[2]);
  acc0[0] = acc[0];
  acc0[1] = acc[1];
  acc0[2] = acc[2];    
}

void loop() {
  M5.IMU.getAccelData(&acc[0], &acc[1], &acc[2]);
  acc[0] = acc[0] - acc0[0];
  acc[1] = acc[1] - acc0[1];
  acc[2] = acc[2] - acc0[2];
  // Serial.printf("Acceleration: X: %f G, Y: %f G, Z: %f G\n", acc[0], acc[1], acc[2]);   

  if (magnitude(acc, 3) > 0.1) {
    // float projection[3] = {0.0f, 0.0f, 0.0f};
    // projectVector(prevAcc, acc, 3, projection);
    
    if (round(angleBetweenVectors(acc, prevAcc, 3)) > 90) {
      prevAcc[0] = acc[0];
      prevAcc[1] = acc[1];
      prevAcc[2] = acc[2];    
      steps++;
      Serial.printf("%u steps\n", steps);  
      delay(400);            
    }
  }
}

float dotProduct(float A[], float B[], int n) {
  float result = 0.0;
  for (int i = 0; i < n; i++) {
    result += A[i] * B[i];
  }
  return result;
}

float magnitude(float A[], int n) {
  float result = 0.0;
  for (int i = 0; i < n; i++) {
    result += A[i] * A[i];
  }
  return sqrt(result);
}

float angleBetweenVectors(float A[], float B[], int n) {
  float dot = dotProduct(A, B, n);
  float magA = magnitude(A, n);
  float magB = magnitude(B, n);
  float cosTheta = dot / (magA * magB);
  return acos(cosTheta) * 180.0 / PI;
}

void projectVector(float A[], float B[], int n, float C[]) {
  float dot = dotProduct(A, B, n);
  float magB = magnitude(B, n);
  float scale = dot / (magB * magB);
  for (int i = 0; i < n; i++) {
    C[i] = scale * B[i];
  }
}
