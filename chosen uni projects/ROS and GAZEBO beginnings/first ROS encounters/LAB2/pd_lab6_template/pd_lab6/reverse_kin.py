import rclpy
from rclpy.node import Node
from geometry_msgs.msg import PointStamped, PoseStamped
from sensor_msgs.msg import JointState
from std_msgs.msg import Int32
import numpy as np
import yaml
import os
from ament_index_python.packages import get_package_share_directory


class ReverseKinNode(Node):
    def __init__(self):
        super().__init__("backward_kinematics")
        
        # Create subscription to end-effector pose
        self.pose_sub = self.create_subscription(
            PointStamped, "/clicked_point", self.point_callback, 10
        )
        
        # Create publisher for joint states
        self.joint_states_pub = self.create_publisher(
            JointState, "/joint_states", 10
        )
        
        # Add a publisher for the end-effector pose to update cube position
        self.pose_pub = self.create_publisher(
            PoseStamped, "pose", 10
        )
        
        # Add a publisher for the state (to indicate when cube is picked up)
        self.state_pub = self.create_publisher(
            Int32, "state", 10
        )
        
        # Load parameters from robot_params.yaml
        self.load_robot_params()
        
        # Current joint angles
        self.current_angles = [0.00, 0.57, 1.50, 1.06, 0.00]
        
        # Target joint angles for animation
        self.target_angles = self.current_angles.copy()
        
        # Animation parameters
        self.is_animating = False
        self.animation_duration = 2.0  # seconds
        self.animation_steps = 50  # total steps for animation
        self.current_step = 0
        self.start_angles = self.current_angles.copy()
        
        # Create a timer for the animation updates (20Hz = 50ms)
        self.animation_timer = self.create_timer(self.animation_duration / self.animation_steps, self.animation_callback)
        
        # Publish initial joint state
        self.publish_joint_state(self.current_angles)
        
        # Tracking variables for the animation sequence
        self.paper_position = None  # Point A - paper location
        self.cube_position = None   # Point B - cube location
        self.home_angles = self.current_angles.copy()  # Home position angles
        
        # State tracking for the animation sequence
        # 0: Initial state, waiting for points
        # 1: Moving to cube (Point B)
        # 2: Moving to paper (Point A)
        # 3: Moving back to home position
        self.sequence_state = 0
        
        # Animation point counter
        self.point_counter = 0
        
        # Distance threshold for determining if we're at a target point
        self.distance_threshold = 0.03
        
        # Flag to track if cube is picked up
        self.is_cube_picked_up = False
        
        self.get_logger().info("Reverse kinematics node initialized")
        
    def load_robot_params(self):
        try:
            # Find path to robot_params.yaml file
            pkg_share = get_package_share_directory('pd_lab6')
            params_path = os.path.join(pkg_share, 'dobot_params.yaml')
            
            # Load parameters from file
            with open(params_path, 'r') as file:
                params = yaml.safe_load(file)
                
            # Extract parameters (converting string values to float)
            self.base_height = float(params.get('base_height'))
            self.link1_length = float(params.get('link1_length'))
            self.link2_length = float(params.get('link2_length'))
            self.link3_length = float(params.get('link3_length'))
            self.link4_length = float(params.get('link4_length'))
            self.link5_length = float(params.get('link5_length'))
            
            # Joint limits (we'll need these for IK)
            self.base_limit_lower = float(params.get('base_limit_lower'))
            self.base_limit_upper = float(params.get('base_limit_upper'))
            self.link1_limit_lower = float(params.get('link1_limit_lower'))
            self.link1_limit_upper = float(params.get('link1_limit_upper'))
            self.link2_limit_lower = float(params.get('link2_limit_lower'))
            self.link2_limit_upper = float(params.get('link2_limit_upper'))
            self.link3_limit_lower = float(params.get('link3_limit_lower'))
            self.link3_limit_upper = float(params.get('link3_limit_upper'))
            self.link4_limit_lower = float(params.get('link4_limit_lower'))
            self.link4_limit_upper = float(params.get('link4_limit_upper'))
            
        except Estatexception as e:
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
            self.get_logger().warn("Using default parameters due to loading failure.")
        
    def point_callback(self, msg):
        # Extract position from the PointStamped message
        x = msg.point.x
        y = msg.point.y
        z = msg.point.z
        
        self.get_logger().info(f"Received point: x={x:.3f}, y={y:.3f}, z={z:.3f}")
        
        # Determine which point was selected based on counter
        if self.point_counter == 0:
            # First point is paper location (Point A)
            self.paper_position = [x, y, z]
            self.get_logger().info(f"Set paper position (Point A) to: {self.paper_position}")
            self.point_counter += 1
            
        elif self.point_counter == 1:
            # Second point is cube location (Point B)
            self.cube_position = [x, y, z]
            self.get_logger().info(f"Set cube position (Point B) to: {self.cube_position}")
            self.point_counter = 0  # Reset counter for next sequence
            
            # Start the animation sequence by moving to cube position first
            self.sequence_state = 1
            self.start_movement_to_point(self.cube_position)
    
    def start_movement_to_point(self, target_point):
        """Start movement to a specified target point"""
        x, y, z = target_point
        
        # Calculate inverse kinematics for target point
        new_joint_angles = self.inverse_kinematics(x, y, z, self.link1_length, 
                                                  self.link2_length, self.link3_length, 
                                                  self.link4_length, self.base_height)
        
        if new_joint_angles is None:
            self.get_logger().error(f"Could not calculate inverse kinematics for point {target_point}")
            return
        
        # Set up animation from current angles to new angles
        self.start_animation(new_joint_angles)
    
    def start_animation(self, target_angles):
        # Store the start angles (current position)
        self.start_angles = self.current_angles.copy()
        
        # Set the target angles
        self.target_angles = target_angles
        
        # Reset animation progress
        self.current_step = 0
        self.is_animating = True
        
        self.get_logger().info(f"Starting animation from {[f'{angle:.2f}' for angle in self.start_angles]} to {[f'{angle:.2f}' for angle in self.target_angles]}")
    
    def animation_callback(self):
        """Timer callback that updates joint angles during animation"""
        if not self.is_animating:
            return
            
        # Update current step
        self.current_step += 1
        
        if self.current_step <= self.animation_steps:
            # Calculate progress (0.0 to 1.0)
            t = self.current_step / self.animation_steps
            
            # Apply easing function for smoother animation (ease-in-out)
            t = self.ease_in_out(t)
            
            # Interpolate joint angles
            new_angles = []
            for i in range(len(self.start_angles)):
                angle = self.start_angles[i] + t * (self.target_angles[i] - self.start_angles[i])
                new_angles.append(angle)
            
            # Update current angles
            self.current_angles = new_angles
            
            # Publish the interpolated joint state
            self.publish_joint_state(self.current_angles)
            
            # Calculate and publish end effector position for cube to follow
            current_position = self.calculate_end_effector_position(self.current_angles)
            self.publish_end_effector_pose(current_position)
            
        else:
            # Animation complete - ensure we set the exact target angles
            self.current_angles = self.target_angles.copy()
            self.publish_joint_state(self.current_angles)
            
            # Publish final end effector position
            current_position = self.calculate_end_effector_position(self.current_angles)
            self.publish_end_effector_pose(current_position)
            
            # Animation complete, handle next steps based on sequence state
            self.is_animating = False
            self.get_logger().info(f"Animation complete. Final angles: {[f'{angle:.2f}' for angle in self.current_angles]}")
            
            # Handle sequence progression
            self.handle_sequence_completion()
    
    def handle_sequence_completion(self):
        """Handle next step in the animation sequence based on current state"""
        if self.sequence_state == 1:
            # Just arrived at cube position (Point B)
            self.get_logger().info("Arrived at cube position (Point B)")
            
            # Pick up the cube
            self.is_cube_picked_up = True
            self.publish_state(1)  # State 1 = cube is picked up
            
            # Move to paper position (Point A)
            self.sequence_state = 2
            self.start_movement_to_point(self.paper_position)
            
        elif self.sequence_state == 2:
            # Just arrived at paper position (Point A)
            self.get_logger().info("Arrived at paper position (Point A)")
            
            # Drop the cube
            self.is_cube_picked_up = False
            self.publish_state(0)  # State 0 = cube is not picked up
            
            # Move back to home position
            self.sequence_state = 3
            self.start_animation(self.home_angles)
            
        elif self.sequence_state == 3:
            # Returned to home position
            self.get_logger().info("Returned to home position")
            
            # Reset sequence state
            self.sequence_state = 0
            
            # Reset paper and cube positions for next sequence
            self.paper_position = None
            self.cube_position = None
    
    def ease_in_out(self, t):
        """Simple quadratic ease-in-out function for smoother motion"""
        if t < 0.5:
            return 2 * t * t
        else:
            return -1 + (4 - 2 * t) * t
    
    def publish_joint_state(self, joint_angles):
        """Publish joint state message to control the robot arm"""
        joint_state_msg = JointState()
        joint_state_msg.header.stamp = self.get_clock().now().to_msg()
        
        # Joint names must match the joint names in your URDF
        joint_state_msg.name = ["Base", "Rear_Arm", "Forearm", "End-effector_rotation", "Gripper_rotation"]
        joint_state_msg.position = joint_angles
        
        # Publish the joint states
        self.joint_states_pub.publish(joint_state_msg)
    
    def calculate_end_effector_position(self, joint_angles):
        """Calculate the end effector position using forward kinematics"""
        theta1 = joint_angles[0]
        theta2 = joint_angles[1]
        theta3 = joint_angles[2]
        theta4 = joint_angles[3]
        
        # Transform matrices for each joint
        # Base to joint 1
        T01 = np.array([
            [np.cos(theta1), -np.sin(theta1), 0, 0],
            [np.sin(theta1), np.cos(theta1), 0, 0],
            [0, 0, 1, self.base_height],
            [0, 0, 0, 1]
        ])
        
        # Joint 1 to joint 2
        T12 = np.array([
            [np.cos(theta2), 0, np.sin(theta2), 0],
            [0, 1, 0, 0],
            [-np.sin(theta2), 0, np.cos(theta2), self.link1_length],
            [0, 0, 0, 1]
        ])
        
        # Joint 2 to joint 3
        T23 = np.array([
            [np.cos(theta3 + np.pi/2), 0, np.sin(theta3 + np.pi/2), 0],
            [0, 1, 0, 0],
            [-np.sin(theta3 + np.pi/2), 0, np.cos(theta3 + np.pi/2), self.link2_length],
            [0, 0, 0, 1]
        ])
        
        # Joint 3 to joint 4
        T34 = np.array([
            [np.cos(theta4), 0, np.sin(theta4), 0],
            [0, 1, 0, 0],
            [-np.sin(theta4), 0, np.cos(theta4), self.link3_length],
            [0, 0, 0, 1]
        ])
        
        # Joint 4 to end effector
        T45 = np.array([
            [1, 0, 0, 0],
            [0, 1, 0, 0],
            [0, 0, 1, self.link4_length + self.link5_length],
            [0, 0, 0, 1]
        ])
        
        # Calculate the final transformation
        T05 = T01 @ T12 @ T23 @ T34 @ T45
        
        # Extract the position
        position = T05[0:3, 3]
        
        return position
    
    def publish_end_effector_pose(self, position):
        """Publish the end effector pose for the cube to follow"""
        pose_msg = PoseStamped()
        pose_msg.header.stamp = self.get_clock().now().to_msg()
        pose_msg.header.frame_id = "base_link"
        
        pose_msg.pose.position.x = position[0]
        pose_msg.pose.position.y = position[1]
        pose_msg.pose.position.z = position[2]
        
        # Set orientation to identity quaternion
        pose_msg.pose.orientation.x = 0.0
        pose_msg.pose.orientation.y = 0.0
        pose_msg.pose.orientation.z = 0.0
        pose_msg.pose.orientation.w = 1.0
        
        self.pose_pub.publish(pose_msg)
    
    def publish_state(self, state):
        """Publish the state of the cube (0 = not picked up, 1 = picked up)"""
        state_msg = Int32()
        state_msg.data = state
        self.state_pub.publish(state_msg)
        self.get_logger().info(f"Published state: {state}")
    
    def inverse_kinematics(self, x, y, z, l1, l2, l3, l4, lb):
        dx = np.sqrt(x**2 + y**2)
        alpha = np.arctan((l4+z-lb)/dx)
        d = np.sqrt(dx**2 + (l4+z-lb-l1)**2)
        theta1 = np.arctan2(y, x)
        if d > (l2 + l3):
            self.get_logger().error("Target is unreachable")
            return None
        gamma = np.arccos((l2**2 + d**2 - l3**2)/(2*l2*d))
        theta2 = np.pi/2 - alpha - gamma
        theta3 = np.pi-np.arccos((l2**2 + l3**2 - d**2)/(2*l2*l3))
        theta4=np.pi-theta2-theta3
        theta1 = np.arctan2(y, x)
        if theta1 < self.base_limit_lower or theta1 > self.base_limit_upper:
            self.get_logger().error("Base joint angle out of limits")
            return None
        if theta2 < self.link1_limit_lower or theta2 > self.link1_limit_upper:
            self.get_logger().error("Link 1 joint angle out of limits")
            return None
        if theta3 < self.link2_limit_lower or theta3 > self.link2_limit_upper:
            self.get_logger().error("Link 2 joint angle out of limits")
            return None
        if theta4 < self.link3_limit_lower or theta4 > self.link3_limit_upper:
            self.get_logger().error("Link 3 joint angle out of limits")
            return None

        return [theta1, theta2, theta3, theta4, 0.0]


def main(args=None):
    rclpy.init(args=args)
    node = ReverseKinNode()
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
