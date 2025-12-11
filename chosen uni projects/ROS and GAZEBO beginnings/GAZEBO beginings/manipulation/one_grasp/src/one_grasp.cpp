#include <memory>
#include <chrono>
#include <cmath>  

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose.hpp>

// MoveIt
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit_visual_tools/moveit_visual_tools.h>
#include <rviz_visual_tools/rviz_visual_tools.hpp>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/msg/attached_collision_object.hpp>  // [CHANGE 1] added

// Gazebo
#include <gazebo_msgs/srv/get_entity_state.hpp>

// TF2
#include <tf2/LinearMath/Transform.h>
#include <tf2/exceptions.h>
#include <tf2/time.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>

using namespace std::chrono_literals;
namespace rvt = rviz_visual_tools;

static tf2::Transform tsToTransform(const geometry_msgs::msg::TransformStamped& ts) {
  tf2::Transform T;
  T.setOrigin(tf2::Vector3(
      ts.transform.translation.x,
      ts.transform.translation.y,
      ts.transform.translation.z));
  tf2::Quaternion q(
      ts.transform.rotation.x,
      ts.transform.rotation.y,
      ts.transform.rotation.z,
      ts.transform.rotation.w);
  T.setRotation(q);
  return T;
}

int main(int argc, char * argv[])
{
  // Initialize ROS and create the Node
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>(
    "one_grasp_node",
    rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true)
  );
  auto logger = node->get_logger();
  tf2_ros::Buffer tf_buffer(node->get_clock());
  tf2_ros::TransformListener tf_listener(tf_buffer, node, true);

  const std::string planning_frame = "base_footprint";
  const std::string ee_frame = "wrist_tf_link";
  const std::string gripper_frame = "arm_tool_link";
  const double pregrasp_distance = node->declare_parameter("pregrasp_distance", 0.05);
  const double cube_half_height = node->declare_parameter("cube_half_height", 0.035);
  const double approach_clearance = node->declare_parameter("approach_clearance", 0.30);
  bool execute_motion = false;
  if (!node->has_parameter("execute")) {
    execute_motion = node->declare_parameter<bool>("execute", false);
  } else {
    node->get_parameter("execute", execute_motion);
  }

  // --- MoveIt: grupa, jak w hello_moveit (jeśli masz arm_torso)
  using moveit::planning_interface::MoveGroupInterface;
  MoveGroupInterface move_group_interface(node, "arm_torso");
  move_group_interface.setPoseReferenceFrame(planning_frame);
  move_group_interface.setEndEffectorLink(ee_frame);
  move_group_interface.setMaxVelocityScalingFactor(0.2);
  move_group_interface.setMaxAccelerationScalingFactor(0.2);
  
  // --- MoveIt: grupa chwytaka/grippera
  MoveGroupInterface gripper_group(node, "gripper");
  gripper_group.setMaxVelocityScalingFactor(1.0);
  gripper_group.setMaxAccelerationScalingFactor(1.0);

  // --- VisualTools do RViz (ramę ustaw na swoją bazową: "world", "base_link", itp.)
  moveit_visual_tools::MoveItVisualTools visual_tools(
    node, "base_link", rvt::RVIZ_MARKER_TOPIC);
  visual_tools.deleteAllMarkers();
  visual_tools.loadRemoteControl();

  // --- Klient serwisu Gazebo /get_entity_state
  auto client = node->create_client<gazebo_msgs::srv::GetEntityState>("/get_entity_state");

  if (!client->wait_for_service(5s)) {
    RCLCPP_ERROR(logger, "Brak serwisu /get_entity_state (Gazebo).");
    rclcpp::shutdown();
    return 1;
  }

  auto req = std::make_shared<gazebo_msgs::srv::GetEntityState::Request>();
  req->name = "green_cube_3::link";
  req->reference_frame = planning_frame;

  auto future = client->async_send_request(req);
  auto ret = rclcpp::spin_until_future_complete(node, future, 5s);

  if (ret != rclcpp::FutureReturnCode::SUCCESS) {
    RCLCPP_ERROR(logger, "Nie udało się uzyskać odpowiedzi z /get_entity_state.");
    rclcpp::shutdown();
    return 1;
  }

  auto resp = future.get();
  if (!resp->success) {
    RCLCPP_ERROR(logger, "GetEntityState nie powiódł się: entity='%s'", req->name.c_str());
    rclcpp::shutdown();
    return 1;
  }

  const auto& p = resp->state.pose.position;
  const auto& o = resp->state.pose.orientation;

  RCLCPP_INFO(logger, "green_cube_3 position [x y z] = [%.4f %.4f %.4f]", p.x, p.y, p.z);
  RCLCPP_INFO(logger, "orientation [x y z w] = [%.4f %.4f %.4f %.4f]", o.x, o.y, o.z, o.w);

  // --- Pokaż w RViz (sfera + oś na pozycji kostki)
  geometry_msgs::msg::Pose cube_pose = resp->state.pose;

  // --- obliczanie pozycji blatu stołu

  req -> name = "table::link";
  req -> reference_frame = planning_frame;

  future = client -> async_send_request (req);
  ret = rclcpp::spin_until_future_complete (node, future, 5s);

  if (ret != rclcpp::FutureReturnCode::SUCCESS) {
    RCLCPP_ERROR (logger, "Nie udało się uzyskać odpowiedzi z /get_entity_state.");
    rclcpp::shutdown ();
    return 1;
  }

  resp = future.get ();
  if (!resp -> success) {
    RCLCPP_ERROR (logger, "GetEntityState nie powiódł się: entity='%s'", req -> name.c_str ());
    rclcpp::shutdown ();
    return 1;
  }

  const auto& table_pose = resp->state.pose;

  RCLCPP_INFO(logger, "table position [x y z] = [%.4f %.4f %.4f]", table_pose.position.x, table_pose.position.y, table_pose.position.z);
  
  // Save the original table pose in the planning frame (with your z offset)
    geometry_msgs::msg::Pose table_pose_world = table_pose;
    table_pose_world.position.z += 0.5; // you already add this later when creating the object

  // --- Oblicz pozycję i orientację końcówki roboczej dla chwytu i pre-chwytu

  if (!tf_buffer.canTransform(ee_frame, gripper_frame, tf2::TimePointZero, tf2::durationFromSec(5.0))) {
    RCLCPP_ERROR(logger, "Brak transformacji %s -> %s", gripper_frame.c_str(), ee_frame.c_str());
    rclcpp::shutdown();
    return 1;
  }

  geometry_msgs::msg::TransformStamped ee_to_gripper;
  try {
    ee_to_gripper = tf_buffer.lookupTransform(ee_frame, gripper_frame, tf2::TimePointZero);
  } catch (const tf2::TransformException & ex) {
    RCLCPP_ERROR(logger, "lookupTransform nie powiódł się: %s", ex.what());
    rclcpp::shutdown();
    return 1;
  }


  tf2::Transform cube_in_base;
  tf2::Transform e_to_gripper_tf;
  tf2::fromMsg(cube_pose, cube_in_base);
  tf2::fromMsg(ee_to_gripper.transform, e_to_gripper_tf);



  tf2::Transform cube_to_top_gripper(
    tf2::Quaternion(tf2::Vector3(0.0, 1.0, 0.0), M_PI),
    tf2::Vector3(0.0, 0.0, cube_half_height)
  );

  tf2::Transform cube_to_pregrasp_gripper(
    cube_to_top_gripper.getRotation(),
    cube_to_top_gripper.getOrigin() + tf2::Vector3(0.0, 0.0, approach_clearance)
  );

  tf2::Transform target_eef_in_base = cube_in_base * cube_to_top_gripper * e_to_gripper_tf.inverse();
  tf2::Transform pregrasp_eef_in_base = cube_in_base * cube_to_pregrasp_gripper * e_to_gripper_tf.inverse();

  geometry_msgs::msg::Pose target_eef_pose;
  const auto & origin = target_eef_in_base.getOrigin();
  const auto rotation = target_eef_in_base.getRotation();
  target_eef_pose.position.x = origin.x();
  target_eef_pose.position.y = origin.y();
  target_eef_pose.position.z = origin.z() + pregrasp_distance;
  target_eef_pose.orientation = tf2::toMsg(rotation);

    geometry_msgs::msg::Pose etf;
  const auto & origin2 = e_to_gripper_tf.getOrigin();
  const auto rotation2 = e_to_gripper_tf.getRotation();
  etf.position.x = origin2.x();
  etf.position.y = origin2.y();
  etf.position.z = origin2.z();
  etf.orientation = tf2::toMsg(rotation2);

  geometry_msgs::msg::Pose pregrasp_eef_pose;
  const auto & pre_origin = pregrasp_eef_in_base.getOrigin();
  const auto pre_rotation = pregrasp_eef_in_base.getRotation();
  pregrasp_eef_pose.position.x = pre_origin.x();
  pregrasp_eef_pose.position.y = pre_origin.y();
  pregrasp_eef_pose.position.z = pre_origin.z();
  pregrasp_eef_pose.orientation = tf2::toMsg(pre_rotation);

  const auto & target_pos = target_eef_pose.position;
  const auto & target_ori = target_eef_pose.orientation;
  const auto & eef_pos = etf.position;
  const auto & eef_or = etf.orientation;



  RCLCPP_INFO(logger, "EE target position [x y z] = [%.4f %.4f %.4f]", eef_pos.x, eef_pos.y, eef_pos.z);
  RCLCPP_INFO(logger, "EE target orientation [x y z w] = [%.4f %.4f %.4f %.4f]", eef_or.x, eef_or.y, eef_or.z, eef_or.w);
  RCLCPP_INFO(logger, "EE target position [x y z] = [%.4f %.4f %.4f]", target_pos.x, target_pos.y, target_pos.z);
  RCLCPP_INFO(logger, "EE target orientation [x y z w] = [%.4f %.4f %.4f %.4f]", target_ori.x, target_ori.y, target_ori.z, target_ori.w);
  RCLCPP_INFO(logger, "EE pregrasp position [x y z] = [%.4f %.4f %.4f]", pregrasp_eef_pose.position.x, pregrasp_eef_pose.position.y, pregrasp_eef_pose.position.z);
  RCLCPP_INFO(logger, "EE pregrasp orientation [x y z w] = [%.4f %.4f %.4f %.4f]", pregrasp_eef_pose.orientation.x, pregrasp_eef_pose.orientation.y, pregrasp_eef_pose.orientation.z, pregrasp_eef_pose.orientation.w);

  // --- Wizualizacja celu końcówki roboczej w RViz
  visual_tools.publishSphere(cube_pose, rvt::GREEN, rvt::LARGE, "green_cube_3");
  visual_tools.publishAxis(cube_pose, 0.15, 0.01);
  visual_tools.publishSphere(target_eef_pose, rvt::BLUE, rvt::SMALL, "ee_target_pose");
  visual_tools.publishAxis(target_eef_pose, 0.18, 0.015);
  visual_tools.publishSphere(pregrasp_eef_pose, rvt::YELLOW, rvt::SMALL, "ee_pregrasp_pose");
  visual_tools.publishAxis(pregrasp_eef_pose, 0.2, 0.02);
  visual_tools.trigger();

  if (!execute_motion) {
    RCLCPP_INFO(logger, "Visualization only. Set 'execute:=true' to run the motion sequence.");
    rclcpp::shutdown();
    return 0;
  }

  moveit::planning_interface::PlanningSceneInterface psi;
  std::vector<moveit_msgs::msg::CollisionObject> objects;

  moveit_msgs::msg::CollisionObject table;
  table.header.frame_id = planning_frame;
  table.id = "table";
  table.primitives.resize(1);
  table.primitives[0].type = shape_msgs::msg::SolidPrimitive::BOX;
  table.primitives[0].dimensions = {1, 0.8, 0.03};
  table.primitive_poses.resize(1);
  table.primitive_poses[0] = table_pose;
  table.primitive_poses[0].position.z += 0.5;
  table.operation = table.ADD;
  objects.push_back(table);

  moveit_msgs::msg::CollisionObject cube;
  cube.header.frame_id = planning_frame;
  cube.id = "green_cube_3";
  cube.primitives.resize(1);
  cube.primitives[0].type = shape_msgs::msg::SolidPrimitive::BOX;
  cube.primitives[0].dimensions = {0.07, 0.07, 0.07};
  cube.primitive_poses.resize(1);
  cube.primitive_poses[0] = cube_pose;
  cube.operation = cube.ADD;
  objects.push_back(cube);

  psi.addCollisionObjects(objects);
  rclcpp::sleep_for(500ms);

  const std::string left_joint  = node->declare_parameter("gripper_left_joint",  "gripper_left_finger_joint");
  const std::string right_joint = node->declare_parameter("gripper_right_joint", "gripper_right_finger_joint");
  const double open_val = node->declare_parameter("gripper_open_val", 0.044);  // jak na screenie
  const double closed_val = node->declare_parameter("gripper_closed_val", 0.031);

  {
    std::map<std::string, double> targets = {
      {left_joint,  open_val},
      {right_joint, open_val}
    };
    gripper_group.setJointValueTarget(targets);
    auto ec = gripper_group.move();  // plan + execute
    if (ec != moveit::core::MoveItErrorCode::SUCCESS) {
      RCLCPP_WARN(logger, "Nie udało się otworzyć chwytaka. Sprawdź nazwy stawów i limity.");
    } else {
      RCLCPP_INFO(logger, "Chwytak otwarty (%.3f m).", open_val);
    }
  }

  auto plan_and_execute = [&](const geometry_msgs::msg::Pose & goal, const std::string & label) {
    move_group_interface.setPoseTarget(goal, ee_frame);
    moveit::planning_interface::MoveGroupInterface::Plan plan;
    if (move_group_interface.plan(plan) != moveit::core::MoveItErrorCode::SUCCESS) {
      RCLCPP_WARN(logger, "Planowanie do %s nie powiodło się.", label.c_str());
      move_group_interface.clearPoseTargets();
      return false;
    }
    auto result = move_group_interface.execute(plan);
    move_group_interface.clearPoseTargets();
    if (result != moveit::core::MoveItErrorCode::SUCCESS) {
      RCLCPP_WARN(logger, "Wykonanie planu do %s nie powiodło się.", label.c_str());
      return false;
    }
    RCLCPP_INFO(logger, "Zrealizowano trajektorię do %s.", label.c_str());
    return true;
  };

  if (plan_and_execute(pregrasp_eef_pose, "pozycji pregrasp")) {
    if (plan_and_execute(target_eef_pose, "pozycji grasp")) {

      // [CHANGE 2] Attach the cube to the EE so MoveIt allows finger↔cube contact during closing
      {
        moveit_msgs::msg::AttachedCollisionObject aco;
        aco.link_name = ee_frame;                 // attach to your EE/palm link
        aco.object.id = "green_cube_3";           // must match the world object id
        aco.object.operation = moveit_msgs::msg::CollisionObject::ADD;

        moveit_msgs::msg::AttachedCollisionObject aco2;
        aco2.link_name = ee_frame;                 // attach to your EE/palm link
        aco2.object.id = "table";           // must match the world object id
        aco2.object.operation = moveit_msgs::msg::CollisionObject::ADD;

        

        // Allow collisions with the gripper links (fingers/palm)
        for (const auto &ln : gripper_group.getLinkNames()){
          aco.touch_links.push_back(ln);
          aco2.touch_links.push_back(ln);
        }

        psi.applyAttachedCollisionObject(aco);
        psi.applyAttachedCollisionObject(aco2);
        rclcpp::sleep_for(200ms);
        move_group_interface.setStartStateToCurrentState();
        RCLCPP_INFO(logger, "Attached 'green_cube_3' to %s with touch_links=gripper.", ee_frame.c_str());
      }

      std::map<std::string, double> targets = {
        {left_joint,  closed_val},
        {right_joint, closed_val}
      };
      gripper_group.setJointValueTarget(targets);
      auto ec = gripper_group.move();
      if (ec != moveit::core::MoveItErrorCode::SUCCESS) {
        RCLCPP_WARN(logger, "Nie udało się zamknąć chwytaka.");
      } else {
        RCLCPP_INFO(logger, "Chwytak zamknięty (%.3f m).", closed_val);
//         std::string object_id = "green_cube_3";   // ID obiektu w planning scene
//         std::string link_name = ee_frame;         // np. "arm_tool_link" lub "gripper_palm_link"

// // Lista linków, które mogą dotykać obiektu (np. palce, sama palma)
//         std::vector<std::string> touch_links = gripper_group.getLinkNames();

// // Attach
//         move_group_interface.attachObject(object_id, link_name, touch_links);
        geometry_msgs::msg::Pose lift_pose = target_eef_pose;
        lift_pose.position.z += 0.05;  // +1 cm
        if (!plan_and_execute(lift_pose, "podniesienia 5 cm")) {
          RCLCPP_WARN(logger, "Podniesienie 5 cm nie powiodło się.");
        }
        else{
        
            // Detach
            std::string object_id = "table";   // ID obiektu w planning scene
            move_group_interface.detachObject(object_id);
            rclcpp::sleep_for(100ms);
            move_group_interface.setStartStateToCurrentState();
            moveit_msgs::msg::CollisionObject table_reset;
            table_reset.header.frame_id = planning_frame;
            table_reset.id = "table";
            table_reset.primitives.resize(1);
            table_reset.primitives[0].type = shape_msgs::msg::SolidPrimitive::BOX;
            table_reset.primitives[0].dimensions = {1, 0.8, 0.03};
            table_reset.primitive_poses.resize(1);
            table_reset.primitive_poses[0] = table_pose_world;  // <- the saved original pose
            table_reset.operation = table_reset.ADD;

psi.applyCollisionObject(table_reset);
rclcpp::sleep_for(100ms);
RCLCPP_INFO(logger, "Table reset to its original world pose.");
            RCLCPP_INFO(logger, "Detached 'table' from %s.", ee_frame.c_str());
            RCLCPP_WARN(logger, "Podniesienie 5 cm nad stół powiodło się.");
          }
        }
      } else {
        RCLCPP_WARN(logger, "Nie dojechałem do grasp — nie zaciskam chwytaka.");
      }
  }

  rclcpp::shutdown();
  return 0;
}
  //       RCLCPP_WARN(logger, "Nie dojechałem do grasp — nie zaciskam chwytaka.");
  //     }
  // }