import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState
import numpy as np
import yaml
import os


class rvizToDobot(Node):
    def __init__(self):
        super().__init__("rvizToDobot")  

        self.joint_subs = self.create_subscription(
            JointState, "/dobot_joint_states", self.dobot_joint_states_callback, 10
        )

        self.joint_pub = self.create_publisher(
            JointState, "/joint_states", 10
        )

    def dobot_joint_states_callback(self, msg):
        rvizJointStates = JointState()
        rvizJointStates.header.stamp = self.get_clock().now().to_msg()
        
        rvizJointStates.name = ["Base", "Rear_Arm", "Forearm", "End-effector_rotation", "Gripper_rotation"]
        
        j1 = msg.position[0]  # Base
        j2 = msg.position[1]  # Rear_Arm
        j3 = msg.position[2]  # Forearm
        j5 = msg.position[3]  # Gripper_rotation
        
        j3 += np.pi/2 -j2
        j4 = np.pi-(j3+j2)
        j5 = -j5-np.pi/2
        
        rvizJointStates.position = [j1, j2, j3, j4, j5]
        
        self.joint_pub.publish(rvizJointStates)
        self.get_logger().debug(f"Published RViz joint states: {rvizJointStates.position}")
        

def main(args=None):  
    rclpy.init(args=args)
    Convert = rvizToDobot()
    rclpy.spin(Convert)
    Convert.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":  
    main()