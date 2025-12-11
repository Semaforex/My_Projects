#include "battery.hpp"
#include <algorithm>

Battery::Battery(unsigned int Capacity){
    capacity_ = Capacity;
    battery_left_ = capacity_;
}

//Battery::Battery(nlohmann::json json_data) {
//    auto valid = valid_json(json_data);
//    if(!valid) throw std::invalid_argument{"Invalid battery json data"};
//    battery_left_ = json_data.at("battery_left");
//    capacity_ = json_data.at("capacity");
//}

void Battery::charge(unsigned int power){
    change_battery_left(battery_left_ + power);
}

void Battery::discharge(unsigned int consumption){
    if(consumption > battery_left_) change_battery_left(0);
    else change_battery_left(battery_left_ - consumption);
}

unsigned int Battery::get_battery_left() const{
    return battery_left_;
}

bool Battery::is_empty() const{
    if(battery_left_ == 0) return true;
    return false;
}

bool Battery::is_full() const{
    if(capacity_ == battery_left_) return true;
    return false;
}

void Battery::change_capacity(unsigned int Capacity){
    capacity_ = Capacity;
    battery_left_ = std::min(capacity_, battery_left_);
}

void Battery::change_battery_left(unsigned int Battery_left){
    battery_left_ = std::min(capacity_, Battery_left);
}

//nlohmann::json Battery::to_json() const{
//    nlohmann::json data{
//            {"capacity", capacity_},
//            {"battery_left", battery_left_}
//    };
//
//    return data;
//}
//
//void Battery::from_json(nlohmann::json json_data){
//    auto valid = valid_json(json_data);
//    if(!valid) throw std::invalid_argument{"Invalid battery json data"};
//    battery_left_ = json_data.at("battery_left");
//    capacity_ = json_data.at("capacity");
//}

unsigned int Battery::get_capacity() const {
    return capacity_;
}

bool Battery::valid_json(const nlohmann::json& json_data){
    auto capacity = json_data.value("capacity", -1);
    auto battery_left = json_data.value("battery_left", -1);

    if(capacity < 0) return false;
    if(battery_left < 0) return false;

    if(capacity < battery_left) return false;

    return true;
}
