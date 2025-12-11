import rclpy
from rclpy.node import Node
from dobot_msgs.srv import GripperControl
from rclpy.action import ActionClient
from dobot_msgs.action import PointToPoint
from geometry_msgs.msg import PointStamped
import time

class RobotControlNode(Node):
    def __init__(self):
        super().__init__("robot_control")  
        self.pose_sub = self.create_subscription(
            PointStamped, "/clicked_point", self.point_callback, 10
        )
    
        self._move_action_client = ActionClient(self, PointToPoint, 'PTP_action')


    def move_dobot(self, target_pose, motion_type=1, velocity_ratio=0.5, acceleration_ratio=0.3, timeout_sec=5.0):
        """Moves the Dobot arm to a target position and waits for completion."""
        goal_msg = PointToPoint.Goal()
        goal_msg.motion_type = motion_type
        goal_msg.target_pose = target_pose
        goal_msg.velocity_ratio = velocity_ratio
        goal_msg.acceleration_ratio = acceleration_ratio
        
        self.get_logger().info(f'Moving Dobot to {target_pose}...')
        
        server_ready = self._move_action_client.wait_for_server(timeout_sec=0.2)
        if not server_ready:
            self.get_logger().error("Action server not available after timeout!")
            return False
        
        future = self._move_action_client.send_goal_async(goal_msg)
        
        if not rclpy.spin_until_future_complete(self, future, timeout_sec=timeout_sec):
            self.get_logger().error(f"Goal acceptance timed out after {timeout_sec} seconds!")
            return False
        
        goal_handle = future.result()
        if not goal_handle.accepted:
            self.get_logger().error("Movement goal rejected!")
            return False
        
        self.get_logger().info("Movement goal accepted, waiting for result...")
        
        result_future = goal_handle.get_result_async()
        if not rclpy.spin_until_future_complete(self, result_future, timeout_sec=timeout_sec):
            self.get_logger().error(f"Movement execution timed out after {timeout_sec} seconds!")
            return False
        
        self.get_logger().info("Movement completed.")
        return True
    

    def point_callback(self, msg):
        """Callback for the point subscriber."""
        x, y, z, r = 1000*msg.point.x, 1000*msg.point.y, 1000*msg.point.z, 0
        success = self.move_dobot([x, y, z, r])
        if not success:
            self.get_logger().warn("Movement did not complete successfully")


def main(args=None):
    rclpy.init(args=args)
    dobot_controller = RobotControlNode()
    rclpy.spin(dobot_controller)  
    dobot_controller.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()