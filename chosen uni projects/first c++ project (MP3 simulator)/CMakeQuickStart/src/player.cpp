#include "player.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <thread>
#include <fmt/core.h>

enum message{
    song_add_success = 0,
    song_add_fail = 1,
    song_remove_success = 2,
    playlist_add_success = 3,
    playlist_add_fail = 4,
    playlist_remove_success = 5,
    song_playlist_add_success = 6,
    song_playlist_add_fail = 7,
    song_playlist_remove_success = 8,
    song_playlist_remove_fail = 9,
    song_not_found = 10,
    playlist_not_found = 11,
    battery_empty = 12
};

// pomieszane info

static const char* message_strings[] = {
    "Song added successfully",
    "Song not added",
    "Song removed successfully",
    "Playlist added successfully",
    "Playlist not added",
    "Playlist removed successfully",
    "Song added to playlist successfully",
    "Song not added to playlist",
    "Song removed from playlist successfully",
    "Song not removed from playlist",
    "Song not found",
    "Playlist not found",
    "Battery is empty"
};


Player::Player(std::string Path, std::string Name, unsigned int Battery_capacity, unsigned int Memory_capacity):
    memory_{Memory()},
    path_{Path},
    battery_{Battery(Battery_capacity)},
    name_{Name},
    interface_{Interface()}
{
    if(name_.empty())
        throw(std::invalid_argument("Name cannot be empty"));
    memory_.set_capacity(Memory_capacity);
}

Player::Player(std::string Path) : 
    path_{Path},
    interface_{Interface()}
{
    nlohmann::json json_data;
    try{
        std::ifstream file{std::filesystem::absolute(Path)};
        if(!file.is_open()){
            throw std::invalid_argument{"Couldn't opened json file"};
        }

        file >> json_data;

        memory_ = Memory(json_data["memory"]);
        battery_ = Battery(json_data["battery"]);
    }catch(std::invalid_argument& e){
        battery_ = Battery(100);
        memory_ = Memory();
        name_ = "default";
    }
}

void Player::add_song(Song song){
    if(!battery_action())
        return;
    std::pair<bool, std::string> result = memory_.add_song(song);
    if(result.first)
        interface_.display_message(message_strings[song_add_success]);
    else{
        interface_.display_message(message_strings[song_add_fail]);
        interface_.display_message(result.second);
    }
}

void Player::remove_song(Song song){
    if(!battery_action())
        return;
    if(memory_.remove_song(song))
        interface_.display_message(message_strings[song_remove_success]);
    else
        interface_.display_message(message_strings[song_not_found]);
}

void Player::add_playlist(Playlist playlist){
    if(!battery_action())
        return;
    if(memory_.add_playlist(playlist))
        interface_.display_message(message_strings[playlist_add_success]);
    else
        interface_.display_message(message_strings[playlist_add_fail]);
}

void Player::remove_playlist(Playlist playlist){
    if(!battery_action())
        return;
    if(memory_.remove_playlist(playlist))
        interface_.display_message(message_strings[playlist_remove_success]);
    else
        interface_.display_message(message_strings[playlist_not_found]);
}

void Player::add_song_to_playlist(Playlist playlist, Song song){
    if(!battery_action())
        return;
    auto result = memory_.add_song_to_playlist(playlist, song);
    if(result.first)
        interface_.display_message(message_strings[song_playlist_add_success]);
    else{
        interface_.display_message(message_strings[song_playlist_add_fail]);
        interface_.display_message(result.second);
    }
}

void Player::remove_song_from_playlist(Playlist playlist, Song song){
    if(!battery_action())
        return;
    auto result = memory_.remove_song_from_playlist(playlist, song);
    if(result.first)
        interface_.display_message(message_strings[song_playlist_remove_success]);
    else{
        interface_.display_message(message_strings[song_playlist_remove_fail]);
        interface_.display_message(result.second);
    }
}

void Player::display_playlist_info(std::string Name, std::string Author){
    if(!battery_action())
    return;
    try{
        std::string result = "";
        Playlist playlist = memory_.find_playlist(Name, Author);
        if(playlist.get_name() == "")
            interface_.display_message(message_strings[playlist_not_found]);
        result += fmt::format("Playlist: {} by {}\n", playlist.get_name(), playlist.get_author());
        for(const auto& song_hash : playlist.get_song_list()){
            Song song = memory_.find_song(song_hash);
            result += fmt::format("Song: {} by {}\n", song.get_title(), song.get_artist());
        }
        interface_.display_message(result);
    }
    catch(std::invalid_argument& e){
        interface_.display_message(e.what());
    }
}

void Player::display_song_info(std::string Title, std::string Artist){
    if(!battery_action())
        return;
    Song song = memory_.find_song(Title, Artist);
    if(song.get_title() == "")
        interface_.display_message(message_strings[song_not_found]);
    interface_.display_message(fmt::format("Song: {} by {}\n", song.get_title(), song.get_artist()));
}

void Player::play_playlist(std::string name, std::string author){
    try{
        Playlist playlist = memory_.find_playlist(name, author);
        std::string message;
        for(size_t song_hash : playlist.get_song_list()){
            Song song = memory_.find_song(song_hash);
            play(song);
        }
    }catch(std::invalid_argument& e){
        interface_.display_message(e.what());
    }
}


void Player::play_song(std::string Artist, std::string Title){
    Song song = memory_.find_song(Title, Artist);
    play(song);
}

void Player::play_all_songs(){
    std::string message;
    for(Song song: memory_.get_songs()){
        play(song);
        if(battery_.get_battery_left() == 0)
            return;
    }
}

void Player::charge_battery(unsigned int Battery_capacity){
    battery_.charge(Battery_capacity);
}

void Player::show_battery(){
    if(!battery_action())
        return;
    interface_.display_message(fmt::format("Battery: {}%", (double) 100 * battery_.get_battery_left()/battery_.get_capacity()));
}

void Player::show_storage(){
    if(!battery_action())
        return;
    std::string result = "";
    for(const auto& playlist : memory_.get_playlists()){
        result += fmt::format("Playlist: {} by {}\n", playlist.get_name(), playlist.get_author());
        for(const auto& song_hash : playlist.get_song_list()){
            Song song = memory_.find_song(song_hash);
            result += fmt::format("Song: {} by {}\n", song.get_title(), song.get_artist());
        }
    }
    interface_.display_message(result);
}

bool Player::battery_action(){
    battery_.discharge(1);
    if(battery_.get_battery_left() == 0){
        interface_.display_message(message_strings[battery_empty]);
        return false;
    }
    return true;
}

void Player::list_all_songs(){
    if(!battery_action())
        return;
    for(const auto& song : memory_.get_songs())
        interface_.display_message(fmt::format("Song: {} by {}\n", song.get_title(), song.get_artist()));
}

Player::~Player(){
    nlohmann::json json_data;
    json_data["memory"] = memory_;
    json_data["battery"] = battery_;

    std::ofstream file{path_};
    if(!file.is_open()){
        interface_.display_message("Saving error");
    }


    file << json_data.dump(4);
    file.close();
}

void Player::display_all_playlists_info() {
    auto playlists = memory_.get_playlists();
    std::for_each(playlists.begin(), playlists.end(), [&](const auto &item){
        display_playlist_info(item.get_name(),item.get_author());
    });
}

void Player::play(Song song){
    for(unsigned int timer = song.get_length(); timer > 0; timer--){
            if(!battery_action())
                return;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            interface_.display_current_song(song, song.get_length() - timer);
        }
}
