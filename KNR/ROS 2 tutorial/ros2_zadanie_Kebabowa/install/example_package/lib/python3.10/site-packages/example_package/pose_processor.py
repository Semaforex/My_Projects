import rclpy
from rclpy.node import Node

from geometry_msgs.msg import Pose
from std_msgs.msg import String

import math
from tf_transformations import euler_from_quaternion

class PoseProcessor(Node):

    def __init__(self):
        super().__init__('pose_processor')
        self.subscription = self.create_subscription(
            Pose,
            'myPose',
            self.pose_callabck,
            10)
        self.publisher = self.create_publisher(String, 'dir', 10)

    def pose_callabck(self, msg):
        q = msg.orientation

        _, _, yaw = euler_from_quaternion([q.x, q.y, q.z, q.w])

        if -math.pi/2 < yaw < math.pi/2:
            dir_msg = String()
            dir_msg.data = 'Facing East'
            dir_msg.data += f" | Position: x={msg.position.x:.2f}, y={msg.position.y:.2f}, z={msg.position.z:.2f}"
            self.publisher.publish(dir_msg)

def main(args=None):
    rclpy.init(args=args)

    pose_processor = PoseProcessor()

    rclpy.spin(pose_processor)

    pose_processor.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
