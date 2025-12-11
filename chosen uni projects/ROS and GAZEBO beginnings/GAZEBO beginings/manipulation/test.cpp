#include <memory>
#include <chrono>
#include <cmath>  // filepath: /home/mateo/Desktop/STERO/stero_25z_mp_ms/pick_place/src/pick_place.cpp

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

int main(int argc, char * argv[])
{
  // Initialize ROS and create the Node
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>(
    "pick_place_node",
    rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true)
  );
  auto logger = node->get_logger();
  tf2_ros::Buffer tf_buffer(node->get_clock());
  tf2_ros::TransformListener tf_listener(tf_buffer, node, true);

  const std::string planning_frame = "base_footprint";
  const std::string ee_frame = "wrist_ft_link";
  const std::string gripper_frame = "arm_tool_link";
  const double pregrasp_distance = node->declare_parameter("pregrasp_distance", 0.2);
  const double cube_half_height = node->declare_parameter("cube_half_height", 0.035);
  const double approach_clearance = node->declare_parameter("approach_clearance", 0.3);
  const double open_val = node->declare_parameter("gripper_open_val", 0.044);  
  const double closed_val = node->declare_parameter("gripper_closed_val", 0.031);
  const double arm_velocity_scale = node->declare_parameter("arm_max_velocity_scale", 0.9);
  const double arm_acceleration_scale = node->declare_parameter("arm_max_acceleration_scale", 0.9);
  const std::vector<double> approach_angles = node->declare_parameter("approach_angles", std::vector<double>{M_PI_2/2});
  const std::vector<double> approach_rotations = node->declare_parameter("approach_rotations", std::vector<double>{0}); //, M_PI_2, 3*M_PI_2
  bool execute_motion = false;
  if (!node->has_parameter("execute")) {
    execute_motion = node->declare_parameter<bool>("execute", false);
  } else {
    node->get_parameter("execute", execute_motion);
  }

  RCLCPP_INFO(
    logger,
    "Parametr 'execute' = %s (false = tylko planowanie, true = planowanie + wykonanie).",
    execute_motion ? "true" : "false"
  );


  // --- MoveIt: grupa, jak w hello_moveit
  using moveit::planning_interface::MoveGroupInterface;
  MoveGroupInterface move_group_interface(node, "arm_torso");
  move_group_interface.setPoseReferenceFrame(planning_frame);
  move_group_interface.setEndEffectorLink(ee_frame);
  move_group_interface.setMaxVelocityScalingFactor(arm_velocity_scale);
  move_group_interface.setMaxAccelerationScalingFactor(arm_acceleration_scale);
  
  // --- MoveIt: grupa chwytaka/grippera
  MoveGroupInterface gripper_group(node, "gripper");
  gripper_group.setMaxVelocityScalingFactor(1.0);
  gripper_group.setMaxAccelerationScalingFactor(1.0);

  // --- VisualTools do RViz
  moveit_visual_tools::MoveItVisualTools visual_tools(
    node, planning_frame, rvt::RVIZ_MARKER_TOPIC);
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

  // --- obliczanie pozycji blatu stołu --------------------------------------
  req->name = "table::link";
  req->reference_frame = planning_frame;

  future = client->async_send_request(req);
  ret = rclcpp::spin_until_future_complete(node, future, 5s);

  if (ret != rclcpp::FutureReturnCode::SUCCESS) {
    RCLCPP_ERROR(logger, "Nie udało się uzyskać odpowiedzi z /get_entity_state (table).");
    rclcpp::shutdown();
    return 1;
  }

  resp = future.get();
  if (!resp->success) {
    RCLCPP_ERROR(logger, "GetEntityState (table) nie powiódł się: '%s'", req->name.c_str());
    rclcpp::shutdown();
    return 1;
  }

  const auto & table_pose = resp->state.pose;
  RCLCPP_INFO(
    logger, "table position [x y z] = [%.4f %.4f %.4f]",
    table_pose.position.x, table_pose.position.y, table_pose.position.z);

  // Zapisz "światową" pozę stołu (z offsetem w Z, jak wcześniej)
  geometry_msgs::msg::Pose table_pose_world = table_pose;
  table_pose_world.position.z += 0.5;

  // --- TF: ee -> gripper, cube -> base -------------------------------------
  if (!tf_buffer.canTransform(
        ee_frame, gripper_frame, tf2::TimePointZero, tf2::durationFromSec(5.0))) {
    RCLCPP_ERROR(logger, "Brak transformacji %s -> %s", gripper_frame.c_str(), ee_frame.c_str());
    rclcpp::shutdown();
    return 1;
  }

  geometry_msgs::msg::TransformStamped ee_to_gripper_msg;
  try {
    ee_to_gripper_msg = tf_buffer.lookupTransform(
      ee_frame, gripper_frame, tf2::TimePointZero);
  } catch (const tf2::TransformException & ex) {
    RCLCPP_ERROR(logger, "lookupTransform nie powiódł się: %s", ex.what());
    rclcpp::shutdown();
    return 1;
  }

  tf2::Transform cube_in_base;
  tf2::Transform e_to_gripper_tf;
  tf2::fromMsg(cube_pose, cube_in_base);
  tf2::fromMsg(ee_to_gripper_msg.transform, e_to_gripper_tf);

  // --- Wizualizacja pozycji kostki -----------------------------------------
  visual_tools.publishAxis(cube_pose, 0.15, 0.01, "green_cube_3");
  visual_tools.trigger();

  

  // --- Planning scene utils -------------------------------------------------
  moveit::planning_interface::PlanningSceneInterface psi;

  auto makeTableObject =
    [&](const geometry_msgs::msg::Pose & pose) -> moveit_msgs::msg::CollisionObject
  {
    moveit_msgs::msg::CollisionObject obj;
    obj.header.frame_id = planning_frame;
    obj.id = "table";
    obj.primitives.resize(1);
    obj.primitives[0].type = shape_msgs::msg::SolidPrimitive::BOX;
    obj.primitives[0].dimensions = {1.0, 0.8, 0.03};
    obj.primitive_poses.resize(1);
    obj.primitive_poses[0] = pose;
    obj.operation = obj.ADD;
    return obj;
  };

  auto makeCubeObject =
    [&](const geometry_msgs::msg::Pose & pose) -> moveit_msgs::msg::CollisionObject
  {
    moveit_msgs::msg::CollisionObject obj;
    obj.header.frame_id = planning_frame;
    obj.id = "green_cube_3";
    obj.primitives.resize(1);
    obj.primitives[0].type = shape_msgs::msg::SolidPrimitive::BOX;
    obj.primitives[0].dimensions = {0.07, 0.07, 0.07};
    obj.primitive_poses.resize(1);
    obj.primitive_poses[0] = pose;
    obj.operation = obj.ADD;
    return obj;
  };

  // Wyczyść potencjalne śmieci z poprzednich uruchomień
  move_group_interface.detachObject("green_cube_3");
  psi.removeCollisionObjects({"green_cube_3", "table"});
  rclcpp::sleep_for(200ms);

  // Dodaj stół i kostkę do sceny
  {
    std::vector<moveit_msgs::msg::CollisionObject> objects;
    objects.push_back(makeTableObject(table_pose_world));
    objects.push_back(makeCubeObject(cube_pose));
    psi.addCollisionObjects(objects);
    rclcpp::sleep_for(500ms);
  }


  // --- Gripper utils --------------------------------------------------------
  const std::string left_joint  =
    node->declare_parameter("gripper_left_joint",  "gripper_left_finger_joint");
  const std::string right_joint =
    node->declare_parameter("gripper_right_joint", "gripper_right_finger_joint");

  auto move_gripper =
  [&](double value, const std::string & action_label) -> bool
  {
    std::map<std::string, double> targets = {
      {left_joint,  value},
      {right_joint, value}
    };
    gripper_group.setJointValueTarget(targets);

    moveit::planning_interface::MoveGroupInterface::Plan plan;
    auto plan_result = gripper_group.plan(plan);
    if (plan_result != moveit::core::MoveItErrorCode::SUCCESS) {
      RCLCPP_WARN(
        logger, "Planowanie ruchu chwytaka do %s (%.3f) nie powiodło się.",
        action_label.c_str(), value
      );
      return false;
    }

    if (!execute_motion) {
      RCLCPP_INFO(
        logger,
        "[PLAN ONLY] Zaplanowano ruch chwytaka, akcja: %s (%.3f m), execute:=false – nie wykonuję.",
        action_label.c_str(), value
      );
      return true;
    }

    auto ec = gripper_group.execute(plan);
    if (ec != moveit::core::MoveItErrorCode::SUCCESS) {
      RCLCPP_WARN(
        logger, "Nie udało się %s chwytaka (wartość %.3f).",
        action_label.c_str(), value);
      return false;
    }

    RCLCPP_INFO(
      logger, "Chwytak %s (%.3f m).",
      action_label.c_str(), value);
    return true;
  };

  auto plan_and_execute =
    [&](const geometry_msgs::msg::Pose & goal, const std::string & label) -> bool
  {
    move_group_interface.setPoseTarget(goal, ee_frame);

    moveit::planning_interface::MoveGroupInterface::Plan plan;
    if (move_group_interface.plan(plan) != moveit::core::MoveItErrorCode::SUCCESS) {
      RCLCPP_WARN(logger, "Planowanie do %s nie powiodło się.", label.c_str());
      move_group_interface.clearPoseTargets();
      return false;
    }

    if (!execute_motion) {
      // TYLKO PLANOWANIE – żadnego ruchu fizycznego
      RCLCPP_INFO(
        logger,
        "[PLAN ONLY] Zaplanowano trajektorię do %s (execute:=false, nie wysyłam do robota).",
        label.c_str()
      );
      move_group_interface.clearPoseTargets();
      return true;
    }

    // WYKONANIE – jak dotychczas
    auto result = move_group_interface.execute(plan);
    move_group_interface.clearPoseTargets();
    if (result != moveit::core::MoveItErrorCode::SUCCESS) {
      RCLCPP_WARN(logger, "Wykonanie planu do %s nie powiodło się.", label.c_str());
      return false;
    }
    RCLCPP_INFO(logger, "Zrealizowano trajektorię do %s.", label.c_str());
    return true;
  };


  // --- Otwórz chwytak na start ----------------------------------------------
  move_gripper(open_val, "otworzyć");

  // --- Generowanie wielu chwytów --------------------------------------------
  tf2::Transform chosen_cube_to_top_gripper;
  tf2::Transform chosen_cube_to_pregrasp_gripper;
  geometry_msgs::msg::Pose chosen_pregrasp_pose;
  geometry_msgs::msg::Pose chosen_grasp_pose;
  double chosen_pitch, chosen_yaw;
  bool grasp_found = false;

  for (double pitch : approach_angles) {
    for (double yaw : approach_rotations) {

        // --- 1. Dodatkowy obrót chwytu (np. RZ 90°)
    tf2::Quaternion q_rot;
    q_rot.setRPY(0.0, 0.0, M_PI_2);  
    tf2::Transform rotate(q_rot, tf2::Vector3(0,0,0));

    // --- 2. Miejsce dotknięcia kostki
    tf2::Quaternion q_grip;
    q_grip.setRPY(0.0, pitch, yaw);
    tf2::Transform cube_to_top_local(q_grip, tf2::Vector3(0,0,cube_half_height));

    // pełna orientacja chwytaka względem kostki
    tf2::Transform cube_to_top_gripper = rotate * cube_to_top_local;

    // --- 3. Pre-grasp: odsunięcie wzdłuż osi Z chwytaka
    tf2::Vector3 approach_offset_local(0.0, 0.0, -approach_clearance);  // -Z: "przed" kostką
    tf2::Quaternion q_total = cube_to_top_gripper.getRotation();
    tf2::Vector3 approach_offset_rotated =
        tf2::quatRotate(q_total, approach_offset_local);

    tf2::Transform cube_to_pregrasp_gripper(
        cube_to_top_gripper.getRotation(),
        cube_to_top_gripper.getOrigin() + approach_offset_rotated
    );

    // --- 4. Do bazy
    tf2::Transform target_eef_in_base =
        cube_in_base * cube_to_top_gripper * e_to_gripper_tf.inverse();

    tf2::Transform pregrasp_eef_in_base =
        cube_in_base * cube_to_pregrasp_gripper * e_to_gripper_tf.inverse();


  geometry_msgs::msg::Pose pregrasp_pose;
  {
    const auto &o  = pregrasp_eef_in_base.getOrigin();
    const auto &rq = pregrasp_eef_in_base.getRotation();

    // wektor odsunięcia w układzie chwytaka: oś Z grippera
    tf2::Vector3 offset_local(0.0, 0.0, pregrasp_distance);
    // jeśli robot jedzie w złą stronę, zmień na -pregrasp_distance

    tf2::Vector3 offset_world = tf2::quatRotate(rq, offset_local);

    pregrasp_pose.position.x = o.x() + offset_world.x();
    pregrasp_pose.position.y = o.y() + offset_world.y();
    pregrasp_pose.position.z = o.z() + offset_world.z();
    pregrasp_pose.orientation = tf2::toMsg(rq);
  }

  geometry_msgs::msg::Pose grasp_pose;
  {
    const auto &o  = target_eef_in_base.getOrigin();
    const auto &rq = target_eef_in_base.getRotation();

    // ten sam mały offset wzdłuż osi chwytaka
    tf2::Vector3 offset_local(0.0, 0.0, pregrasp_distance);
    tf2::Vector3 offset_world = tf2::quatRotate(rq, offset_local);

    grasp_pose.position.x = o.x() + offset_world.x();
    grasp_pose.position.y = o.y() + offset_world.y();
    grasp_pose.position.z = o.z() + offset_world.z();
    grasp_pose.orientation = tf2::toMsg(rq);
  }


      RCLCPP_INFO(
        logger,
        "Próba chwytu: pitch=%.2f deg, yaw=%.2f deg",
        pitch * 180.0 / M_PI, yaw * 180.0 / M_PI);

      // wizualizacja aktualnego kandydata
      visual_tools.publishAxis(pregrasp_pose, 0.15, 0.01, "ee_pregrasp_candidate");
      visual_tools.publishAxis(grasp_pose, 0.15, 0.01, "ee_grasp_candidate");
      visual_tools.trigger();

      // spróbuj dojechać do pregrasp + grasp
      if (!plan_and_execute(pregrasp_pose, "pozycji pregrasp")) {
        continue;  // spróbuj kolejnego kąta
      }
      if (!plan_and_execute(grasp_pose, "pozycji grasp")) {
        continue;  // też spróbuj kolejnego kąta
      }

      // Udało się – zapamiętaj użyty transform i pozycje
      chosen_cube_to_top_gripper = cube_to_top_gripper;
      chosen_cube_to_pregrasp_gripper = cube_to_pregrasp_gripper;
      chosen_pregrasp_pose = pregrasp_pose;
      chosen_grasp_pose = grasp_pose;
      chosen_pitch = pitch;
      chosen_yaw = yaw;
      grasp_found = true;
      break;
    }
    if (grasp_found) break;
  }

  if (!grasp_found) {
    RCLCPP_ERROR(logger, "Nie udało się znaleźć żadnego poprawnego chwytu.");
    rclcpp::shutdown();
    return 1;
  }

   // --- Attach kostki i zamknięcie chwytaka ----------------------------------
  {
    std::vector<std::string> touch_links = gripper_group.getLinkNames();
    move_group_interface.attachObject("green_cube_3", ee_frame, touch_links);
    RCLCPP_INFO(logger, "Attached 'green_cube_3' do %s.", ee_frame.c_str());
  }

  move_gripper(closed_val, "zamknąć");

  // Na czas podniesienia ignorujemy stół w planning scene
  psi.removeCollisionObjects({"table"});
  rclcpp::sleep_for(100ms);

  // lekki podjazd w górę
  geometry_msgs::msg::Pose lift_pose = chosen_grasp_pose;
  lift_pose.position.z += 0.05;

  if (!plan_and_execute(lift_pose, "podniesienia 5 cm")) {
    RCLCPP_WARN(logger, "Podniesienie 5 cm nie powiodło się. Sprzątanie sceny i zakończenie.");

    // Odczep kostkę i przywróć czystą scenę
    move_group_interface.detachObject("green_cube_3");
    psi.removeCollisionObjects({"green_cube_3"});

    std::vector<moveit_msgs::msg::CollisionObject> objects;
    objects.push_back(makeTableObject(table_pose_world));  // stół z powrotem
    objects.push_back(makeCubeObject(cube_pose));          // kostka tam gdzie była
    psi.addCollisionObjects(objects);
    rclcpp::sleep_for(500ms);

    rclcpp::shutdown();
    return 1;
  }

  // --- Faza odkładania: lustro po Y -----------------------------------------
  geometry_msgs::msg::Pose mirror_cube_pose = cube_pose;
  mirror_cube_pose.position.y = -mirror_cube_pose.position.y;

  tf2::Transform cube_mirror_in_base;
  tf2::fromMsg(mirror_cube_pose, cube_mirror_in_base);

  tf2::Transform place_eef_in_base =
    cube_mirror_in_base * chosen_cube_to_top_gripper * e_to_gripper_tf.inverse();
  tf2::Transform preplace_eef_in_base =
    cube_mirror_in_base * chosen_cube_to_pregrasp_gripper * e_to_gripper_tf.inverse();

  geometry_msgs::msg::Pose preplace_pose;
  {
    const auto &o = preplace_eef_in_base.getOrigin();
    const auto &rq = preplace_eef_in_base.getRotation();
    preplace_pose.position.x = o.x();
    preplace_pose.position.y = o.y();
    preplace_pose.position.z = o.z();
    preplace_pose.orientation = tf2::toMsg(rq);
  }

  geometry_msgs::msg::Pose place_pose;
  {
    const auto &o = place_eef_in_base.getOrigin();
    const auto &rq = place_eef_in_base.getRotation();
    place_pose.position.x = o.x();
    place_pose.position.y = o.y();
    place_pose.position.z = o.z();
    place_pose.orientation = tf2::toMsg(rq);
  }

  if (!plan_and_execute(preplace_pose, "pozycji pre-putdown")) {
    RCLCPP_WARN(logger, "Planowanie do pre-putdown nie powiodło się. Pomijam odkładanie.");
  } else {
    if (!plan_and_execute(place_pose, "pozycji putdown")) {
      RCLCPP_WARN(logger, "Planowanie do putdown nie powiodło się.");
    } else {
      // otwórz chwytak i odczep kostkę
      move_gripper(open_val, "otworzyć po odłożeniu");
      move_group_interface.detachObject("green_cube_3");
      rclcpp::sleep_for(150ms);

      // uporządkuj scenę: usuń i dodaj stół + kostkę w nowych pozach
      psi.removeCollisionObjects({"green_cube_3", "table"});
      rclcpp::sleep_for(100ms);

      std::vector<moveit_msgs::msg::CollisionObject> objects;
      objects.push_back(makeTableObject(table_pose_world));
      objects.push_back(makeCubeObject(mirror_cube_pose));
      psi.addCollisionObjects(objects);
      rclcpp::sleep_for(500ms);

      RCLCPP_INFO(logger, "Kostka odłożona w (x0, -y0, z0) i scena zaktualizowana.");
    }
  }

  rclcpp::shutdown();
  return 0;
}