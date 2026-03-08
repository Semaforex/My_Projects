#!/home/mparadow/anaconda3/envs/compress_ai_env/bin/python3

import rclpy
from rclpy.node import Node
from std_msgs.msg import UInt8MultiArray
import cv2
import pickle
import numpy as np
import torch
from compressai.zoo import bmshj2018_hyperprior
import torchvision.transforms as T
from PIL import Image

class DecompressorNode(Node):
    def __init__(self):
        super().__init__('decompressor_node')
        self.subscription = self.create_subscription(
            UInt8MultiArray,
            'compressed_image_stream',
            self.listener_callback,
            10)
        self.subscription  # prevent unused variable warning
        
        # Initialize AI Model
        self.device = 'cuda' if torch.cuda.is_available() else 'cpu'
        self.quality = 1 # Using quality 1 as in original script
        self.net = bmshj2018_hyperprior(quality=self.quality, pretrained=True).eval().to(self.device)
        
        self.out_size = (1280, 720) # Target size for display
        
        self.get_logger().info(f"Decompressor Node Started on {self.device}")

    def listener_callback(self, msg):
        try:
            # Deserialize
            # msg.data is array('B') or list of ints
            data_bytes = bytes(msg.data)
            out_comp = pickle.loads(data_bytes)
            
            # Decompress
            with torch.no_grad():
                out_dec = self.net.decompress(out_comp['strings'], out_comp['shape'])
                
                # Post-process
                # Convert tensor back to PIL
                rec_img_tensor = out_dec['x_hat'].clamp(0, 1)[0].cpu()
                rec_img = T.ToPILImage()(rec_img_tensor)
                
                # Resize for display
                rec_img = rec_img.resize(self.out_size, Image.BICUBIC)
                
                # Convert to OpenCV format (BGR) for display
                img_cv = cv2.cvtColor(np.array(rec_img), cv2.COLOR_RGB2BGR)
                
                cv2.imshow("Decompressed Image", img_cv)
                cv2.waitKey(1)
                
                self.get_logger().info("Received and displayed frame.")
                
        except Exception as e:
            self.get_logger().error(f"Failed to decompress or display: {e}")

def main(args=None):
    rclpy.init(args=args)
    node = DecompressorNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        cv2.destroyAllWindows()
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
