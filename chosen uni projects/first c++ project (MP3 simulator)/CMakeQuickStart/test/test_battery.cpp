#include <catch2/catch_test_macros.hpp>
#include "battery.hpp"

using json = nlohmann::json;

TEST_CASE("Creation"){
    Battery battery{90};
    REQUIRE(battery.is_full());
    REQUIRE(battery.get_battery_left() == 90);
}

TEST_CASE("Default value"){
    Battery battery;
    REQUIRE(battery.get_battery_left() == 100);
}

TEST_CASE("Setting capacity"){
    Battery battery;
    battery.change_capacity(90);
    REQUIRE(battery.get_battery_left() == 90);
    REQUIRE(battery.is_full());
}

TEST_CASE("Setting capacity bigger than battery left"){
    Battery battery{90};
    REQUIRE(battery.is_full());

    battery.change_capacity(100);
    REQUIRE(!battery.is_full());
}

TEST_CASE("Setting battery left"){
    Battery battery;
    REQUIRE(battery.is_full());
    REQUIRE(battery.get_battery_left() == 100);

    battery.change_battery_left(90);
    REQUIRE(battery.get_battery_left() == 90);

    battery.change_battery_left(110);
    REQUIRE(battery.is_full());
    REQUIRE(battery.get_battery_left() == 100);

    battery.change_battery_left(0);
    REQUIRE(battery.is_empty());
    REQUIRE(battery.get_battery_left() == 0);
}

TEST_CASE("Charging"){
    Battery battery;
    REQUIRE(battery.get_battery_left() == 100);

    battery.discharge(10);
    REQUIRE(battery.get_battery_left() == 90);

    battery.discharge(91);
    REQUIRE(battery.is_empty());

    battery.charge(10);
    REQUIRE(battery.get_battery_left() == 10);

    battery.charge(91);
    REQUIRE(battery.is_full());
}

TEST_CASE("Json out"){
    Battery battery;
    battery.change_capacity(90);
    battery.change_battery_left(60);

    json data = battery;
    REQUIRE(data.at("capacity") == 90);
    REQUIRE(data.at("battery_left") == 60);
}

TEST_CASE("From json"){
    nlohmann::json data{
            {"capacity", 70},
            {"battery_left", 60}
    };

    Battery battery{data};
    REQUIRE(battery.get_capacity() == 70);
    REQUIRE(battery.get_battery_left() == 60);
}

TEST_CASE("Setting from json"){
    nlohmann::json data{
            {"capacity", 70},
            {"battery_left", 60}
    };

    Battery battery = data.template get<Battery>();

    REQUIRE(battery.get_capacity() == 70);
    REQUIRE(battery.get_battery_left() == 60);
}

//TEST_CASE("From invalid json - capacity < battery_left"){
//    nlohmann::json data{
//            {"capacity", 59},
//            {"battery_left", 60}
//    };
//
//    REQUIRE_THROWS_AS(data.template get<Battery>(), std::invalid_argument);
//}

//TEST_CASE("Setting from invalid json - capacity < battery_left"){
//    nlohmann::json data{
//            {"capacity", 59},
//            {"battery_left", 60}
//    };
//
//    REQUIRE_THROWS_AS(data.template get<Battery>(), std::invalid_argument);
//}
