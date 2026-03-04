#define BLYNK_TEMPLATE_ID "TMPL3sRjgjej8"
#define BLYNK_TEMPLATE_NAME "Pratham Motor"
#define BLYNK_AUTH_TOKEN  "8_Kxi_sfE6HFoEqNVImWGpxUxm5KIi2Z"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi credentials
char ssid[] = "Nisarg's Galaxy S21 FE 5G ";   //iQOO Neo7
char pass[] = "12345678";   //ieei1563

// ======================== PIN DEFINITIONS ========================
#define RELAY_PIN 13
#define CURRENT_SENSOR_PIN 34
#define TEMP_SENSOR_PIN 4
#define VIBRATION_PIN 5

// ======================== ACS712 CONFIG ========================
#define ACS712_SENSITIVITY 0.185
#define ACS712_VREF 2.5
#define ADC_SAMPLES 50

// ======================== THRESHOLDS ========================
#define CURRENT_NO_LOAD 0.08
#define CURRENT_NORMAL_MIN 0.20
#define CURRENT_NORMAL_MAX 1.20
#define CURRENT_WARNING 1.50
#define CURRENT_CRITICAL 2.00

#define TEMP_NORMAL_MAX 50.0
#define TEMP_WARNING 60.0
#define TEMP_CRITICAL 70.0

#define VIBRATION_NORMAL_MAX 5
#define VIBRATION_WARNING 10
#define VIBRATION_CRITICAL 20

#define FAULT_PERSISTENCE_TIME 3000
#define STARTUP_IGNORE_TIME 2000

// ======================== BLYNK PINS ========================
#define VPIN_MOTOR_CONTROL V0
#define VPIN_CURRENT V1
#define VPIN_TEMPERATURE V2
#define VPIN_VIBRATION V3
#define VPIN_FAULT_STATUS V4
#define VPIN_AUTO_SHUTDOWN V5
#define VPIN_MOTOR_STATUS V6

// ======================== GLOBAL VARIABLES ========================
OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature tempSensor(&oneWire);

float currentValue = 0.0;
float temperatureValue = 0.0;
int vibrationCount = 0;
bool motorRunning = false;
bool autoShutdown = true;
String faultStatus = "OK";

unsigned long lastVibrationTime = 0;
unsigned long faultStartTime = 0;
unsigned long motorStartTime = 0;
bool faultPersistent = false;

BlynkTimer timer;

// ======================== CURRENT READING ========================
float readCurrent() {
  long sumRaw = 0;

  for (int i = 0; i < ADC_SAMPLES; i++) {
    sumRaw += analogRead(CURRENT_SENSOR_PIN);
    delayMicroseconds(100);
  }

  float avgRaw = sumRaw / (float)ADC_SAMPLES;
  float voltage = (avgRaw / 4095.0) * 3.3;
  float current = abs((voltage - ACS712_VREF) / ACS712_SENSITIVITY);

  if (current < 0.05) current = 0.0;

  return current;
}

// ======================== SENSOR READING ========================
void readSensorsAndCheckFaults() {

  currentValue = readCurrent();

  tempSensor.requestTemperatures();
  temperatureValue = tempSensor.getTempCByIndex(0);

  Serial.println("========== SENSOR READINGS ==========");
  Serial.print("Motor: ");
  Serial.println(motorRunning ? "RUNNING" : "STOPPED");
  Serial.print("Current: ");
  Serial.print(currentValue, 3);
  Serial.println(" A");
  Serial.print("Temperature: ");
  Serial.print(temperatureValue, 1);
  Serial.println(" C");
  Serial.print("Vibration Count: ");
  Serial.println(vibrationCount);

  checkFaults();

  Serial.print("Fault Status: ");
  Serial.println(faultStatus);
  Serial.println("=====================================\n");

  Blynk.virtualWrite(VPIN_CURRENT, currentValue);
  Blynk.virtualWrite(VPIN_TEMPERATURE, temperatureValue);
  Blynk.virtualWrite(VPIN_VIBRATION, vibrationCount);
  Blynk.virtualWrite(VPIN_FAULT_STATUS, faultStatus);

  vibrationCount = 0;
}

