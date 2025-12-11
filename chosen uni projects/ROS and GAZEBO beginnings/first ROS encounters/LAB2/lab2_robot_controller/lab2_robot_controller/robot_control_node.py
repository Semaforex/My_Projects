import rclpy
from rclpy.node import Node
from dobot_msgs.srv import GripperControl
from rclpy.action import ActionClient
from dobot_msgs.action import PointToPoint
import time

class RobotControlNode(Node):

    def __init__(self):
        super().__init__('robot_control_node')
        self.declare_parameter('tower_height', 1)
        self.tower_height = self.get_parameter('tower_height').value

        self._move_action_client = ActionClient(self, PointToPoint, 'PTP_action')
        self._gripper_service = self.create_client(GripperControl, 'dobot_gripper_service')

        while not self._gripper_service.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('Service not available, waiting again...')

    def move_dobot(self, target_pose, motion_type=1, velocity_ratio=0.5, acceleration_ratio=0.3):
        """Moves the Dobot arm to a target position and waits for completion."""
        goal_msg = PointToPoint.Goal()
        goal_msg.motion_type = motion_type
        goal_msg.target_pose = target_pose
        goal_msg.velocity_ratio = velocity_ratio
        goal_msg.acceleration_ratio = acceleration_ratio
        
        self.get_logger().info(f'Moving Dobot to {target_pose}...')
        
        self._move_action_client.wait_for_server()
        future = self._move_action_client.send_goal_async(goal_msg)
        rclpy.spin_until_future_complete(self, future)
        goal_handle = future.result()
        
        if not goal_handle.accepted:
            self.get_logger().error("Movement goal rejected!")
            return
        
        self.get_logger().info("Movement goal accepted, waiting for result...")
        result_future = goal_handle.get_result_async()
        rclpy.spin_until_future_complete(self, result_future)
        self.get_logger().info("Movement completed.")

    def control_gripper(self, gripper_state, keep_compressor_running=True):
        """Controls the Dobot gripper to open or close."""
        if not self._gripper_service.wait_for_service(timeout_sec=2.0):
            self.get_logger().error("Gripper service not available!")
            return
        
        request = GripperControl.Request()
        request.gripper_state = gripper_state
        request.keep_compressor_running = keep_compressor_running
        
        self.get_logger().info(f'Sending gripper command: {gripper_state}')
        future = self._gripper_service.call_async(request)
        rclpy.spin_until_future_complete(self, future)
        response = future.result()
        
        if response.success:
            self.get_logger().info(f'Gripper action successful: {response.message}')
        else:
            self.get_logger().error(f'Gripper action failed: {response.message}')

    def build_tower(self):
        """Builds a tower of height `self.tower_height`."""
        block_height = 20
        initial_heigth = 2.4+block_height*(self.tower_height-1)
        pick_x, pick_y, pick_z, pick_r = 167.1, 78.6, initial_heigth, 25.4
        place_x, place_y, place_z, place_r = 167, -63, 2.4, -20 
        pick_start_position = [pick_x, pick_y, pick_z, pick_r]  
        place_start_position = [place_x, place_y, place_z, place_r]
        travel_height = 110.0  
        pick_high_position = [pick_x, pick_y, travel_height, pick_r]  
        place_high_position = [place_x, place_y, travel_height, place_r]  
        block_height = 20
        self.control_gripper('open')
        for i in range(self.tower_height):
            place_position = place_start_position.copy()
            pick_position = pick_start_position.copy()
            place_position[2] += i * block_height
            pick_position[2] -= i * block_height
            
            
            # Pick block
            self.move_dobot(pick_high_position)
            self.move_dobot(pick_position)
            self.control_gripper('close')
            time.sleep(1)  
            self.move_dobot(pick_high_position)
            
            self.move_dobot(place_high_position)
            
            # Place block
            self.move_dobot(place_position)
            self.control_gripper('open', keep_compressor_running=False)
            time.sleep(1)  
            
            # Move back up hi
            self.move_dobot(place_high_position)
        
        self.get_logger().info("Tower construction complete!")


def main(args=None):
    rclpy.init(args=args)
    dobot_controller = RobotControlNode()
    dobot_controller.build_tower()
    rclpy.shutdown()

if __name__ == '__main__':
    main()