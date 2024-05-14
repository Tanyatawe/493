const int sensorPin = 1; // Digital input pin for the RPM sensor
const int pinionTeeth = 22; // Number of teeth on the pinion
const int spurGearTeeth = 76; // Number of teeth on the spur gear
const float pulsePeriod = 0.02; // Pulse period in seconds
const float circumference = 0.8; // Circumference of the rotating object in meters (adjust according to your setup)

unsigned long lastMillis = 0; // Time of the last pulse count reset
volatile unsigned long pulseCount = 0; // Variable to store pulse count
float rpm = 0; // RPM value

void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(sensorPin, INPUT_PULLUP); // Set sensor pin as input with internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(sensorPin), countPulse, RISING); // Attach interrupt on rising edge
}

void loop() {
  // Calculate RPM every second
  if (millis() - lastMillis >= 1000) {
    // Calculate RPM using pulse count and time
    rpm = (pulseCount * 60.0) / (spurGearTeeth * pinionTeeth * pulsePeriod); // Calculate RPM
    float speed_mps = (rpm * (2.0*PI*circumference)) / 60.0; // Convert RPM to meters per second
    
    // Print RPM value to Serial monitor
//     Serial.print("RPM: ");
//    Serial.println(rpm);
    Serial.print("Speed (m/s): ");
    Serial.println(speed_mps);
    // Reset pulse count and update last millis
    pulseCount = 0;
    lastMillis = millis();
  }
}

// Interrupt service routine to count pulses
void countPulse() {
  pulseCount++;
}
