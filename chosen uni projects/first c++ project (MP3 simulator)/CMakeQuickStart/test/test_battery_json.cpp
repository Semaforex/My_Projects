#include <catch2/catch_test_macros.hpp>
#include "battery.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST_CASE("to_json"){
    Battery battery{100};
    json data = battery;

    REQUIRE(data.at("battery_left") == 100);
    REQUIRE(data.at("capacity") == 100);
}

TEST_CASE("from_json"){
    json data = {
            {"battery_left", 10},
            {"capacity", 90}
    };

    Battery battery = data.template get<Battery>();
    REQUIRE(battery.get_capacity() == 90);
    REQUIRE(battery.get_battery_left() == 10);
}
