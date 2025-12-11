#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"

// Klasa węzła dziedzicząca po rclcpp::Node
class OdomInfo : public rclcpp::Node {
public:
    OdomInfo() : Node("odometry_info") {
        
        RCLCPP_INFO(this->get_logger(), "Wezel 'odometry_info' zostal uruchomiony.");
        RCLCPP_INFO(this->get_logger(), "Oczekiwanie na dane z tematu /mobile_base_controller/odom...");

        // Utworzenie subskrybenta
        // Typ wiadomości: nav_msgs::msg::Odometry
        // Temat: /mobile_base_controller/odom
        subscription_ = this->create_subscription<nav_msgs::msg::Odometry>(
            "/mobile_base_controller/odom", 
            10, 
            std::bind(&OdomInfo::odom_callback, this, std::placeholders::_1)
        );
    }

private:
    void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg) const {
        // Pobranie współrzędnych pozycji (X, Y)
        double position_x = msg->pose.pose.position.x;
        double position_y = msg->pose.pose.position.y;

        // Pobranie orientacji (Kwaterniony Z, W - uproszczony podgląd obrotu)
        double orientation_z = msg->pose.pose.orientation.z;
        double orientation_w = msg->pose.pose.orientation.w;

        RCLCPP_INFO(this->get_logger(), 
            "Pozycja [X: %.3f, Y: %.3f] | Orientacja (quat) [Z: %.3f, W: %.3f]", 
            position_x, position_y, orientation_z, orientation_w);
    }

    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscription_;
};


int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    
    rclcpp::spin(std::make_shared<OdomInfo>());
    
    rclcpp::shutdown();
    return 0;
}