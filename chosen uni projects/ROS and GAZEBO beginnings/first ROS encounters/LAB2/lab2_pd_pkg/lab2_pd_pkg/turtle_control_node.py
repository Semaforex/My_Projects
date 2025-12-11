import rclpy
from rclpy.node import Node
from turtlesim.action import RotateAbsolute
from turtlesim.srv import Spawn
from rclpy.action import ActionClient
import math

class TurtleControlNode(Node):

    def __init__(self):
        super().__init__('turtle_control_node')
        self.declare_parameters(
            namespace='turtle_names',
            parameters=[
                ('turtle2_name', 'turtle2'),
                ('turtle3_name', 'turtle3')
            ]
        )

        self._action_client = ActionClient(self, RotateAbsolute, 'turtle1/rotate_absolute')
        self._client = self.create_client(Spawn, 'spawn')

        while not self._client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('Service not available, waiting again...')

    def rotate_turtle(self, theta):
        """Rotates Turtle1 by a given angle and waits for completion."""
        goal_msg = RotateAbsolute.Goal()
        goal_msg.theta = theta

        self.get_logger().info(f'Rotating Turtle1 by {theta} radians...')
        future = self._action_client.send_goal_async(goal_msg)
        rclpy.spin_until_future_complete(self, future)
        goal_handle = future.result()

        if not goal_handle.accepted:
            self.get_logger().error("Rotation goal rejected!")
            return
        
        self.get_logger().info("Rotation goal accepted, waiting for result...")
        result_future = goal_handle.get_result_async()
        rclpy.spin_until_future_complete(self, result_future)
        self.get_logger().info("Rotation completed.")

    def spawn_turtle(self, x, y, turtle_num):
        """Spawns a new turtle at (x, y) and waits for completion."""
        request = Spawn.Request()
        request.x = x
        request.y = y
        request.theta = 0.0
        request.name = self.get_parameter(f'turtle_names.turtle{turtle_num}_name').get_parameter_value().string_value

        self.get_logger().info(f'Spawning Turtle{turtle_num} at ({request.x}, {request.y})...')
        future = self._client.call_async(request)
        rclpy.spin_until_future_complete(self, future)
        self.get_logger().info(f'Turtle{turtle_num} spawned successfully.')

def main(args=None):
    rclpy.init(args=args)
    turtle_control_node = TurtleControlNode()

    # Sequential execution
    turtle_control_node.rotate_turtle(math.pi - 1e-6)  # Rotate first
    turtle_control_node.spawn_turtle(10.0, 10.0, 2)    # Spawn second turtle
    turtle_control_node.rotate_turtle(math.pi / 4)     # Rotate again
    turtle_control_node.rotate_turtle(-math.pi / 2)    # Another rotation
    turtle_control_node.spawn_turtle(5.0, 10.0, 3)     # Spawn third turtle

    rclpy.shutdown()

if __name__ == '__main__':
    main()
