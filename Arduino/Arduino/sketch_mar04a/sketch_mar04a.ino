#include <micro_ros_arduino.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
//BP added
#include <micro_ros_utilities/type_utilities.h>
#include <micro_ros_utilities/string_utilities.h>
#include <std_msgs/msg/float32.h>
#include <std_msgs/msg/string.h>
#include <std_msgs/msg/int32.h>

rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

//BP added
rcl_publisher_t string_publisher;
rcl_publisher_t pedometer_publisher;
rcl_publisher_t yaw_publisher;
std_msgs__msg__String string_msg;
std_msgs__msg__Int32 pedometer_msg;
std_msgs__msg__Float32 yaw_msg;

#define LED_PIN 13

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}


void error_loop(){
  while(1){
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
}



void setup() {
  set_microros_transports();
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  
  
  delay(2000);

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_arduino_node", "", &support));

  
  // BP create message publisher
  // create publisher
  RCCHECK(rclc_publisher_init_default(
    &string_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String),
    "detection"));
/*
  // create publisher
  RCCHECK(rclc_publisher_init_default(
    &pedometer_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "pedometer"));
*/
  // create publisher
  RCCHECK(rclc_publisher_init_default(
    &yaw_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
    "yaw")); 
  //end

}

void loop() {
  delay(100);
  
  //BP publish messages
  string_msg.data = micro_ros_string_utilities_set(string_msg.data, "This is BP");
  RCSOFTCHECK(rcl_publish(&string_publisher, &string_msg, NULL));
  //Display Number
  pedometer_msg.data = 5;
  //RCSOFTCHECK(rcl_publish(&pedometer_publisher, &pedometer_msg, NULL));
  //Display Float
  yaw_msg.data = 5.55;
  RCSOFTCHECK(rcl_publish(&yaw_publisher, &yaw_msg, NULL));
}
