#include "my_bot/include/dvl_hardware.hpp"
#include "pluginlib/class_list_macros.hpp"


namespace my_bot
{

hardware_interface::CallbackReturn
DVLHardware::on_init(const hardware_interface::HardwareInfo & info)
{
  RCLCPP_INFO(rclcpp::get_logger("DVLHardware"), "DVL INIT");

  dvl_topic_ = info.hardware_parameters.at("dvl_topic");

  vx_ = 0.0;
  vy_ = 0.0;
  vz_ = 0.0;

  return hardware_interface::CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn
DVLHardware::on_configure(const rclcpp_lifecycle::State &)
{
  RCLCPP_INFO(rclcpp::get_logger("DVLHardware"), "DVL CONFIGURE");

  node_ = std::make_shared<rclcpp::Node>("dvl_hardware");

  sub_ = node_->create_subscription<std_msgs::msg::Float64MultiArray>(
    dvl_topic_, 10,
    [this](const std_msgs::msg::Float64MultiArray::SharedPtr msg)
    {
      vx_ = msg->data[0];
      vy_ = msg->data[1];
      vz_ = msg->data[2];

      RCLCPP_INFO(node_->get_logger(),
        "DVL DATA: %.2f %.2f %.2f",
        vx_, vy_, vz_);
    });

  executor_ = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();
  executor_->add_node(node_);

  std::thread([this]() {
    executor_->spin();
  }).detach();

  return hardware_interface::CallbackReturn::SUCCESS;
}

std::vector<hardware_interface::StateInterface>
DVLHardware::export_state_interfaces()
{
  return {
    {"dvl_frame", "vx", &vx_},
    {"dvl_frame", "vy", &vy_},
    {"dvl_frame", "vz", &vz_}
  };
}

hardware_interface::return_type
DVLHardware::read(const rclcpp::Time &, const rclcpp::Duration &)
{
  return hardware_interface::return_type::OK;
}

}  // namespace my_bot

PLUGINLIB_EXPORT_CLASS(my_bot::DVLHardware, hardware_interface::SystemInterface)