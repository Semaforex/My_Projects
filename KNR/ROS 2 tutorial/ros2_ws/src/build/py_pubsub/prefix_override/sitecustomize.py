import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/mparadow/Desktop/ROS2_tutorial/ros2_ws/src/install/py_pubsub'
