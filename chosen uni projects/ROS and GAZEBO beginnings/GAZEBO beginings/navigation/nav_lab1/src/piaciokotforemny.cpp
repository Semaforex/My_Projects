#include <chrono>
#include <cmath>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/twist.hpp"

#include "nav_msgs/msg/path.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

using namespace std::chrono_literals;

// Proste stany do maszyny stanów
enum State {
    INIT,
    MOVE_FORWARD,
    TURN,
    STOP
};

class PiaciokotForemny : public rclcpp::Node {
public:
    PiaciokotForemny() : Node("piaciokotforemny") {
        subscription_ = this->create_subscription<nav_msgs::msg::Odometry>(
            "/mobile_base_controller/odom", 
            10, 
            std::bind(&PiaciokotForemny::odom_callback, this, std::placeholders::_1)
        );

        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

        path_pub_ = this->create_publisher<nav_msgs::msg::Path>("/trajectory_path", 10);

        path_msg_.header.frame_id = "odom";


        // Timer sterujący
        timer_ = this->create_wall_timer(
            50ms, std::bind(&PiaciokotForemny::control_loop, this)
        );

        RCLCPP_INFO(this->get_logger(), "Wezel uruchomiony. Czekam na odometrie...");
    }

private:
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr path_pub_;  

    rclcpp::TimerBase::SharedPtr timer_;

    nav_msgs::msg::Odometry current_odom_;
    nav_msgs::msg::Path path_msg_;

    bool odom_received_ = false;

    State state_ = INIT;
    
    double start_x_ = 0.0;
    double start_y_ = 0.0;
    double start_yaw_ = 0.0;
    
    int sides_completed_ = 0;
    const int TOTAL_SIDES = 5;       
    const double SIDE_LENGTH = 1.0;  // metry
    const double TURN_ANGLE = (2.0 * M_PI) / TOTAL_SIDES;
    
    const double LINEAR_VEL = 0.2;
    const double ANGULAR_VEL = 0.3;

    void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg) {
        current_odom_ = *msg;
        odom_received_ = true;

        geometry_msgs::msg::PoseStamped pose_stamped;
        pose_stamped.header.stamp = msg->header.stamp;
        pose_stamped.header.frame_id = msg->header.frame_id;
        pose_stamped.pose = msg->pose.pose;

        path_msg_.header.stamp = msg->header.stamp;
        path_msg_.poses.push_back(pose_stamped);

        if (path_msg_.poses.size() > 1000) {
            path_msg_.poses.erase(path_msg_.poses.begin());
        }

        path_pub_->publish(path_msg_);
    }

    void control_loop() {
        if (!odom_received_) return;

        geometry_msgs::msg::Twist cmd_vel;
        double current_x = current_odom_.pose.pose.position.x;
        double current_y = current_odom_.pose.pose.position.y;
        double current_yaw = get_yaw_from_odom(current_odom_);

        switch (state_) {
            case INIT:
                start_x_ = current_x;
                start_y_ = current_y;
                state_ = MOVE_FORWARD;
                RCLCPP_INFO(this->get_logger(), "Start boku nr: %d", sides_completed_ + 1);
                break;

            case MOVE_FORWARD:
                {
                    double dist = std::sqrt(std::pow(current_x - start_x_, 2) + 
                                          std::pow(current_y - start_y_, 2));
                    
                    if (dist < SIDE_LENGTH) {
                        // Jedź prosto
                        cmd_vel.linear.x = LINEAR_VEL;
                        cmd_vel.angular.z = 0.0;
                    } else {
                        cmd_vel.linear.x = 0.0;
                        state_ = TURN;
                        start_yaw_ = current_yaw;
                        RCLCPP_INFO(this->get_logger(), "Bok zakonczony. Obracam sie...");
                    }
                }
                break;

            case TURN:
                {
                    double angle_diff = normalize_angle(current_yaw - start_yaw_);
                    
                    if (std::abs(angle_diff) < TURN_ANGLE) {
                        cmd_vel.linear.x = 0.0;
                        cmd_vel.angular.z = ANGULAR_VEL;
                    } else {
                        cmd_vel.angular.z = 0.0;
                        sides_completed_++;
                        
                        if (sides_completed_ >= TOTAL_SIDES) {
                            state_ = STOP;
                            RCLCPP_INFO(this->get_logger(), "Koniec zadania!");
                        } else {
                            start_x_ = current_x;
                            start_y_ = current_y;
                            state_ = MOVE_FORWARD;
                            RCLCPP_INFO(this->get_logger(), "Start boku nr: %d", sides_completed_ + 1);
                        }
                    }
                }
                break;

            case STOP:
                cmd_vel.linear.x = 0.0;
                cmd_vel.angular.z = 0.0;
                break;
        }

        publisher_->publish(cmd_vel);
    }

    // FUNKCJE POMOCNICZE
    
    double get_yaw_from_odom(const nav_msgs::msg::Odometry & odom) {
        double qx = odom.pose.pose.orientation.x;
        double qy = odom.pose.pose.orientation.y;
        double qz = odom.pose.pose.orientation.z;
        double qw = odom.pose.pose.orientation.w;

        double siny_cosp = 2 * (qw * qz + qx * qy);
        double cosy_cosp = 1 - 2 * (qy * qy + qz * qz);
        return std::atan2(siny_cosp, cosy_cosp);
    }

    double normalize_angle(double angle) {
        while (angle > M_PI) angle -= 2.0 * M_PI;
        while (angle < -M_PI) angle += 2.0 * M_PI;
        return angle;
    }
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PiaciokotForemny>());
    rclcpp::shutdown();
    return 0;
}