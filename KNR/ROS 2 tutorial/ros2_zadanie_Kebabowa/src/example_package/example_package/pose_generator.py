import rclpy
from rclpy.node import Node
import random

from geometry_msgs.msg import Pose

class PoseGenerator(Node):

    def __init__(self):
        super().__init__('pose_generator')
        self.publisher_ = self.create_publisher(Pose, 'myPose', 10)
        timer_period = 0.5
        self.timer = self.create_timer(timer_period, self.timer_callback)

    def timer_callback(self):
        msg = Pose()
        (x, y, z) = (random.random(), random.random(), random.random())
        msg.position.x = x
        msg.position.y = y
        msg.position.z = z

        (x, y, z, w) = (random.random(), random.random(), random.random(), random.random())
        norm = (x**2 + y**2 + z**2 + w**2)**0.5
        msg.orientation.x = x / norm
        msg.orientation.y = y / norm
        msg.orientation.z = z / norm
        msg.orientation.w = w / norm
        self.publisher_.publish(msg)


def main(args=None):
    rclpy.init(args=args)

    pose_generator = PoseGenerator()

    rclpy.spin(pose_generator)

    pose_generator.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()

