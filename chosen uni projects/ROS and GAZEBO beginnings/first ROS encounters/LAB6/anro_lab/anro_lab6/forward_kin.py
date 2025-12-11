#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import PoseStamped
from sensor_msgs.msg import JointState
from math import cos, sin, sqrt
from numpy import matrix


class ForwardKinNode(Node):
    def __init__(self):
        super().__init__("forward_kin")
        self.publisher = self.create_publisher(PoseStamped, "pose", 10)
        self.subscriber = self.create_subscription(
            JointState, "/joint_states", self.callback, 10
        )
        self.position = [0, 0, 0, 0]
        self.rotation = [0, 0, 0, 0]
        self.timer = self.create_timer(0.05, self.timer_callback)

    def callback(self, msg):

        return
        
        

    def timer_callback(self):
        msg = PoseStamped()
        msg.pose.position.x = 0.0
        msg.pose.position.y = 0.0
        msg.pose.position.z = 0.0
        msg.pose.orientation.x = 0.0
        msg.pose.orientation.y = 0.0
        msg.pose.orientation.z = 0.0
        msg.pose.orientation.w = 1.0
        msg.header.stamp.sec = self.get_clock().now().to_msg().sec
        msg.header.stamp.nanosec = self.get_clock().now().to_msg().nanosec
        msg.header.frame_id = "gripper_end"
        self.publisher.publish(msg)


def main(arg=None):
    rclpy.init(args=arg)
    node = ForwardKinNode()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
