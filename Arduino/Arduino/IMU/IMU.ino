#include <Wire.h>
#include <Arduino_ICM_20948.h>
#include <micro_ros_arduino.h>
#include <geometry_msgs/msg/quaternion.h>
#include <geometry_msgs/msg/vector3.h>
#include <sensor_msgs/msg/imu.h>

ICM_20948 myICM;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  myICM.begin(Wire);
  myICM.initMPU();

  // Initialize microROS
  micro_ros_setup();

  // Create a microROS publisher for IMU data
  micro_ros_create_publisher_sensor_msgs_msg_Imu("/imu/data_raw", 10);
}

void loop() {
  if (myICM.dataReady()) {
    myICM.getAGMT();  // Update sensor data

    // Create a message for IMU data
    sensor_msgs__msg__Imu imu_msg;
    imu_msg.header.stamp.sec = micro_ros_clock_get_seconds();
    imu_msg.header.stamp.nanosec = micro_ros_clock_get_nseconds() % 1000000000;

    // Set orientation in quaternion format
    imu_msg.orientation = createQuaternion(myICM.getQuaternionI(),
                                           myICM.getQuaternionJ(),
                                           myICM.getQuaternionK(),
                                           myICM.getQuaternionReal());

    // Set angular velocity
    imu_msg.angular_velocity = createVector3((double)myICM.getRawGyroX() * 0.01745,
                                             (double)myICM.getRawGyroY() * 0.01745,
                                             (double)myICM.getRawGyroZ() * 0.01745);

    // Set linear acceleration
    imu_msg.linear_acceleration = createVector3(calculateLinearAcceleration(myICM.getRawAccelX()),
                                                calculateLinearAcceleration(myICM.getRawAccelY()),
                                                calculateLinearAcceleration(myICM.getRawAccelZ()));

    // Set covariance matrices (adjust the values according to your sensor specifications)
    for (int i = 0; i < 9; ++i) {
      imu_msg.orientation_covariance[i] = 0.0;  // Set your values here
      imu_msg.angular_velocity_covariance[i] = 0.0;  // Set your values here
      imu_msg.linear_acceleration_covariance[i] = 0.0;  // Set your values here
    }

    // Publish the IMU data
    micro_ros_publish(&imu_msg);

    delay(100);  // Adjust the delay as needed
  }

  // Spin microROS
  micro_ros_spin_once();
}

// Function to create a Quaternion message
geometry_msgs__msg__Quaternion createQuaternion(double x, double y, double z, double w) {
  geometry_msgs__msg__Quaternion quat_msg;
  quat_msg.x = x;
  quat_msg.y = y;
  quat_msg.z = z;
  quat_msg.w = w;
  return quat_msg;
}

// Function to create a Vector3 message
geometry_msgs__msg__Vector3 createVector3(double x, double y, double z) {
  geometry_msgs__msg__Vector3 vector3_msg;
  vector3_msg.x = x;
  vector3_msg.y = y;
  vector3_msg.z = z;
  return vector3_msg;
}

// Function to calculate linear acceleration from raw accelerometer data
double calculateLinearAcceleration(int16_t rawAccel) {
  // Assuming raw accelerometer values are in g (gravity) units
  return (double)rawAccel * 9.80665;
}
