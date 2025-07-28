#include <TMCStepper.h>

// UART interface to TMC2209
#define DRIVER_ADDRESS 0b00     // Address set via MS1/MS2 (GND/GND = 0)
#define R_SENSE 0.11f           // Typical sense resistor value for BigTreeTech
#define EN_PIN    4             // Optional: connect to ENN pin (LOW to enable)
#define STEP_PIN 18
#define DIR_PIN  19

// ESP32 UART1 pins
#define RXD1 16  // Not used (PDN_UART is single wire), can be unused
#define TXD1 17  // Must connect this to PDN_UART pad on the driver

TMC2209Stepper driver(&Serial1, R_SENSE, DRIVER_ADDRESS);

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  Serial.begin(115200);       // For debug output
  delay(1000);
  Serial.println("TMC2209 UART test starting...");

  Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);

  driver.begin();             // Start driver
  driver.toff(5);             // Enable driver by setting off time
  driver.rms_current(400);    // Set motor RMS current to 800 mA
  driver.microsteps(16);      // Set 1/16 microstepping
  driver.pdn_disable(true);   // Use PDN_UART for UART
  driver.I_scale_analog(false); // Use digital current scaling
  driver.en_spreadCycle(false); // Use StealthChop (quiet mode)

  Serial.print("DRV_STATUS: 0x");
  Serial.println(driver.DRV_STATUS(), HEX);  // Should return a valid hex value (not all 1s or 0s)

}

void loop() {
  // Simple movement loop
  digitalWrite(DIR_PIN, HIGH);
  for (int i = 0; i < 2000; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(100);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(100);
  }

  delay(1000);

  digitalWrite(DIR_PIN, LOW);
  for (int i = 0; i < 2000; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(100);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(100);
  }

  delay(1000);

    Serial.println(driver.DRV_STATUS(), HEX);  // Should return a valid hex value (not all 1s or 0s)

}
