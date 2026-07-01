#include <Arduino.h>
#include <Wire.h>

// I2C Address for MPU-6050
const int MPU_addr = 0x68; 

// Define GPIO Pins for Encoder A and B channels
#define ENC1_A 12
#define ENC1_B 13
#define ENC2_A 14
#define ENC2_B 27
#define ENC3_A 25
#define ENC3_B 26

// Volatile variables stored in RAM for fast ISR access
volatile long yaw_ticks = 0;
volatile long pitch_ticks = 0;
volatile long roll_ticks = 0;

// Variables for raw IMU datasets
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

// Interrupt Service Routines (ISRs) optimized for fast execution
void IRAM_ATTR readYawEncoder() {
  if (digitalRead(ENC1_A) == digitalRead(ENC1_B)) {
    yaw_ticks++;
  } else {
    yaw_ticks--;
  }
}

void IRAM_ATTR readPitchEncoder() {
  if (digitalRead(ENC2_A) == digitalRead(ENC2_B)) {
    pitch_ticks++;
  } else {
    pitch_ticks--;
  }
}

void IRAM_ATTR readRollEncoder() {
  if (digitalRead(ENC3_A) == digitalRead(ENC3_B)) {
    roll_ticks++;
  } else {
    roll_ticks--;
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize I2C Communication for the MPU-6050
  Wire.begin(21, 22); // SDA = 21, SCL = 22
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // Set to zero to wake up the MPU-6050
  Wire.endTransmission(true);

  // Configure Encoder Inputs with Internal Pullup Resistors
  pinMode(ENC1_A, INPUT_PULLUP);
  pinMode(ENC1_B, INPUT_PULLUP);
  pinMode(ENC2_A, INPUT_PULLUP);
  pinMode(ENC2_B, INPUT_PULLUP);
  pinMode(ENC3_A, INPUT_PULLUP);
  pinMode(ENC3_B, INPUT_PULLUP);

  // Bind Interrupts to falling/rising edges of Phase A channels
  attachInterrupt(digitalPinToInterrupt(ENC1_A), readYawEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC2_A), readPitchEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC3_A), readRollEncoder, CHANGE);
}

void loop() {
  // Read Data from MPU-6050 Register Block
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B); // Starting register for Accelerometer data (X-axis High)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // Request a total of 14 continuous registers

  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  // Package Data into a Clean Stream for Telemetry/Plotting
  Serial.print("Y:"); Serial.print(yaw_ticks);
  Serial.print(",P:"); Serial.print(pitch_ticks);
  Serial.print(",R:"); Serial.print(roll_ticks);
  Serial.print(",AX:"); Serial.print(AcX);
  Serial.print(",AY:"); Serial.print(AcY);
  Serial.print(",AZ:"); Serial.print(AcZ);
  Serial.print(",GX:"); Serial.print(GyX);
  Serial.print(",GY:"); Serial.print(GyY);
  Serial.print(",GZ:"); Serial.println(GyZ);

  delay(50); // Sample rate pacing (approx 20Hz stream for serialization debugging)
}
