#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import PoseStamped
from sensor_msgs.msg import JointState
from math import cos, sin, sqrt
from numpy import matrix
from visualization_msgs.msg import Marker, MarkerArray
from geometry_msgs.msg import PointStamped
from std_msgs.msg import Int32
import random
import yaml
import os
from ament_index_python.packages import get_package_share_directory


class CubeMoveNode(Node):
    def __init__(self):
        super().__init__("cube_move")
        
        # Load robot parameters
        self.load_robot_params()
        
        self.array_publisher =  self.create_publisher(MarkerArray, "camera_link", 10)
       
        self.joint_subscriber = self.create_subscription(JointState, 'joint_states', self.joints_callback, 10)
        self.pose_subscriber = self.create_subscription(PoseStamped, "pose", self.pose_callback, 10)
        self.state_subscriber = self.create_subscription(Int32, 'state', self.state_callback, 10)
        self.point_subscriber = self.create_subscription(
            PointStamped, "clicked_point", self.point_callback, 10)

        self.kostka_pos = [0.0, -0.04, -0.03]
        self.kostka_orient = [0.0, 0.0, 0.0, 1.0]
        self.kartka_pos = [0.2, -0.03, -0.04]
        self.kartka_orient = [0.0, 0.0, 0.0, 1.0]

        self.x, self.y, self.z = 0.0, 0.0, 0.0
        self.point_counter = 0
        self.state = 0
        self.joints = []
        
        # Create a timer to continuously update markers when cube is picked up
        # Increase frequency for smoother movement (50 Hz instead of 20 Hz)
        self.timer = self.create_timer(0.02, self.timer_callback)
        
        # Last end effector position when cube was dropped
        self.last_drop_position = None
        
        self.get_logger().info("Cube Move Node initialized")

    def load_robot_params(self):
        try:
            # Find path to robot_params.yaml file
            pkg_share = get_package_share_directory('pd_lab6')
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
            
        except Exception as e:
            self.get_logger().error(f"Failed to load parameters: {str(e)}")
            # Use default values if parameters can't be loaded
            self.base_height = 0.05
            self.link1_length = 0.088
            self.link2_length = 0.135
            self.link3_length = 0.147
            self.link4_length = 0.05
            self.link5_length = 0.02
            self.get_logger().warn("Using default robot parameters")

    def pose_callback(self, pose):
        # If cube is picked up (state == 1), update its position to follow the arm
        if self.state == 1:
            self.kostka_pos[0] = pose.pose.position.x + 0.024
            self.kostka_pos[1] = pose.pose.position.y
            self.kostka_pos[2] = pose.pose.position.z
            self.kostka_orient[0] = pose.pose.orientation.x
            self.kostka_orient[1] = pose.pose.orientation.y
            self.kostka_orient[2] = pose.pose.orientation.z
            self.kostka_orient[3] = pose.pose.orientation.w
            
            # When picked up, attach to joint5 for more accurate following
            self.publish_markers("joint5")
            
            # Store the current position in case cube is dropped
            self.last_drop_position = self.kostka_pos.copy()
            
            # Log for debugging
            self.get_logger().debug(f"Updating cube position to: {self.kostka_pos}")

    def state_callback(self, state_data):
        old_state = self.state
        self.state = state_data.data
        
        if old_state != self.state:
            self.get_logger().info(f"State changed from {old_state} to {self.state}")
            
            # If transitioning from picked up (1) to dropped (0)
            if old_state == 1 and self.state == 0:
                # Cube has been dropped at its current position
                # We'll switch to using base_link as the frame
                self.publish_markers("base_link")

    def timer_callback(self):
        # When the cube is picked up (state == 1), continuously update markers
        if self.state == 1:
            self.publish_markers("joint5")
        else:
            # When cube is not picked up, it should still be visible in the scene
            if self.last_drop_position is not None:
                self.publish_markers("base_link")
            else:
                # If we haven't dropped the cube yet, just use the default position
                self.publish_markers("base_link")

    def callback(self):
        kostka = Marker()
        kostka.header.frame_id = "camera_link"
        kostka.header.stamp.sec = self.get_clock().now().to_msg().sec
        kostka.header.stamp.nanosec = self.get_clock().now().to_msg().nanosec
        kostka.ns = "marker_namespace"
        kostka.id = 0
        kostka.type = 1
        kostka.action = 0
        kostka.frame_locked = True
        temp_cube = self.convert(self.kostka_pos)
        kostka.pose.position.x = float(temp_cube[0])
        kostka.pose.position.y = float(temp_cube[1])
        kostka.pose.position.z = float(temp_cube[2])
        kostka.pose.orientation.x = float(self.kostka_orient[0])
        kostka.pose.orientation.y = float(self.kostka_orient[1])
        kostka.pose.orientation.z = float(self.kostka_orient[2])
        kostka.pose.orientation.w = float(self.kostka_orient[3])
        kostka.scale.x = 0.02
        kostka.scale.y = 0.02
        kostka.scale.z = 0.02
        kostka.color.a = 1.0
        kostka.color.r = 1.0
        kostka.color.g = 0.5
        kostka.color.b = 0.0

        kartka = Marker()
        kartka.header.frame_id = "camera_link"
        kartka.header.stamp.sec = self.get_clock().now().to_msg().sec
        kartka.header.stamp.nanosec = self.get_clock().now().to_msg().nanosec
        kartka.ns = "marker_namespace"
        kartka.id = 1
        kartka.type = 1
        kartka.action = 0
        kartka.frame_locked = True
        kartka_trans = self.convert(self.kartka_pos)
        kartka.pose.position.x = float(kartka_trans[0])
        kartka.pose.position.y = float(kartka_trans[1])
        kartka.pose.position.z = float(kartka_trans[2])
        kartka.pose.orientation.x = float(self.kartka_orient[0])
        kartka.pose.orientation.y = float(self.kartka_orient[1])
        kartka.pose.orientation.z = float(self.kartka_orient[2])
        kartka.pose.orientation.w = float(self.kartka_orient[3])
        kartka.scale.x = 0.05
        kartka.scale.y = 0.1
        kartka.scale.z = 0.001
        kartka.color.a = 1.0
        kartka.color.r = 1.0
        kartka.color.g = 1.0
        kartka.color.b = 1.0
        
        markery = MarkerArray()
        markery.markers.append(kostka)
        markery.markers.append(kartka)
        self.array_publisher.publish(markery)

    def joints_callback(self, joints):
        self.joints = joints.position
        self.joint1 = joints.position[0]
        self.joint2 = joints.position[1]
        self.joint3 = joints.position[2]
        self.joint4 = joints.position[3]
        self.joint5 = joints.position[4]

    def point_callback(self, point):
        self.x = float(point.point.x)
        self.y = float(point.point.y)
        self.z = float(point.point.z)
        
        if self.point_counter == 0:
            # First point defines paper position (Point A)
            self.kartka_pos = [self.x, self.y, self.z]
            angle = random.uniform(-1, 1)
            self.kartka_orient = [0.0, 0.0, sqrt(1-angle**2), angle]
            self.point_counter += 1
            self.get_logger().info(f"Set paper position to: {self.kartka_pos}")
        else:
            # Second point defines cube position (Point B)
            self.kostka_pos = [self.x, self.y, self.z+0.01]  # Slightly above surface
            angle = random.uniform(-1, 1)
            self.kostka_orient = [0.0, 0.0, angle, sqrt(1-angle**2)]
            self.publish_markers("base_link")
            self.callback()  # Update markers in camera_link frame
            self.point_counter = 0  # Reset for next sequence
            self.get_logger().info(f"Set cube position to: {self.kostka_pos}")

    def convert(self, point):

        base_point = matrix([[0], [0], [0], [1]])

        M1 = matrix([[cos(self.joint1), -sin(self.joint1), 0, 0],
                    [sin(self.joint1), cos(self.joint1), 0, 0],
                    [0, 0, 1, self.base_height],
                    [0, 0, 0, 1]])

        M2 = matrix([[cos(self.joint2), 0, sin(self.joint2), 0],
                    [0, 1, 0, 0],
                    [-sin(self.joint2), 0, cos(self.joint2), self.link1_length],
                    [0, 0, 0, 1]])

        M3 = matrix([[cos(self.joint3+1.57), 0, sin(self.joint3+1.57), 0],
                    [0, 1, 0, 0],
                    [-sin(self.joint3+1.57), 0, cos(self.joint3+1.57), self.link2_length],
                    [0, 0, 0, 1]])

        M4 = matrix([[cos(self.joint4), 0, sin(self.joint4), 0],
                    [0, 1, 0, 0],
                    [-sin(self.joint4), 0, cos(self.joint4), self.link3_length],
                    [0, 0, 0, 1]])

        M5 = matrix([[1, 0, 0, 0.0],
                    [0, 1, 0, 0.0],
                    [0, 0, 1, self.link4_length + self.link5_length],
                    [0, 0, 0, 1]])

        M = M1*M2*M3*M4*M5
        self.position = M*base_point
        
        t_point = M*matrix([[point[0]], [point[1]], [point[2]], [1.0]])

        return t_point

    def publish_markers(self, link):
        # Create cube marker
        kostka = Marker()
        kostka.header.frame_id = link
        kostka.header.stamp.sec = self.get_clock().now().to_msg().sec
        kostka.header.stamp.nanosec = self.get_clock().now().to_msg().nanosec
        kostka.ns = "marker_namespace"
        kostka.id = 0
        kostka.type = 1  # Cube
        kostka.action = 0
        kostka.frame_locked = True
        
        # Set cube position and orientation
        kostka.pose.position.x = float(self.kostka_pos[0])
        kostka.pose.position.y = float(self.kostka_pos[1])
        kostka.pose.position.z = float(self.kostka_pos[2])
        kostka.pose.orientation.x = float(self.kostka_orient[0])
        kostka.pose.orientation.y = float(self.kostka_orient[1])
        kostka.pose.orientation.z = float(self.kostka_orient[2])
        kostka.pose.orientation.w = float(self.kostka_orient[3])
        kostka.scale.x = 0.02
        kostka.scale.y = 0.02
        kostka.scale.z = 0.02
        kostka.color.a = 1.0
        kostka.color.r = 1.0
        kostka.color.g = 0.5
        kostka.color.b = 0.0

        # Create paper marker (always in base_link frame)
        kartka = Marker()
        kartka.header.frame_id = "base_link"
        kartka.header.stamp.sec = self.get_clock().now().to_msg().sec
        kartka.header.stamp.nanosec = self.get_clock().now().to_msg().nanosec
        kartka.ns = "marker_namespace"
        kartka.id = 1
        kartka.type = 1  # Cube
        kartka.action = 0
        kartka.frame_locked = True
        
        # Set paper position and orientation
        kartka.pose.position.x = float(self.kartka_pos[0])
        kartka.pose.position.y = float(self.kartka_pos[1])
        kartka.pose.position.z = float(self.kartka_pos[2])
        kartka.pose.orientation.x = float(self.kartka_orient[0])
        kartka.pose.orientation.y = float(self.kartka_orient[1])
        kartka.pose.orientation.z = float(self.kartka_orient[2])
        kartka.pose.orientation.w = float(self.kartka_orient[3])
        kartka.scale.x = 0.05
        kartka.scale.y = 0.1
        kartka.scale.z = 0.001
        kartka.color.a = 1.0
        kartka.color.r = 1.0
        kartka.color.g = 1.0
        kartka.color.b = 1.0

        # Create and publish marker array
        markery = MarkerArray()
        markery.markers.append(kostka)
        markery.markers.append(kartka)
        self.array_publisher.publish(markery)


def main(arg=None):
    rclpy.init(args=arg)
    node = CubeMoveNode()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
