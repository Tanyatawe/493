import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from sensor_msgs.msg import LaserScan 
from std_msgs.msg import Float32,Float64 
from sensor_msgs.msg import Joy
from vesc_msgs.msg import VescStateStamped

prev_mode = 0
duty_cycle = 0.12
servo_range = 1.8
std_center_range = 80
gain_dutycycle = 16
max_center_range = 150
eva_dis = 0.4
def translate(value, InMin, InMax, OutMin, OutMax):
    leftSpan = InMax - InMin
    rightSpan = OutMax - OutMin
    valueScaled = float(value - InMin) / float(leftSpan)
    Out  = OutMin + (valueScaled * rightSpan)
    if(Out > OutMax):
        Out =OutMax
    elif(Out < OutMin):
        Out = OutMin
    return round(Out,4)

def MEAN(ranges):
    MAE = sum(ranges)/len(ranges)
    return MAE

def filtered(rangee):
    high_filter = [x for x in rangee if x <= 9.99]
    low_filter = [x for x in high_filter if x > 0.09]
    if(low_filter == []):
        low_filter = [0]
    return low_filter

class MinimalSubscriber(Node):

  def __init__(self):

    super().__init__('minimal_subscriber')
    self.subscription = self.create_subscription(
      Joy,
      'joy',
      self.joycallback,
      10)
    self.publishspeed = self.create_publisher(Float64, '/commands/motor/speed', 10)


    self.publishangle = self.create_publisher(Float64, '/commands/servo/position', 10)

    self.sublidar = self.create_subscription(
      LaserScan,
      'scan',
      self.lidarcallback,
      10)


    self.subscription 
    self.sublidar
 
  def joycallback(self, joy):

    global prev_mode
    manual_mode = joy.buttons[5]
    auto_mode = joy.buttons[4]
    stop_b = joy.buttons[2]

    if manual_mode == 1:
      prev_mode = 0
    
    if auto_mode == 1:
      prev_mode = 1
    
    if stop_b == 1:
      prev_mode = 10
      msg2 = Float64()
      msg2.data = 0.0
      self.publishangle.publish(msg2)
     
      msg = Float64()
      msg.data = 0.5
      self.publishspeed.publish(msg)


    if prev_mode == 0 :
      #manual mode
      joy_s = joy.axes[3]
      #pub servo
      joy_s = translate(joy_s,1.0,-1.0,0.15,0.85)
      
      msg2 = Float64()
      msg2.data = joy_s
      self.publishangle.publish(msg2)

      joy_a = joy.axes[7]
      #pub servo
      joy_a = translate(joy_a,-1.0,1.0,-1500,1500)
      
      msg = Float64()
      msg.data = joy_a
      self.publishspeed.publish(msg)
    

  def lidarcallback(self, scan_msg):

    global prev_mode, gain_mae,gain_dis_max,duty_cycle,servo_range,std_center_range,gain_dutycycle, max_center_range, eva_dis
    #manual mode 
    
  
    print(prev_mode)
    if prev_mode == 0:
      print("manual mode")

    #auto mode
    elif prev_mode == 1:
      print("auto mode")
      safety_gap1 = 200
      safety_gap2 = 450
      left_gap = 99
      right_gap = 101

      gain_mae = 1.4
      gain_dis_max = 0.16
      gain_dis_min = 1-gain_mae-gain_dis_max

      speed_min = 0.01

      scan_msg_ranges = scan_msg.ranges
      len_ranges = len(scan_msg.ranges)
      
      safety_min1 = min(scan_msg_ranges[int(len_ranges/2)-safety_gap1:int(len_ranges/2)+safety_gap1])
      safety_min2 = min(scan_msg_ranges[int(len_ranges/2)-safety_gap2:int(len_ranges/2)+safety_gap2])
      safety_max = max(scan_msg_ranges[int(len_ranges/2)-safety_gap2:int(len_ranges/2)+safety_gap2])
      
      ranges_left = scan_msg_ranges[int(len_ranges/2)+1:len_ranges-left_gap]
      ranges_right = scan_msg_ranges[right_gap:int(len_ranges/2)-1]
      
      right_filtered = filtered(ranges_right)
      left_filtered = filtered(ranges_left)

      dis_min =  min(left_filtered)-min(right_filtered)
      dis_max = max(left_filtered)-max(right_filtered)

      MAE_right = MEAN(right_filtered)
      MAE_left = MEAN(left_filtered)

      right_min = 0-translate(min(right_filtered),0.1,eva_dis,-1.2,0.0)
      left_min = translate(min(left_filtered),0.1,eva_dis,-1.2,0.0)
      print("MIN = ",dis_min)
      print("MAX = ",dis_max)
      angle = (gain_dis_min*dis_min)+(gain_mae*(MAE_left - MAE_right))+(gain_dis_max*dis_max)+right_min+left_min
      servoangle = translate(angle,0-servo_range,servo_range,-1.0,1.0)

      servoangle = translate(servoangle,-1.0,1.0,0.15,0.85)

      #Robust control -- Angle
      #if servoangle > 0.3 and servoangle < 0.4: 
       #  servoangle = servoangle-0.2
      #elif servoangle > 0.6 and servoangle <0.7:
       #  servoangle = servoangle+0.1
      #else:
       #  servoangle = servoangle

      #Robust control widee track 
      #if left_gap > : 
      #   servoangle = servoangle-0.1
      #elif servoangle > 0.6 and servoangle <0.7:
      #   servoangle = servoangle+0.1
      #else:
      #   servoangle = servoangle

      print("servoangle = ",servoangle)
      msg2 = Float64()
      msg2.data = servoangle
      self.publishangle.publish(msg2)

      safety1 = translate(safety_min1,0.15,0.22,0.0,1.0)
      safety2 = translate(safety_min2,0.12,0.18,0.0,1.0)
      
      center_range =  translate(abs((MAE_left - MAE_right)),0.01,0.4,0,max_center_range)
      center_gap = int(std_center_range+center_range)
      s_range = translate(max_center_range-center_gap,0,max_center_range-std_center_range,1.0,2.0)
      ranges_center = min(scan_msg_ranges[int(len_ranges/2)-center_gap:int(len_ranges/2)+center_gap])
      dutycycle = (translate(ranges_center,0.1,duty_cycle*gain_dutycycle*s_range,speed_min,duty_cycle))*safety1*safety2
      
      dutycycle = translate(dutycycle,-0.0,0.1,0.0,15500.0)

      print("dutycycle = ",dutycycle)

      #Robust control -- Dutycycle
      # if dutycycle > 3000.0 and dutycycle < 5000.0: 
      #   dutycycle = dutycycle + 2000.0

      msg = Float64()
      msg.data = dutycycle
      self.publishspeed.publish(msg)
    

def main(args=None):
 
  rclpy.init(args=args)
 
  minimal_subscriber = MinimalSubscriber()
 
  rclpy.spin(minimal_subscriber)
 
  minimal_subscriber.destroy_node()
   
  rclpy.shutdown()
 
if __name__ == '__main__':
  main()
