#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState
from math import cos, sin, sqrt, tan
from math import acos, asin
from visualization_msgs.msg import Marker, MarkerArray
from geometry_msgs.msg import PoseStamped
import time
import numpy as np
from numpy import matrix
from math import atan, acos
from numpy.linalg import inv
from std_msgs.msg import Int32
import yaml
import os
from ament_index_python.packages import get_package_share_directory

class ReverseKinNode(Node):
    def __init__(self):
        super().__init__("reverse_kin")
        # self.publisher = self.create_publisher(JointState, 'joint_states', 10)
        self.marker_subscriber = self.create_subscription(MarkerArray, "camera_link", self.marker_callback, 10)
        self.state_publisher = self.create_publisher(Int32, 'state', 10)
        self.pose_publisher = self.create_publisher(PoseStamped, "pose", 10)
        
        # Load robot parameters from YAML file
        self.load_robot_params()
        
        self.joint1 = 0.0      
        self.joint2 = 0.5     
        self.joint3 = 1      
        self.joint4 = 1    
        self.joint5 = 0.0      
        
        self.kostka_pos_x = 1.0
        self.kostka_pos_y = 1.0
        self.kostka_pos_z = 1.0        
 
        self.timer = self.create_timer(0.02, self.publish)
        
        self.krok = 100
        self.counter_krok = 0
        self.p_joint1 = 0.0
        self.p_joint2 = 0.5
        self.p_joint3 = 1
        self.p_joint4 = 1
        self.p_joint5 = 0.0
        self.state = 0
        self.zmiana_p = False
        self.otrzymanie_kostki = False
        self.a = 0

    def load_robot_params(self):
        try:
            # Find path to robot_params.yaml file
            pkg_share = get_package_share_directory('anro_lab6')
            params_path = os.path.join(pkg_share, 'dobot_params.yaml')
            
            # Load parameters from file
            with open(params_path, 'r') as file:
                params = yaml.safe_load(file)
                
            # Extract parameters (converting string values to float)
            self.base_height = float(params.get('base_height', 0.05))
            self.link1_length = float(params.get('link1_length', 0.088))
            self.link2_length = float(params.get('link2_length', 0.135))
            self.link3_length = float(params.get('link3_length', 0.147))
            self.link4_length = float(params.get('link4_length', 0.05))
            self.link5_length = float(params.get('link5_length', 0.02))
            self.base_limit_lower = float(params.get('base_limit_lower', -3.14))
            self.base_limit_upper = float(params.get('base_limit_upper', 3.14))
            self.link1_limit_lower = float(params.get('link1_limit_lower', -1.57))
            self.link1_limit_upper = float(params.get('link1_limit_upper', 1.57))
            self.link2_limit_lower = float(params.get('link2_limit_lower', -1.57))
            self.link2_limit_upper = float(params.get('link2_limit_upper', 1.57))
            self.link3_limit_lower = float(params.get('link3_limit_lower', -1.57))
            self.link3_limit_upper = float(params.get('link3_limit_upper', 1.57))
            self.link4_limit_lower = float(params.get('link4_limit_lower', -1.57))
            self.link4_limit_upper = float(params.get('link4_limit_upper', 1.57))
            self.link5_limit_lower = float(params.get('link5_limit_lower', -1.57))
            self.link5_limit_upper = float(params.get('link5_limit_upper', 1.57))
            
            
            # Use renamed variables for backward compatibility
            self.rear_arm_l = self.link2_length
            self.fore_arm_l = self.link3_length
            
            self.get_logger().info(f"Loaded robot parameters from {params_path}")
            
        except Exception as e:
            self.get_logger().error(f"Failed to load parameters: {str(e)}")
            # Use default values if parameters can't be loaded
            self.base_height = 0.05
            self.link1_length = 0.088
            self.link2_length = 0.135
            self.link3_length = 0.147
            self.link4_length = 0.05
            self.link5_length = 0.02
            self.base_limit_lower = -1.57
            self.base_limit_upper = 1.57
            self.link1_limit_lower = 0.0
            self.link1_limit_upper = 1.48
            self.link2_limit_lower = -0.17
            self.link2_limit_upper = 1.57
            self.link3_limit_lower = -1.57
            self.link3_limit_upper = 1.57
            self.link4_limit_lower = -1.57
            self.link4_limit_upper = 1.57

            
            # Use renamed variables for backward compatibility
            self.rear_arm_l = self.link2_length
            self.fore_arm_l = self.link3_length
            
            self.get_logger().warn("Using default robot parameters")
        
    def publish_state(self):
        state_info = Int32()
        state_info.data = self.state
        self.state_publisher.publish(state_info)
        
    def publish_pose(self):
        """Publish current end effector pose for the cube to follow"""
        if self.state == 1:  # Only publish when cube is picked up
            pose_msg = PoseStamped()
            pose_msg.header.stamp = self.get_clock().now().to_msg()
            pose_msg.header.frame_id = "base_link"
            
            # Calculate forward kinematics to get end effector position
            H1 = matrix([
                [cos(self.joint1), -sin(self.joint1), 0, 0],
                [sin(self.joint1), cos(self.joint1), 0, 0],
                [0, 0, 1, self.base_height/2],
                [0, 0, 0, 1]
            ])

            # Joint 2: Rear_Arm (around Y)
            H2 = matrix([
                [cos(self.joint2), 0, sin(self.joint2), 0],
                [0, 1, 0, 0],
                [-sin(self.joint2), 0, cos(self.joint2), self.link1_length],
                [0, 0, 0, 1]
            ])

            # Joint 3: Forearm (around Y)
            H3 = matrix([
                [cos(self.joint3), 0, sin(self.joint3), 0],
                [0, 1, 0, 0],
                [-sin(self.joint3), 0, cos(self.joint3), self.link2_length],
                [0, 0, 0, 1]
            ])

            # Joint 4: End-effector_rotation (around Y)
            H4 = matrix([
                [cos(self.joint4), 0, sin(self.joint4), 0],
                [0, 1, 0, 0],
                [-sin(self.joint4), 0, cos(self.joint4), self.link3_length],
                [0, 0, 0, 1]
            ])

            # Joint 5: Gripper_rotation (around Z)
            H5 = matrix([
                [cos(self.joint5), -sin(self.joint5), 0, 0],
                [sin(self.joint5), cos(self.joint5), 0, 0],
                [0, 0, 1, self.link4_length],
                [0, 0, 0, 1]
            ])

            # Additional transform to the end of the gripper
            H6 = matrix([
                [1, 0, 0, 0],
                [0, 1, 0, 10],
                [0, 0, 1, self.link5_length],
                [0, 0, 0, 1]
            ])
            H = H1*H2*H3*H4*H5*H6
            
            # Get end effector position
            end_effector_pos = H * matrix([[0], [0], [0], [1]])
            
            pose_msg.pose.position.x = float(end_effector_pos[0])
            pose_msg.pose.position.y = float(end_effector_pos[1])
            pose_msg.pose.position.z = float(end_effector_pos[2])-2*self.link5_length
            
            # Set orientation to identity quaternion
            pose_msg.pose.orientation.x = 0.0
            pose_msg.pose.orientation.y = 0.0
            pose_msg.pose.orientation.z = 0.0
            pose_msg.pose.orientation.w = 1.0
            
            # self.pose_publisher.publish(pose_msg)
        
    def marker_callback(self, markery):
            self.name = "góra"
            kostka = markery.markers[0]
            kartka = markery.markers[1]
            
            n_kostka_pos = self.convert(kostka.pose.position)
            n_kartka_pos = self.convert(kartka.pose.position)
            self.kostka_pos_x = n_kostka_pos[0]
            self.kostka_pos_y = n_kostka_pos[1]
            self.kostka_pos_z = n_kostka_pos[2]
            self.kostka_orient_x = kostka.pose.orientation.x
            self.kostka_orient_y = kostka.pose.orientation.y
            self.kostka_orient_z = kostka.pose.orientation.z
            self.kostka_orient_w = kostka.pose.orientation.w
            self.kartka_pos_x = n_kartka_pos[0]
            self.kartka_pos_y = n_kartka_pos[1]
            self.kartka_pos_z = n_kartka_pos[2]
            self.kartka_orient_x = kartka.pose.orientation.x
            self.kartka_orient_y =kartka.pose.orientation.y
            self.kartka_orient_z =kartka.pose.orientation.z
            self.kartka_orient_w = kartka.pose.orientation.w
        
            # self.move()
            
    def move(self):
        if self.name == "Kostka":
            x = self.kostka_pos_x 
            y = self.kostka_pos_y
            z = self.kostka_pos_z - 0.005    
            orient_w = self.kostka_orient_w
        if self.name == "kartka":
            x = self.kartka_pos_x 
            y = self.kartka_pos_y 
            z = self.kartka_pos_z + 0.01
            orient_w = self.kartka_orient_w
            
        if self.name == "góra":
            # Podnieś nad środek kartki
            x = 0.2
            y = 0
            z = 0.1
            orient_w = self.kostka_orient_w 
    
        lb = self.base_height
        l1 = self.link1_length
        print(x, y, z)
        l2 = self.link2_length
        l3 = self.link3_length
        l4 = self.link4_length
        dx = np.sqrt(x**2 + y**2)
        alpha = np.arctan((l4+z-lb)/dx)
        d = np.sqrt(dx**2 + (l4+z-lb-l1)**2)
        self.get_logger().info(f"x, y, z: {x}, {y}, {z}; Calculated distance: {d}, l2+l3: {l2+l3}")
        theta1 = np.arctan2(y, x)
        if_error = False
        if d > (l2 + l3):
            if_error = True
        else:
            gamma = np.arccos((l2**2 + d**2 - l3**2)/(2*l2*d))
            theta2 = np.pi/2 - alpha - gamma
            theta3 = np.pi-np.arccos((l2**2 + l3**2 - d**2)/(2*l2*l3))
            theta4=np.pi-theta2-theta3
            theta1 = np.arctan2(y, x)
            # theta5 = self.convert_orientation(quat)

            if theta1 < self.base_limit_lower or theta1 > self.base_limit_upper:
                self.get_logger().error("Base joint angle out of limits")
                if_error = True
            if theta2 < self.link1_limit_lower or theta2 > self.link1_limit_upper:
                self.get_logger().error("Link 1 joint angle out of limits")
                if_error = True
            if theta3 < self.link2_limit_lower or theta3 > self.link2_limit_upper:
                self.get_logger().error("Link 2 joint angle out of limits")
                if_error = True
            if theta4 < self.link3_limit_lower or theta4 > self.link3_limit_upper:
                self.get_logger().error("Link 3 joint angle out of limits")
                if_error = True
        if not if_error:
            self.joint1 = theta1
            self.joint2 = theta2
            self.joint3 = theta3
            self.joint4 = theta4
            self.joint5 = theta1 - 2*acos(orient_w)  # Assuming joint 5 follows joint 1 for simplicity
        else:
            self.joint1 = 0.0      
            self.joint2 = 0.5     
            self.joint3 = 1      
            self.joint4 = 1    
            self.joint5 = 0.0  
        self.k_joint1 = (self.joint1 - self.p_joint1)/self.krok
        self.k_joint2 = (self.joint2 - self.p_joint2)/self.krok
        self.k_joint3 = (self.joint3 - self.p_joint3)/self.krok
        self.k_joint4 = (self.joint4 - self.p_joint4)/self.krok
        self.k_joint5 = (self.joint5 - self.p_joint5)/self.krok  # Assuming joint 5 follows joint 1 for simplicity
        self.zmiana_p = True
 
    def convert(self, point):
        H1 = matrix([
            [cos(self.joint1), -sin(self.joint1), 0, 0],
            [sin(self.joint1), cos(self.joint1), 0, 0],
            [0, 0, 1, self.base_height/2],
            [0, 0, 0, 1]
        ])

        # Joint 2: Rear_Arm (around Y)
        H2 = matrix([
            [cos(self.joint2), 0, sin(self.joint2), 0],
            [0, 1, 0, 0],
            [-sin(self.joint2), 0, cos(self.joint2), self.link1_length],
            [0, 0, 0, 1]
        ])

        # Joint 3: Forearm (around Y)
        H3 = matrix([
            [cos(self.joint3), 0, sin(self.joint3), 0],
            [0, 1, 0, 0],
            [-sin(self.joint3), 0, cos(self.joint3), self.link2_length],
            [0, 0, 0, 1]
        ])

        # Joint 4: End-effector_rotation (around Y)
        H4 = matrix([
            [cos(self.joint4), 0, sin(self.joint4), 0],
            [0, 1, 0, 0],
            [-sin(self.joint4), 0, cos(self.joint4), self.link3_length],
            [0, 0, 0, 1]
        ])

        # Joint 5: Gripper_rotation (around Z)
        H5 = matrix([
            [cos(self.joint5), -sin(self.joint5), 0, 0],
            [sin(self.joint5), cos(self.joint5), 0, 0],
            [0, 0, 1, self.link4_length],
            [0, 0, 0, 1]
        ])

        # Additional transform to the end of the gripper
        H6 = matrix([
            [1, 0, 0, 0],
            [0, 1, 0, 0],
            [0, 0, 1, self.link5_length],
            [0, 0, 0, 1]
        ])
        H_camera = matrix([
            [1, 0, 0, 10],
            [0, 1, 0, 0],
            [0, 0, 1, -2],
            [0, 0, 0, 1]
        ])
        H = H1*H2*H3*H4*H5*H6 #H_camera
        H = inv(H)
        t_point = H*matrix([[point.x], [point.y], [point.z], [1.0]])
        # self.get_logger().info(f"Converted point: {t_point}")
        return t_point
            
    def publish(self):
        if  self.zmiana_p:
            self.p_joint1 += self.k_joint1 
            self.p_joint2 += self.k_joint2
            self.p_joint3 += self.k_joint3 
            self.p_joint4 += self.k_joint4 
            self.p_joint5 += self.k_joint5 
            self.counter_krok += 1
            
            # Publish pose update during movement so cube follows arm smoothly
            self.publish_pose()
            
        if self.counter_krok == self.krok:
            time.sleep(1)
            a = 0
            if self.name == "góra" and self.a == 0:
                self.name = "Kostka"
                self.a = 1
                
            elif self.name == "Kostka":
                self.name = "góra"
                self.a = 1
                self.state = 1
                
            elif self.name == "góra" and self.a == 1:
                self.name = "kartka"
                self.a = 0
                # self.state = 1
            
            else:
                self.name = ""
                # self.state = 0
            
            self.publish_state()
            # self.otrzymanie_kostki = (self.state == 1)
            self.otrzymanie_kostki = False
            self.counter_krok = 0
            if self.name != "":
                # self.move()
                pass
            else:
                self.state = 0
                self.publish_state()
                self.zmiana_p = False
                
        new_msg = JointState()
        new_msg.name = ["Base", "Rear_Arm", "Forearm", "End-effector_rotation", "Gripper_rotation"]
        new_msg.header.frame_id = "base_link"
        new_msg.header.stamp.sec = self.get_clock().now().to_msg().sec
        new_msg.header.stamp.nanosec = self.get_clock().now().to_msg().nanosec
        new_msg.position = [self.p_joint1, self.p_joint2, self.p_joint3, self.p_joint4, self.p_joint5]
        # self.publisher.publish(new_msg)
        
def main(arg=None):
    rclpy.init(args=arg)
    node = ReverseKinNode()
    rclpy.spin(node)
    rclpy.shutdown()
    
if __name__ == "__main__":
    main()
