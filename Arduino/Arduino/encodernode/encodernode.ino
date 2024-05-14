#include <micro_ros_arduino.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/float64.h>

const int sensorPin = 1; // Digital input pin for the RPM sensor
const int pinionTeeth = 22; // Number of teeth on the pinion
const int spurGearTeeth = 76; // Number of teeth on the spur gear
const float pulsePeriod = 0.02; // Pulse period in seconds
const float circumference = 0.525; // Circumference of the rotating object in meters (adjust according to your setup)

unsigned long lastMillis = 0; // Time of the last pulse count reset
volatile unsigned long pulseCount = 0; // Variable to store pulse count
float rpm = 0; // RPM value

rcl_publisher_t motor_speed_publisher;
std_msgs__msg__Float64 motor_speed_msg;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

void error_loop(){
  while(1){
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(100);
  }
}

void setup() {
  set_microros_transports();
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(sensorPin, INPUT_PULLUP); // Set sensor pin as input with internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(sensorPin), countPulse, RISING); // Attach interrupt on rising edge

  allocator = rcl_get_default_allocator();

  // Initialize micro-ROS
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  RCCHECK(rclc_node_init_default(&node, "micro_ros_arduino_node", "", &support));

  // Initialize publisher
  RCCHECK(rclc_publisher_init_default(
    &motor_speed_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float64),
    "motor_speed"));

  // Initialize executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  motor_speed_msg.data = 0;
}

void loop() {
 
  // Calculate RPM every second
  if (millis() - lastMillis >= 1000) {
    // Calculate RPM using pulse count and time
     rpm = (pulseCount * 60.0) / (spurGearTeeth * pinionTeeth * pulsePeriod); // Calculate RPM
     float speed_mps = (rpm * (2.0*PI*circumference)) / 60.0;
    // Publish RPM message
    motor_speed_msg.data = rpm;
    RCSOFTCHECK(rcl_publish(&motor_speed_publisher, &motor_speed_msg, NULL));

    Serial.print("motor_speed: ");
    Serial.println(rpm);
    Serial.print("Speed (m/s): ");
    Serial.println(speed_mps);
    // Reset pulse count and update last millis
    pulseCount = 0;
    lastMillis = millis();
  }

  // Spin micro-ROS executor
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}

// Interrupt service routine to count pulses
void countPulse() {
  pulseCount++;
}