// ======================== FAULT DETECTION ========================
void checkFaults() {

  String newFaultStatus = "OK";
  bool criticalFault = false;
  unsigned long currentMillis = millis();

  bool inStartupPhase = (motorRunning &&
                         (currentMillis - motorStartTime) < STARTUP_IGNORE_TIME);

  if (motorRunning && !inStartupPhase) {

    if (currentValue > CURRENT_CRITICAL) {
      newFaultStatus = "CRITICAL: Motor Stalled!";
      criticalFault = true;
    }
    else if (currentValue > CURRENT_WARNING) {
      newFaultStatus = "WARNING: High Current";
    }
    else if (currentValue > CURRENT_NORMAL_MAX) {
      newFaultStatus = "NOTICE: Elevated Current";
    }

    if (currentValue < CURRENT_NO_LOAD) {
      newFaultStatus = "FAULT: Motor Not Running";
      criticalFault = true;
    }
    else if (currentValue < CURRENT_NORMAL_MIN && newFaultStatus == "OK") {
      newFaultStatus = "WARNING: Low Current";
    }
  }

  if (temperatureValue > TEMP_CRITICAL) {
    newFaultStatus = "CRITICAL: Overheating!";
    criticalFault = true;
  }
  else if (temperatureValue > TEMP_WARNING && newFaultStatus == "OK") {
    newFaultStatus = "WARNING: High Temperature";
  }

  if (vibrationCount > VIBRATION_CRITICAL) {
    newFaultStatus = "CRITICAL: Excess Vibration!";
    criticalFault = true;
  }
  else if (vibrationCount > VIBRATION_WARNING && newFaultStatus == "OK") {
    newFaultStatus = "WARNING: High Vibration";
  }

  if (criticalFault) {
    if (!faultPersistent) {
      faultStartTime = currentMillis;
      faultPersistent = true;
    } else if (currentMillis - faultStartTime >= FAULT_PERSISTENCE_TIME) {
      if (autoShutdown && motorRunning) {
        stopMotor();
        Blynk.logEvent("motor_fault", newFaultStatus);
      }
    }
  } else {
    faultPersistent = false;
  }

  faultStatus = newFaultStatus;
}

// ======================== MOTOR CONTROL ========================
void startMotor() {
  digitalWrite(RELAY_PIN, HIGH);
  motorRunning = true;
  motorStartTime = millis();
  faultPersistent = false;

  Blynk.virtualWrite(VPIN_MOTOR_STATUS, 255);
}

void stopMotor() {
  digitalWrite(RELAY_PIN, LOW);
  motorRunning = false;

  Blynk.virtualWrite(VPIN_MOTOR_STATUS, 0);
}

// ======================== BLYNK CALLBACKS ========================
BLYNK_WRITE(VPIN_MOTOR_CONTROL) {
  int value = param.asInt();
  if (value == 1) startMotor();
  else stopMotor();
}

BLYNK_WRITE(VPIN_AUTO_SHUTDOWN) {
  autoShutdown = param.asInt();
}

BLYNK_CONNECTED() {
  Serial.println("Blynk Connected!");
}

// ======================== SETUP ========================
void setup() {

  Serial.begin(115200);
  delay(2000);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(VIBRATION_PIN, INPUT);
  digitalWrite(RELAY_PIN, LOW);

  tempSensor.begin();

  Serial.print("Connecting to WiFi & Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  if (Blynk.connected()) {
    Serial.println(" ✓ Connected!");
  } else {
    Serial.println(" ✗ Connection Failed");
  }

  timer.setInterval(2000L, readSensorsAndCheckFaults);

  Serial.println("SYSTEM READY - Monitoring...");
}

// ======================== LOOP ========================
void loop() {
  Blynk.run();
  timer.run();

  if (digitalRead(VIBRATION_PIN) == HIGH) {
    unsigned long now = millis();
    if (now - lastVibrationTime > 50) {
      vibrationCount++;
      lastVibrationTime = now;
    }
  }
}
