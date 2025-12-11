import rclpy
from rclpy.node import Node
from geometry_msgs.msg import PoseStamped
from sensor_msgs.msg import JointState
import numpy as np
import yaml
import os


class ForwardKin(Node):

    def __init__(self):
        super().__init__("forward_kinematics")

        # Create subscription to joint states
        self.joint_states_sub = self.create_subscription(
            JointState, "/joint_states", self.joint_states_callback, 10
        )

        # Create publisher for end-effector pose
        self.pose_pub = self.create_publisher(
            PoseStamped, "/end_effector_pose", 10
        )

        # Load parameters from robot_params.yaml
        self.load_robot_params()

        self.get_logger().info(
            f"Forward Kinematics initialized with parameters:\n"
            f"base_height: {self.base_height}\n"
            f"link1_length: {self.link1_length}\n"
            f"link2_length: {self.link2_length}\n"
            f"link3_length: {self.link3_length}\n"
            f"link4_length: {self.link4_length}\n"
            f"link5_length: {self.link5_length}"
        )

    def load_robot_params(self):
        try:
            # Find path to robot_params.yaml file
            pkg_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
            params_path = os.path.join(pkg_path, 'config', 'robot_params.yaml')
            
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
            
            # Joint limits (we might use these later)
            self.base_limit_lower = float(params.get('base_limit_lower', -1.57))
            self.base_limit_upper = float(params.get('base_limit_upper', 1.57))
            self.link1_limit_lower = float(params.get('link1_limit_lower', 0.0))
            self.link1_limit_upper = float(params.get('link1_limit_upper', 1.48))
            self.link2_limit_lower = float(params.get('link2_limit_lower', -0.17))
            self.link2_limit_upper = float(params.get('link2_limit_upper', 1.57))
            self.link3_limit_lower = float(params.get('link3_limit_lower', -1.57))
            self.link3_limit_upper = float(params.get('link3_limit_upper', 1.57))
            self.link4_limit_lower = float(params.get('link4_limit_lower', -1.57))
            self.link4_limit_upper = float(params.get('link4_limit_upper', 1.57))
            
        except Exception as e:
            self.get_logger().error(f"Failed to load parameters: {str(e)}")
            # Use default values if parameters can't be loaded
            self.base_height = 0.05
            self.link1_length = 0.088
            self.link2_length = 0.135
            self.link3_length = 0.147
            self.link4_length = 0.05
            self.link5_length = 0.02

    def rotation_to_quaternion(self, R):
        trace = R[0, 0] + R[1, 1] + R[2, 2]

        if trace > 0:
            S = np.sqrt(trace + 1.0) * 2
            qw = 0.25 * S
            qx = (R[2, 1] - R[1, 2]) / S
            qy = (R[0, 2] - R[2, 0]) / S
            qz = (R[1, 0] - R[0, 1]) / S
        elif R[0, 0] > R[1, 1] and R[0, 0] > R[2, 2]:
            S = np.sqrt(1.0 + R[0, 0] - R[1, 1] - R[2, 2]) * 2
            qw = (R[2, 1] - R[1, 2]) / S
            qx = 0.25 * S
            qy = (R[0, 1] + R[1, 0]) / S
            qz = (R[0, 2] + R[2, 0]) / S
        elif R[1, 1] > R[2, 2]:
            S = np.sqrt(1.0 + R[1, 1] - R[0, 0] - R[2, 2]) * 2
            qw = (R[0, 2] - R[2, 0]) / S
            qx = (R[0, 1] + R[1, 0]) / S
            qy = 0.25 * S
            qz = (R[1, 2] + R[2, 1]) / S
        else:
            S = np.sqrt(1.0 + R[2, 2] - R[0, 0] - R[1, 1]) * 2
            qw = (R[1, 0] - R[0, 1]) / S
            qx = (R[0, 2] + R[2, 0]) / S
            qy = (R[1, 2] + R[2, 1]) / S
            qz = 0.25 * S

        return np.array([qw, qx, qy, qz])

    def forward_kinematics(self, theta1, theta2, theta3, theta4, theta5):
        # Joint 1: Base rotation (around Z)
        T01 = np.array([
            [np.cos(theta1), -np.sin(theta1), 0, 0],
            [np.sin(theta1), np.cos(theta1), 0, 0],
            [0, 0, 1, self.base_height/2],
            [0, 0, 0, 1]
        ])

        # Joint 2: Rear_Arm (around Y)
        T12 = np.array([
            [np.cos(theta2), 0, np.sin(theta2), 0],
            [0, 1, 0, 0],
            [-np.sin(theta2), 0, np.cos(theta2), self.link1_length],
            [0, 0, 0, 1]
        ])

        # Joint 3: Forearm (around Y)
        T23 = np.array([
            [np.cos(theta3), 0, np.sin(theta3), 0],
            [0, 1, 0, 0],
            [-np.sin(theta3), 0, np.cos(theta3), self.link2_length],
            [0, 0, 0, 1]
        ])

        # Joint 4: End-effector_rotation (around Y)
        T34 = np.array([
            [np.cos(theta4), 0, np.sin(theta4), 0],
            [0, 1, 0, 0],
            [-np.sin(theta4), 0, np.cos(theta4), self.link3_length],
            [0, 0, 0, 1]
        ])

        # Joint 5: Gripper_rotation (around Z)
        T45 = np.array([
            [np.cos(theta5), -np.sin(theta5), 0, 0],
            [np.sin(theta5), np.cos(theta5), 0, 0],
            [0, 0, 1, self.link4_length],
            [0, 0, 0, 1]
        ])

        # Additional transform to the end of the gripper
        T5E = np.array([
            [1, 0, 0, 0],
            [0, 1, 0, 0],
            [0, 0, 1, self.link5_length],
            [0, 0, 0, 1]
        ])

        # Calculate the complete transformation
        T0E = T01 @ T12 @ T23 @ T34 @ T45 @ T5E
        
        return T0E

    def joint_states_callback(self, msg):
        # Extract joint angles from the message
        joint_positions = msg.position
        
        if len(joint_positions) < 5:
            self.get_logger().warn(
                f"Received only {len(joint_positions)} joint positions, need at least 5"
            )
            return

        # Extract joint angles
        theta1 = joint_positions[0]  # Base
        theta2 = joint_positions[1]  # Rear_Arm
        theta3 = joint_positions[2]  # Forearm
        theta4 = joint_positions[3]  # End-effector_rotation
        theta5 = joint_positions[4]  # Gripper_rotation

        # Calculate forward kinematics
        T0E = self.forward_kinematics(theta1, theta2, theta3, theta4, theta5)
        position = T0E[:3, 3]
        rotation = T0E[:3, :3]
        
        # Convert rotation matrix to quaternion
        quaternion = self.rotation_to_quaternion(rotation)

        # Create and publish PoseStamped message
        pose_msg = PoseStamped()
        pose_msg.header.stamp = self.get_clock().now().to_msg()
        pose_msg.header.frame_id = "base_link"

        # Set position
        pose_msg.pose.position.x = float(position[0])
        pose_msg.pose.position.y = float(position[1])
        pose_msg.pose.position.z = float(position[2])

        # Set orientation
        pose_msg.pose.orientation.w = float(quaternion[0])
        pose_msg.pose.orientation.x = float(quaternion[1])
        pose_msg.pose.orientation.y = float(quaternion[2])
        pose_msg.pose.orientation.z = float(quaternion[3])

        # Publish the pose
        self.pose_pub.publish(pose_msg)
        self.get_logger().info(
            f"End-effector position: ({position[0]:.3f}, {position[1]:.3f}, {position[2]:.3f})"
        )


def main(args=None):
    rclpy.init(args=args)
    node = ForwardKin()
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()