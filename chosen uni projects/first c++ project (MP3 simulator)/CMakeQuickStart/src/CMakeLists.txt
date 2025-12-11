add_library(song song.cpp song.hpp)
add_library(playlist playlist.cpp playlist.hpp)
add_library(player player.cpp player.hpp)
add_library(memory memory.cpp memory.hpp)
add_library(battery battery.cpp battery.hpp)
add_library(interface interface.cpp interface.hpp)
target_include_directories(song PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(playlist PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(player PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(battery PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(memory PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(interface PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(playlist PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(song PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(player PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(memory PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(battery PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(playlist PRIVATE fmt)
target_link_libraries(song PRIVATE fmt)
target_link_libraries(player PRIVATE fmt)

target_link_libraries(interface PRIVATE song)

target_link_libraries(interface
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
)

target_link_libraries(player PUBLIC song playlist memory battery interface)