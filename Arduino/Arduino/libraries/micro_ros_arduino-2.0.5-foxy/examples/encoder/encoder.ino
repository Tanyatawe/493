#include <std_msgs/msg/int32.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#define RPM_PIN A1 // RPM sensor connected to pin A1

rcl_node_t node;
rcl_publisher_t rpm_publisher;
std_msgs__msg__Int32 rpm_msg;

rclc_support_t support;
rcl_allocator_t allocator;

void setup() {
  // Initialize RPM sensor pin
  pinMode(RPM_PIN, INPUT);

  // Initialize ROS
  allocator = rcl_get_default_allocator();
  rclc_support_init(&support, 0, NULL, &allocator);

  // Create node
  rclc_node_init_default(&node, "rpm_sensor_node", "", &support);

  // Create publisher
  rclc_publisher_init_default(
    &rpm_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "rpm"
  );
}

void loop() {
  // Read RPM sensor data
  int rpmValue = analogRead(RPM_PIN);

  // Publish RPM value
  rpm_msg.data = rpmValue;
  rcl_publish(&rpm_publisher, &rpm_msg, NULL);

  delay(1000); // Adjust delay as needed
}
