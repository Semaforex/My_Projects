import rclpy
from rclpy.node import Node
from geometry_msgs.msg import PointStamped
from sensor_msgs.msg import JointState
import numpy as np
import yaml
import os
from ament_index_python.packages import get_package_share_directory



class BackwardKin(Node):

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

        # Load parameters from robot_params.yaml
        self.load_robot_params()

        self.get_logger().info(
            f"Backward Kinematics initialized with parameters:\n"
            f"base_height: {self.base_height}\n"
            f"link1_length: {self.link1_length}\n"
            f"link2_length: {self.link2_length}\n"
            f"link3_length: {self.link3_length}\n"
            f"link4_length: {self.link4_length}\n"
            f"link5_length: {self.link5_length}"
        )

        self.angles = [0.00, 0.57, 1.50, 1.06, 0.00]
        joint_state_msg = JointState()
        joint_state_msg.header.stamp = self.get_clock().now().to_msg()
        
        # Joint names must match the joint names in your URDF
        joint_state_msg.name = ["Base", "Rear_Arm", "Forearm", "End-effector_rotation", "Gripper_rotation"]
        joint_state_msg.position = self.angles
        self.joint_states_pub.publish(joint_state_msg)
        

    def load_robot_params(self):
        # Keep your existing robot_params loading code
        try:
            # Find path to robot_params.yaml file
            pkg_share = get_package_share_directory('lab4_pd')
            params_path = os.path.join(pkg_share, 'config', 'robot_params.yaml')
            
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
            self.get_logger().warn("Using default parameters due to loading failure.")

    def point_callback(self, msg):
        # Extract position and orientation from the PoseStamped message
        x = msg.point.x
        y = msg.point.y
        z = msg.point.z
        
        # Log the received pose
        self.get_logger().info(f"Received end-effector pose: x={x:.3f}, y={y:.3f}, z={z:.3f}")
        
        # Calculate inverse kinematics (this is where you'll implement your IK solution)
        # For now, just using placeholders
        joint_angles = self.inverse_kinematics(x, y, z, self.link1_length, self.link2_length, self.link3_length, self.link4_length, self.base_height)
        if joint_angles is None:
            return
        
        # Create and publish JointState message
        joint_state_msg = JointState()
        joint_state_msg.header.stamp = self.get_clock().now().to_msg()
        
        # Joint names must match the joint names in your URDF
        joint_state_msg.name = ["Base", "Rear_Arm", "Forearm", "End-effector_rotation", "Gripper_rotation"]
        joint_state_msg.position = joint_angles
        
        # Publish the joint states
        self.joint_states_pub.publish(joint_state_msg)
        
        self.get_logger().info(f"Published joint angles: {[f'{angle:.2f}' for angle in joint_angles]}")
    
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
    node = BackwardKin()
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()