import rclpy
from rclpy.node import Node
from std_msgs.msg import UInt8MultiArray
import cv2
import torch
import torchvision.transforms as T
from PIL import Image
from compressai.zoo import bmshj2018_hyperprior
import pickle
import numpy as np
import time

class CompressorNode(Node):
    def __init__(self):
        super().__init__('compressor_node')
        self.publisher_ = self.create_publisher(UInt8MultiArray, 'compressed_image_stream', 10)
        
        # Declare parameters
        self.declare_parameter('fps', 1.0) # Default 1 FPS as requested "FPS i can set myself"
        self.fps = self.get_parameter('fps').get_parameter_value().double_value
        
        self.timer = self.create_timer(1.0 / self.fps, self.timer_callback)
        
        # Initialize Camera
        self.cap = cv2.VideoCapture(0)
        if not self.cap.isOpened():
             self.get_logger().error("Cannot open camera")
        
        # Initialize AI Model
        self.device = 'cuda' if torch.cuda.is_available() else 'cpu'
        self.quality = 1 # Using quality 1 as in original script
        self.net = bmshj2018_hyperprior(quality=self.quality, pretrained=True).eval().to(self.device)
        self.transform = T.Compose([
            T.Resize((256, 256)), # Based on IN_SIZE from original script
            T.ToTensor()
        ])
        
        self.get_logger().info(f"Compressor Node Started on {self.device}. FPS: {self.fps}")

    def timer_callback(self):
        ret, frame = self.cap.read()
        if not ret:
            self.get_logger().warning("Can't receive frame (stream end?). Exiting ...")
            return

        # Convert BGR (OpenCV) to RGB (PIL/Torch)
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        pil_img = Image.fromarray(frame_rgb)
        
        # Preprocess
        x = self.transform(pil_img).unsqueeze(0).to(self.device)
        
        with torch.no_grad():
            start_time = time.time()
            out_comp = self.net.compress(x)
            
            # Serialize the compressed data (dictionary with strings and shape)
            # The tensor strings are on CPU already if I recall correctly, but ensure they are picklable
            # out_comp contains 'strings' (list of bytes) and 'shape' (torch.Size or tuple)
            serialized_data = pickle.dumps(out_comp)
            
            msg = UInt8MultiArray()
            # data in UInt8MultiArray is a list of integers, so convert bytes to list
            msg.data = list(serialized_data)
            
            self.publisher_.publish(msg)
            
            comp_time = time.time() - start_time
            size_kb = len(serialized_data) / 1024.0
            self.get_logger().info(f"Published frame. Size: {size_kb:.2f} KB. Time: {comp_time:.4f}s")

    def __del__(self):
        if self.cap.isOpened():
            self.cap.release()

def main(args=None):
    rclpy.init(args=args)
    node = CompressorNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
