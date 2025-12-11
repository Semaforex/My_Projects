#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <unordered_set>

using json = nlohmann::json;

class Battery {
    private:
        unsigned int capacity_;
        unsigned int battery_left_;

        [[nodiscard]] static bool valid_json(const nlohmann::json& json_data);
    public:
        explicit Battery(unsigned int Capacity = 100);
        void charge(unsigned int power);
        void discharge(unsigned int consumption);
        [[nodiscard]] unsigned int get_battery_left() const;
        [[nodiscard]] unsigned int get_capacity() const;
        [[nodiscard]] bool is_empty() const;
        [[nodiscard]] bool is_full() const;
        void change_capacity(unsigned int Capacity);
        void change_battery_left(unsigned int Battery_left);
};

inline void to_json(json& j, const Battery& battery) {
    j = json{
        {"battery_left", battery.get_battery_left()},
        {"capacity", battery.get_capacity()}
    };
}

inline void from_json(const json& j, Battery& battery) {
    battery.change_capacity(j.at("capacity"));
    battery.change_battery_left(j.at("battery_left"));
}
