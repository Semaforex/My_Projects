include(AddCatch2)

add_executable(
  unit_tests
  #test_battery.cpp
  test_playlist.cpp
  test_song.cpp
  test_memory.cpp
  test_battery_json.cpp
)

target_link_libraries(
  unit_tests PRIVATE battery playlist song memory nlohmann_json::nlohmann_json Catch2::Catch2WithMain
)

include(CTest)
include(Catch)

add_executable(
  test_battery
  test_battery.cpp
)
target_link_libraries(
        test_battery PRIVATE battery Catch2::Catch2WithMain
)
catch_discover_tests(test_battery)

add_executable(
    test_battery_to_json
        test_battery_json.cpp
)
target_link_libraries(
        test_battery_to_json PRIVATE battery Catch2::Catch2WithMain
)
catch_discover_tests(test_battery_to_json)
